// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#ifdef _MSC_VER
// warning C4201: nonstandard extension used: nameless struct/union
#pragma warning(push)
#pragma warning(disable : 4201)
#endif
#include <paho-mqtt/MQTTClient.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
// Required for Sleep(DWORD)
#include <Windows.h>
#else
// Required for sleep(unsigned int)
#include <unistd.h>
#endif

#include <azure/core/az_result.h>
#include <azure/core/az_span.h>
#include <azure/iot/az_iot_provisioning_client.h>
// TODO: can't be internal:
#include <azure/iot/internal/az_iot_reference.h>

typedef struct evt_struct
{
  uint8_t type;
  char string[10];
} evt_struct;

#define Q_SIZE 2
#define Q_TYPE evt_struct
#include <azure/iot/internal/az_iot_queue.h>

// Clients
static az_iot_provisioning_client provisioning_client;

enum
{
  STATE_ENTER = 0x1,
  STATE_EXIT = 0x2,
};

struct hsm
{
  struct hsm * super_hsm;
  az_result (*currentState)(struct hsm *this, int event_type, void* event_data);
  // no queue, one event only.
} test_hsm;

static az_result initial_state(struct hsm *this, int event_type, void* event_data);
static az_result state_a(struct hsm *this, int event_type, void* event_data);

static az_result initial_state(struct hsm *this, int event_type, void* event_data)
{
  az_result ret = AZ_OK;

  switch(event_type)
  {
    case STATE_ENTER:
      LOG_SUCCESS("initial_state: event STATE_ENTER");

      // Change states w/o queue:
      this->currentState = state_a;
      this->currentState(this, STATE_ENTER, NULL);
      break;

    default:
      ret = this->super_hsm->currentState(this->super_hsm, event_type, event_data);
  }

  return ret;
}

static az_result state_a(struct hsm *this, int event_type, void* event_data)
{
  az_result ret = AZ_OK;

  switch(event_type)
  {
    case STATE_ENTER:
      LOG_SUCCESS("state_a: event STATE_ENTER");
      break;

    default:
      ret = this->super_hsm->currentState(this->super_hsm, event_type, event_data);
  }

  return ret; 
}


int main()
{
  LOG_SUCCESS("Started.");

  test_hsm.currentState = initial_state;
  test_hsm.currentState(&test_hsm, STATE_ENTER, NULL);

  az_iot_queue q;
  az_iot_queue_init(&q);

  evt_struct e1 = { 1, "Hello 1" };
  evt_struct e2 = { 2, "Hello 2" };
  evt_struct e3 = { 3, "Hello 3" };

  az_iot_queue_enqueue(&q, &e1);
  az_iot_queue_enqueue(&q, &e2);
  az_iot_queue_enqueue(&q, &e3);

  evt_struct* ret;
  ret = az_iot_queue_dequeue(&q);
  ret = az_iot_queue_dequeue(&q);
  ret = az_iot_queue_dequeue(&q);

  az_iot_queue_enqueue(&q, &e1);
  ret = az_iot_queue_dequeue(&q);
  az_iot_queue_enqueue(&q, &e2);
  ret = az_iot_queue_dequeue(&q);
  az_iot_queue_enqueue(&q, &e3);
  ret = az_iot_queue_dequeue(&q);
  az_iot_queue_enqueue(&q, &e1);
  ret = az_iot_queue_dequeue(&q);
  az_iot_queue_enqueue(&q, &e2);
  ret = az_iot_queue_dequeue(&q);
  az_iot_queue_enqueue(&q, &e3);
  ret = az_iot_queue_dequeue(&q);




  return 0;
}

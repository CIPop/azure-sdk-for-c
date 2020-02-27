// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef _az_IOT_HUB_CLIENT_H
#define _az_IOT_HUB_CLIENT_H

#include <az_result.h>
#include <az_span.h>

#include <_az_cfg_prefix.h>

typedef struct az_iot_hub_client_options {
  az_span module_id;
  az_span user_agent;
} az_iot_hub_client_options;

typedef struct az_iot_hub_client {
  struct {
    az_span iot_hub_hostname;
    az_span device_id;
    az_iot_hub_client_options options;
  } _internal;
} az_iot_hub_client;

az_iot_hub_client_options az_iot_hub_client_options_default();

// TODO : add returns to all init functions.
AZ_NODISCARD az_result az_iot_hub_client_init(
    az_iot_hub_client const * client,
    az_span iot_hub_hostname,
    az_span device_id,
    az_iot_hub_client_options * options);

AZ_NODISCARD az_result az_iot_hub_client_user_name_get(
    az_iot_hub_client const * client,
    az_span mqtt_user_name,
    az_span * out_mqtt_user_name);

AZ_NODISCARD az_result az_iot_hub_client_id_get(
    az_iot_hub_client const * client,
    az_span mqtt_client_id,
    az_span * out_mqtt_client_id);


AZ_NODISCARD az_result az_iot_hub_client_sas_signature_get(
    az_iot_hub_client const * client,
    uint32_t token_expiration_unix_time,
    az_span signature,
    az_span * out_signature);

AZ_NODISCARD az_result az_iot_hub_client_sas_password_get(
    az_iot_hub_client const * client,
    az_span base64_hmac_sha256_signature,
    az_span key_name,
    az_span mqtt_password,
    az_span * out_mqtt_password);


#include <_az_cfg_suffix.h>

#endif //!_az_IOT_HUB_CLIENT_H

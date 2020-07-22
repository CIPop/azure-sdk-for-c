// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

/**
 * @file az_iot_queue.h
 *
 * @brief Azure IoT common definitions.
 *
 * @note You MUST NOT use any symbols (macros, functions, structures, enums, etc.)
 * prefixed with an underscore ('_') directly in your application code. These symbols
 * are part of Azure SDK's internal implementation; we do not document these symbols
 * and they are subject to change in future versions of the SDK which would break your code.
 */

#ifndef _az_IOT_QUEUE_INTERNAL_H
#define _az_IOT_QUEUE_INTERNAL_H

#include <stdint.h>
#include <azure/core/az_result.h>
#include <azure/core/internal/az_precondition_internal.h>

#include <azure/core/_az_cfg_prefix.h>

#ifndef Q_SIZE
#define Q_SIZE 5
#endif

#ifndef Q_TYPE
#define Q_TYPE uint32_t
#endif

#ifndef Q_IDX_TYPE
#define Q_IDX_TYPE int32_t
#endif

/**
 * @brief Azure IoT Queue
 *
 */
typedef struct az_iot_queue
{
  struct
  {
    Q_TYPE data[Q_SIZE];
    Q_IDX_TYPE start_idx;
    Q_IDX_TYPE end_idx;
    Q_IDX_TYPE count;
  } _internal;
} az_iot_queue;

AZ_INLINE void az_iot_queue_init(az_iot_queue* q)
{
    _az_PRECONDITION_NOT_NULL(q);

    q->_internal.count = 0;
    q->_internal.start_idx = 0;
    q->_internal.end_idx = 0;
}

AZ_INLINE bool az_iot_queue_enqueue(az_iot_queue* q, Q_TYPE* element)
{
    _az_PRECONDITION_NOT_NULL(q);

    if (q->_internal.count < Q_SIZE)
    {
        q->_internal.data[q->_internal.end_idx] = *element;
        q->_internal.end_idx = (q->_internal.end_idx + 1) % Q_SIZE;
        q->_internal.count++;

        return true;
    }

    return false;
}

AZ_INLINE Q_TYPE* az_iot_queue_dequeue(az_iot_queue* q)
{
    _az_PRECONDITION_NOT_NULL(q);

    Q_TYPE* ret = NULL;
    if (q->_internal.count > 0)
    {
        ret = &(q->_internal.data[q->_internal.start_idx]);
        q->_internal.start_idx = (q->_internal.start_idx + 1) % Q_SIZE;
        q->_internal.count--;
    }

    return ret;
}

#include <azure/core/_az_cfg_suffix.h>

#endif //!_az_IOT_QUEUE_INTERNAL_H

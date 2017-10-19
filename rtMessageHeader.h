/* Copyright [2017] [Comcast, Corp.]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __RT_MESSAGE_HEADER_H__
#define __RT_MESSAGE_HEADER_H__

#include "rtError.h"
#include <stdint.h>

#define RTMSG_HEADER_MAX_TOPIC_LENGTH 128

// size of all fields in 
// #define RTMSG_HEADER_SIZE (24 + (2 * RTMSG_HEADER_MAX_TOPIC_LENGTH))

typedef struct
{
  uint16_t version;
  uint16_t header_length;
  uint32_t sequence_number;
  uint32_t flags;
  uint32_t control_data;
  uint32_t payload_length;
  uint32_t topic_length;
  char     topic[RTMSG_HEADER_MAX_TOPIC_LENGTH];
  uint32_t reply_topic_length;
  char     reply_topic[RTMSG_HEADER_MAX_TOPIC_LENGTH];
} rtMessageHeader;

rtError rtMessageHeader_Init(rtMessageHeader* hdr);
rtError rtMessageHeader_Encode(rtMessageHeader* hdr, uint8_t* buff);
rtError rtMessageHeader_Decode(rtMessageHeader* hdr, uint8_t const* buff);

#endif

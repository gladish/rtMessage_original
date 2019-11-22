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
#include "rtError.h"
#include "rtMessage.h"
#include "rtVector.h"

#ifdef WITH_CJSON
#include <cJSON.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef WITH_CJSON
struct _rtMessage
{
  cJSON*  json;
  int     count;
};
#else

typedef struct
{
  union
  {
    char*               str;
    int32_t             i32;
    double              d;
    struct _rtMessage*  msg;
  } data;
} rtValue;

struct _rtMessage
{
  int         count;
  rtVector    props;
};
#endif

/**
 * Allocate storage and initializes it as new message
 * @param pointer to the new message
 * @return rtError
 **/
rtError
rtMessage_Create(rtMessage* message)
{
  struct _rtMessage* m = malloc(sizeof(struct _rtMessage));
  if (!m)
    return RT_FAIL;

#ifdef WITH_CJSON
  m->json = cJSON_CreateObject();
  m->count = 1;
#else
  m->count = 1;

  // TODO
  assert(0);
#endif

  *message = m;

  return RT_OK;
}

/**
 * Copies only the data within the fields of the message
 * @param message to be copied
 * @param pointer to new copy of the message
 * @return rtError
 */
rtError
rtMessage_Clone(rtMessage const message, rtMessage* copy)
{
  *copy = (rtMessage) malloc(sizeof(struct _rtMessage));

#ifdef WITH_CJSON
  if (copy)
  {
    (*copy)->json = cJSON_Duplicate(message->json, 1);
    (*copy)->count = 1;
    return RT_OK;
  }
#else
  // TODO
  assert(0);
#endif

  return RT_FAIL;
}

/* Allocates storage and initializes it as new message
 * @param pointer to the new message
 * @param fill the new message with this data
 * @return rtError
 **/
rtError
rtMessage_FromBytes(rtMessage* message, uint8_t const* bytes, int n)
{
  (void) n;

  #if 0
  printf("------------------------------------------\n")
  for (i = 0; i < 256; ++i)
  {
    if (i > 0)
      printf(" ");
    if (i % 16 == 0)
      printf("\n");
    printf("0x%02x", bytes[i]);
  }
  printf("\n\n");
  #endif

#ifdef WITH_CJSON
  *message = (rtMessage) malloc(sizeof(struct _rtMessage));
  if (message)
  {
    (*message)->json = cJSON_Parse((char *) bytes);
    return RT_OK;
  }
#else
  // TODO
  assert(0);
#endif

  return RT_FAIL;
}

/**
 * Destroy a message; free the storage that it occupies.
 * @param pointer to message to be destroyed
 * @return rtError
 **/
rtError
rtMessage_Destroy(rtMessage message)
{
#ifdef WITH_CJSON
  if ((message) && ((message)->count == 0))
  {
    if (message->json)
      cJSON_Delete(message->json);
    free(message);
    return RT_OK;
  }
#else
  // TODO
  assert(0);
#endif

  return RT_FAIL;
}

/**
 * Extract the data from a message as a byte sequence.
 * @param extract the data bytes from this message.
 * @param pointer to the byte sequence location 
 * @param pointer to number of bytes in the message
 * @return rtErro
 **/
rtError
rtMessage_ToByteArray(rtMessage message, uint8_t** buff, uint32_t *n)
{
#ifdef WITH_CJSON
  return rtMessage_ToString(message, (char **) buff, n);
#else
  // TODO
  assert(0);
  return RT_OK;
#endif
}

/**
 * Format message as string
 * @param message to be converted to string
 * @param pointer to a string where message is to be stored
 * @param  pointer to number of bytes in the message
 * @return rtStatus
 **/
rtError
rtMessage_ToString(rtMessage const m, char** s, uint32_t* n)
{
#ifdef WITH_CJSON
  *s = cJSON_PrintUnformatted(m->json);
  *n = strlen(*s);
#else
  // TODO
  assert(0);
#endif

  return RT_OK;
}

/**
 * Add string field to the message
 * @param message to be modified
 * @param name of the field to be added
 * @param value of the field to be added
 * @return void
 **/
void
rtMessage_SetString(rtMessage message, char const* name, char const* value)
{
#ifdef WITH_CJSON
   cJSON_AddItemToObject(message->json, name, cJSON_CreateString(value));
#else
  // TODO
  assert(0);
#endif
}

/**
 * Add integer field to the message
 * @param message to be modified
 * @param name of the field to be added
 * @param integer value of the field to be added
 * @return void
 **/
void
rtMessage_SetInt32(rtMessage message, char const* name, int32_t value)
{
#ifdef WITH_CJSON
  cJSON_AddNumberToObject(message->json, name, value); 
#else
  // TODO
  assert(0);
#endif
}

/**
 * Add double field to the message
 * @param message to be modified
 * @param name of the field to be added
 * @param double value of the field to be added
 * @return void
 **/
void
rtMessage_SetDouble(rtMessage message, char const* name, double value)
{
#ifdef WITH_CJSON
  cJSON_AddItemToObject(message->json, name, cJSON_CreateNumber(value));
#else
  // TODO
  assert(0);
#endif
}

/**
 * Add sub message field to the message
 * @param message to be modified
 * @param name of the field to be added
 * @param new message item to be added
 * @return rtError
 **/
rtError
rtMessage_SetMessage(rtMessage message, char const* name, rtMessage item)
{
  if (!message || !item)
    return RT_ERROR_INVALID_ARG;

#ifdef WITH_CJSON
  if (item->json)
  {
    cJSON* obj = cJSON_Duplicate(item->json, 1);
    cJSON_AddItemToObject(message->json, name, obj);
  }
#else
  // TODO
  assert(0);
#endif
  return RT_OK;
}

/**
 * Get field value of type string using field name.
 * @param message to get field
 * @param name of the field
 * @param pointer to string value obtained.
 * @return rtError
 **/
rtError
rtMessage_GetString(rtMessage const  message, const char* name, char const** value)
{
#ifdef WITH_CJSON
  cJSON* p = cJSON_GetObjectItem(message->json, name);
  if (p)
  {
    *value = p->valuestring;
    return RT_OK;
  }
#else
  // TODO
  assert(0);
#endif

  return RT_FAIL;
}

/**
 * Get field value of type string using field name.
 * @param message to get field
 * @param name of the field
 * @param pointer to string value obtained.
 * @param size of value obtained
 * @return rtError
 **/
rtError
rtMessage_GetStringValue(rtMessage const message, char const* name, char* fieldvalue, int n)
{
#ifdef WITH_CJSON
  cJSON* p = cJSON_GetObjectItem(message->json, name);
  if (p)
  {
    char *value = p->valuestring;
    int const len = (int) strlen(value);
    if (len <= n)
    {
      snprintf(fieldvalue, n, "%s", value);
      return RT_OK;
    }
    return RT_FAIL;
  }
#else
  // TODO
  assert(0);
#endif

  return RT_FAIL;
}

/**
 * Get field value of type integer using field name.
 * @param message to get field
 * @param name of the field
 * @param pointer to integer value obtained.
 * @return rtError
 **/
rtError
rtMessage_GetInt32(rtMessage const message,const char* name, int32_t* value)
{  
#ifdef WITH_CJSON
  cJSON* p = cJSON_GetObjectItem(message->json, name);
  if (p)
  {
    *value = p->valueint;
    return RT_OK;
  }
#else
  // TODO
  assert(0);
#endif

  return RT_FAIL;
}

/**
 * Get field value of type double using field name.
 * @param message to get field
 * @param name of the field
 * @param pointer to double value obtained.
 * @return rtError
 **/
rtError
rtMessage_GetDouble(rtMessage const  message, char const* name,double* value)
{
#ifdef WITH_CJSON
  cJSON* p = cJSON_GetObjectItem(message->json, name);
  if (p)
  {
    *value = p->valuedouble;
    return RT_OK;
  }
#else
  // TODO
  assert(0);
#endif

  return RT_FAIL;
}

/**
 * Get field value of type message using name
 * @param message to get field
 * @param name of the field
 * @param message obtained
 * @return rtError
 **/
rtError
rtMessage_GetMessage(rtMessage const message, char const* name, rtMessage* clone)
{
  *clone = (rtMessage) malloc(sizeof(struct _rtMessage));

#ifdef WITH_CJSON
  cJSON* p = cJSON_GetObjectItem(message->json, name);
  if (p)
  {
     (*clone)->json = cJSON_Duplicate(p, cJSON_True);
     (*clone)->count = 1;
     return RT_OK;
  }
#else
  // TODO
  assert(0);
#endif

  return RT_FAIL;
}

/**
 * Get topic of message to be sent
 * @param message to get topic
 * @param name of the topic
 * @return rtError
 **/
rtError
rtMessage_GetSendTopic(rtMessage const m, char* topic)
{
  rtError err = RT_OK;

#ifdef WITH_CJSON
  cJSON* obj = cJSON_GetObjectItem(m->json, "_topic");
  if (obj)
    strcpy(topic, obj->valuestring);
  else
    err = RT_FAIL;
#else
  // TODO
  assert(0);
#endif

  return err;
}

/**
 * Set topic of message to be sent
 * @param message to set topic
 * @param name of the topic
 * @return rtError
 **/
rtError
rtMessage_SetSendTopic(rtMessage const m, char const* topic)
{
#ifdef WITH_CJSON
  cJSON* obj = cJSON_GetObjectItem(m->json, "_topic");
  if (obj)
    cJSON_ReplaceItemInObject(m->json, "_topic", cJSON_CreateString(topic));
  else
    cJSON_AddItemToObject(m->json, "_topic", cJSON_CreateString(topic));
  if (obj)
    cJSON_Delete(obj);
#else
  // TODO
  assert(0);
#endif

  return RT_OK;
}

/**
 * Add string field to array in message
 * @param message to be modified
 * @param name of the field to be added
 * @param value of the field to be added
 * @return rtError
 **/
rtError
rtMessage_AddString(rtMessage m, char const* name, char const* value)
{
#ifdef WITH_CJSON
  cJSON* obj = cJSON_GetObjectItem(m->json, name);
  if (!obj)
  {
    obj = cJSON_CreateArray();
    cJSON_AddItemToObject(m->json, name, obj);
  }
  cJSON_AddItemToArray(obj, cJSON_CreateString(value));
#else
  // TODO
  assert(0);
#endif

  return RT_OK;
}

/**
 * Add message field to array in message
 * @param message to be modified
 * @param name of the field to be added
 * @param message to be added
 * @return rtError
 **/
rtError
rtMessage_AddMessage(rtMessage m, char const* name, rtMessage const item)
{
    if (!m || !item)
    return RT_ERROR_INVALID_ARG;

#ifdef WITH_CJSON
  cJSON* obj = cJSON_GetObjectItem(m->json, name);
  if (!obj)
  {
    obj = cJSON_CreateArray();
    cJSON_AddItemToObject(m->json, name, obj);
  }
  if (item->json)
  {
    cJSON* item_obj = cJSON_Duplicate(item->json, 1);
    cJSON_AddItemToArray(obj, item_obj);
  }
#else
  // TODO
  assert(0);
#endif

  return RT_OK;
}

/**
 * Get length of array from message
 * @param message to get array length from
 * @param name of the array
 * @param fill length of array
 * @return rtError
 **/
rtError
rtMessage_GetArrayLength(rtMessage const m, char const* name, int32_t* length)
{
#ifdef WITH_CJSON
  cJSON* obj = cJSON_GetObjectItem(m->json, name);
  if (!obj)
    *length = 0;
  else
    *length = cJSON_GetArraySize(obj);
#else 
  // TODO
  assert(0);
#endif
  return RT_OK;
}

/**
 * Get string item from array in message
 * @param message to get string item from
 * @param name of the string item
 * @param index of array
 * @param value obtained
 * @param length of string item
 * @return rtError
 **/
rtError
rtMessage_GetStringItem(rtMessage const m, char const* name, int32_t idx, char* value, int len)
{
#ifdef WITH_CJSON
  cJSON* obj = cJSON_GetObjectItem(m->json, name);
  if (!obj)
    return RT_PROPERTY_NOT_FOUND;
  if (idx >= cJSON_GetArraySize(obj))
    return RT_FAIL;

  cJSON* item = cJSON_GetArrayItem(obj, idx);
  if (item)
  {
    strncpy(value, item->valuestring, len);
  }
#else
  // TODO
  assert(0);
#endif
  return RT_OK;
}

/**
 * Get message item from array in parent message
 * @param message to get message item from
 * @param name of message item
 * @param index of array
 * @param message obtained
 * @return rtError
 **/
rtError
rtMessage_GetMessageItem(rtMessage const m, char const* name, int32_t idx, rtMessage* msg)
{
#ifdef WITH_CJSON
  cJSON* obj = cJSON_GetObjectItem(m->json, name);
  if (!obj)
    return RT_PROPERTY_NOT_FOUND;
  if (idx >= cJSON_GetArraySize(obj))
    return RT_FAIL;

  *msg = (rtMessage) malloc(sizeof(struct _rtMessage));
  if (msg)
  {
    (*msg)->json = cJSON_GetArrayItem(obj, idx);
    return RT_OK;
  }
#else
  // TODO
  assert(0);
#endif
  return RT_OK;
}

/**
 * Increase reference count of message by 1
 * @param message
 * @return rtError
 **/
rtError
rtMessage_Retain(rtMessage m)
{
  __atomic_fetch_add(&m->count, 1, __ATOMIC_SEQ_CST);
  return 0;
}

/**
 * Decrease reference count of message by 1 and destroy message if count is 0
 * @param message
 * @return rtError
 **/
rtError
rtMessage_Release(rtMessage m)
{
  __atomic_fetch_sub(&m->count, 1, __ATOMIC_SEQ_CST);
  if (m->count == 0)
    rtMessage_Destroy(m);
  return 0;
}

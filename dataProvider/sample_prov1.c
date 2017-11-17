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
#include "rtConnection.h"
#include "rtLog.h"
#include "rtMessage.h"

#include <stdlib.h>
#include <unistd.h>

void onMessage(rtMessageHeader const* hdr, uint8_t const* buff, uint32_t n, void* closure)
{
  int id = 0;
  rtConnection con = (rtConnection) closure;

  rtMessage req;
  rtMessage_FromBytes(&req, buff, n);
  rtMessage_GetInt32(req, "id", &id);

  if (rtMessageHeader_IsRequest(hdr))
  {
    char* buff = NULL;
    uint32_t buff_length = 0;
    
    rtMessage_ToString(req, &buff, &buff_length);
    rtLog_Info("Req:%.*s", buff_length, buff);
    free(buff);

    // create response
    rtMessage res;
    rtMessage_Create(&res);
    rtMessage_SetInt32(res, "id", id);
    rtMessage_SetString(res, "reply", "reply -- general provider");
    rtConnection_SendResponse(con, hdr, res, 1000);
    rtMessage_Destroy(res);
  }

  rtMessage_Destroy(req);

  rtLog_Info("flags     :%d", hdr->flags);
  rtLog_Info("is_request:%d", rtMessageHeader_IsRequest(hdr));
}

int main()
{
  rtError err;

  rtLog_SetLevel(RT_LOG_DEBUG);

  rtConnection con;
  rtConnection_Create(&con, "general", "tcp://127.0.0.1:10001");
  rtConnection_AddListener(con, "RDK.MODEL.general", onMessage, con);

  while (1)
  {
    err = rtConnection_Dispatch(con);
    rtLog_Info("dispatch:%s", rtStrError(err));
    if (err != RT_OK)
      sleep(1);
  }

  rtConnection_Destroy(con);
  return 0;
}


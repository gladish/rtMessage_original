/* 
 * Copyright [2019] [Comcast, Corp.]
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
#ifndef __RT_ROUTING_TREE_H__
#define __RT_ROUTING_TREE_H__

#include "rtError.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RTMSG_MAX_EXPRESSION_LEN 128

typedef enum
{
  rtBool_TRUE,
  rtBool_FALSE
} rtBool;

typedef struct rtRoutingTree rtRoutingTree;

typedef struct 
{
  void* context;
  char  expression[RTMSG_MAX_EXPRESSION_LEN];
} rtRoutingTreeEntry;

rtRoutingTree* rtRoutingTree_New();
void           rtRoutingTree_Delete(rtRoutingTree* t);

rtError rtRoutingTree_AddRoute(
  rtRoutingTree*  self, 
  char const*     expression,
  void*           context);

rtBool  rtRoutingTree_IsMatch();

rtError rtRoutingTree_RemoveIf(
  rtRoutingTree*  self,
  rtBool          (*matches)(rtRoutingTreeEntry*, void*),
  void*           context);

rtError rtRoutingTree_ForEach(
  rtRoutingTree*  self,
  char const*     topic,
  rtError         (*callback)(rtRoutingTreeEntry*, void*),
  void*           context);

#ifdef __cplusplus
}
#endif

#endif

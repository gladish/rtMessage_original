/* 
 * Copyright [2017] [Comcast, Corp.]
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
#include "rtRoutingTree.h"
#include "rtVector.h"

#include <stdlib.h>
#include <string.h>

#define RTMSG_ROUTING_TABLE 1

#ifdef WITH_LINEAR_ROUTING_TABLE
struct rtRoutingTree
{
  rtVector routes;
};

static void
rtRoutingTree_DeleteEntry(void* p)
{
  rtRoutingTreeEntry* entry = (rtRoutingTreeEntry *) p;
  free(entry);
}

rtRoutingTree*
rtRoutingTree_New()
{
  rtRoutingTree* tree = malloc(sizeof(rtRoutingTree));
  rtVector_Create(&tree->routes);
  return tree;
}

void
rtRoutingTree_Delete(rtRoutingTree* self)
{
  rtVector_Destroy(self->routes, &rtRoutingTree_DeleteEntry);
  free(self);
}

rtBool
rtRoutingTree_IsTopicMatch(char const* topic, char const* exp)
{
  char const* t = topic;
  char const* e = exp;

  while (*t && *e)
  {
    if (*e == '*')
    {
      while (*t && *t != '.')
        t++;
      e++;
    }

    if (*e == '>')
    {
      while (*t)
        t++;
      e++;
    }

    if (!(*t || *e))
      break;

    if (*t != *e)
      break;

    t++;
    e++;
  }

  // rtLogInfo("match[%d]: %s <> %s", !(*t || *e), topic, exp);
  return (rtBool) !(*t || *e);
}

rtError
rtRoutingTree_AddRoute(rtRoutingTree* self, char const* exp, void* context)
{
  rtRoutingTreeEntry* entry = malloc(sizeof(rtRoutingTreeEntry));
  strncpy(entry->expression, exp, RTMSG_MAX_EXPRESSION_LEN);
  entry->context = context;
  rtVector_PushBack(self->routes, entry);
  return RT_OK;
}

rtError
rtRoutingTree_RemoveIf(
  rtRoutingTree*  self,
  rtBool          (*predicate)(rtRoutingTreeEntry*, void*),
  void*           context)
{
  rtVector_Cleanup cleanup = NULL;
  for (size_t i = 0; i < rtVector_Size(self->routes);)
  {
    rtRoutingTreeEntry* entry = (rtRoutingTreeEntry *) rtVector_At(self->routes, i);
    if (predicate(entry, context))
      rtVector_RemoveItemAt(self->routes, i, cleanup);
    else
      i++;
  }
  
  return RT_OK;
}

rtError
rtRoutingTree_ForEach(
  rtRoutingTree*  self,
  char const*     topic,
  rtError         (*callback)(rtRoutingTreeEntry*, void*),
  void*           context)
{
  size_t i;
  size_t n;
  rtRoutingTreeEntry* entry;
  rtError err;

  for (i = 0, n = rtVector_Size(self->routes); i < n; ++i)
  {
    entry = (rtRoutingTreeEntry *) rtVector_At(self->routes, i);
    if (rtRoutingTree_IsTopicMatch(topic, entry->expression))
    {
      if ((err = callback(entry, context)) != RT_OK)
        return err;
    }
  }

  return RT_OK;
}

int
rtRoutingTree_GetSize(
  rtRoutingTree*  self)
{
  return rtVector_Size(self->routes);
}


#else
#endif

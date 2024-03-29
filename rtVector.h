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
#ifndef __RT_VECTOR_H__
#define __RT_VECTOR_H__

#include "rtError.h"
#include <stdio.h>

struct _rtVector;
typedef struct _rtVector* rtVector;

typedef void (*rtVector_Cleanup)(void *);

rtError rtVector_Create(rtVector* v);
rtError rtVector_Destroy(rtVector v, rtVector_Cleanup destroyer);
rtError rtVector_PushBack(rtVector v, void* item);
rtError rtVector_RemoveItem(rtVector v, void* item, rtVector_Cleanup destroyer);
rtError rtVector_RemoveItemAt(rtVector v, int n, rtVector_Cleanup destroyer);
void*   rtVector_At(rtVector v, size_t index);
size_t  rtVector_Size(rtVector v);

#endif

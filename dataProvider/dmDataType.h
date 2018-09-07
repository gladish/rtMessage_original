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
#ifndef __DM_DATA_TYPE_H__
#define __DM_DATA_TYPE_H__

enum class dmDataType
{
  UInt8,
  SInt8,
  UInt16,
  SInt16,
  UInt32,
  SInt32,
  UInt64,
  SInt64,
  String,
  Boolean,
  Real32,
  Real64,
  DateTime,
  Reference,
};

#endif

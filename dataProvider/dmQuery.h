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
#ifndef __DM_QUERY_H__
#define __DM_QUERY_H__

#include "dmValue.h"
#include "dmProviderOperation.h"
#include "dmQueryResult.h"

class dmQuery
{
public:
  virtual ~dmQuery() { }
  virtual bool exec() = 0;
  virtual void reset() = 0;
  virtual void setQueryString(dmProviderOperation op, char const* s) = 0;
  virtual void setProviderName(std::string provider) = 0;
  virtual dmQueryResult const& results() = 0;
};

#endif

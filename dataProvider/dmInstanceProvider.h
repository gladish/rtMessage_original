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
#ifndef __DM_INSTANCE_PROVIDER_H__
#define __DM_INSTANCE_PROVIDER_H__

#include "dmObjectPath.h"
#include "dmInstance.h"

class dmInstanceProvider
{
public:
  using pointer = std::shared_ptr<dmInstanceProvider>;

  virtual dmObjectPath::list enumerateInstanceNames(dmObjectPath const& path) = 0;

  virtual dmInstance::pointer getInstance(dmObjectPath const& path,
    std::vector<std::string> const& propertyList = std::vector<std::string>()) = 0;
};

#endif

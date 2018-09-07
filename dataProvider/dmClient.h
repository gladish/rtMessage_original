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
#ifndef __DM_CLIENT_H__
#define __DM_CLIENT_H__

#include "dmInstance.h"
#include "dmQuery.h"
#include "dmOperation.h"


class dmClient
{
public:
  dmClient();

  void close();

  dmInstance::pointer getInstance(
    dmObjectPath const& name,
    std::vector<std::string> const& propertyList = std::vector<std::string>());

  std::vector<dmObjectPath> enumerateInstanceNames(
    dmObjectPath const& path);

  dmInstance::pointer_list execQuery(dmOperation op, std::string const& query);

private:
  dmInstance::pointer_list execQueryGetInstance(dmQuery const& q);
};


#endif

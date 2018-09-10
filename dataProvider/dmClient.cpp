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
#include "dmClient.h"
#include "dmModelDatabase.h"
#include "dmQuery.h"

#include <rtLog.h>
#include <assert.h>

dmClient::dmClient()
{
}

void
dmClient::close()
{
}

dmInstance::pointer
dmClient::getInstance(dmObjectPath const& name, std::vector<std::string> const& propertyList)
{
  return dmInstance::pointer();
}

std::vector<dmObjectPath>
dmClient::enumerateInstanceNames(dmObjectPath const& path)
{
  return std::vector<dmObjectPath>();
}

dmInstance::pointer_list
dmClient::execQueryGetInstance(dmQuery const& q)
{
  dmInstanceProvider::pointer provider = dmModelDatabase::instance()
    .get_provider(q.object_path().objectName(), true);

  if (!provider)
  {
    rtLog_Warn("failed to get provider for object path:%s", q.object_path().toString().c_str());
    return dmInstance::pointer_list();
  }

  dmInstance::pointer_list list;
  dmInstance::pointer instance = provider->getInstance(q.object_path(), q.property_list());
  list.push_back(instance);

  return list;
}

dmInstance::pointer_list
dmClient::execQuery(dmOperation op, std::string const& query)
{
  rtLog_Debug("execQuery:%s", query.c_str());

  dmInstance::pointer_list instances;
  dmQuery::pointer_list queries = dmQuery::compile(query);

  for (dmQuery::pointer const& q : queries)
  {
    if (op == dmOperation::GetInstance)
    {
      dmInstance::pointer_list temp = execQueryGetInstance(*q);
      instances.insert(instances.end(), temp.begin(), temp.end());
    }
  }

  return instances;
}

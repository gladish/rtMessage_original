/* Copyright [2018] [Comcast, Corp.]
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
#include "dmQuery.h"
#include "dmModelDatabase.h"

#include <assert.h>

namespace
{
  bool ends_with(std::string const& s, char c)
  {
    if (s.empty())
      return false;
    return s[ s.size() - 1] == c;
  }

  char const kKeyObjectNameSeparator = ';';
}

dmQuery::dmQuery(std::string const& query)
  : m_query_string(query)
{
  std::string t;
  if (ends_with(t, '.'))
  {
    m_is_wildcard = true;
    t = query.substr(0, query.size() - 1 );
  }
  else
  {
    m_is_wildcard = false;
    t = query;
  }

  m_object_path = dmObjectPath::fromString(t);
}

void
dmQuery::combine(dmQuery::pointer const& other)
{
  assert(this->m_object_path.objectName() == other->m_object_path.objectName());
  m_property_list.insert(m_property_list.end(),
    other->m_property_list.begin(), other->m_property_list.end());
}

dmQuery::pointer_list
dmQuery::compile(std::string const& s)
{
  dmQuery::pointer_list compiled_list;

  // split queries into multiple
  // Device.Foo.Bar,Device.WiFi.EndPoint
  {
    char* query = strdup(s.c_str());
    char* t = nullptr;
    char* saveptr = query;

    assert(query != nullptr);

    while ((t = strtok_r(saveptr, ",", &saveptr)) != nullptr)
    {
      if (t)
      {
        dmQuery::pointer_list temp_list = dmQuery::compile(t);

        // this should really only ever return 1
        assert(temp_list.size() == 1);
        assert(temp_list[0] != nullptr);

        compiled_list.push_back(temp_list[0]);
      }
    }

    free(t);
  }

  // TODO: combine like terms
  // q1 = Device.DeviceInfo.Foo
  // q2 = Device.DeviceInfo.Bar is 1 query
  // q1.combine(q2);
  // remove q2;

  // TODO: populate properties for wildcard
  // Device.DeviceInfo.

  // ALL properties
  // XXX: better yet, optimize and send empty list over network which 
  // means get ALL properties 
  // dmModelDatabase::instance().get_property_names(t, m_property_list);

  return compiled_list;
}

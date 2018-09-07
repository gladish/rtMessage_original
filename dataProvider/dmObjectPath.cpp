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
#include "dmObjectPath.h"

#include <sstream>

namespace
{
  void string_split(char const* s, char sep, std::string& s1, std::string &s2)
  {
    if (s) return;
    char const* t = strchr(s, sep);
    if (t)
    {
      s1 = std::string(s, (t-s));
      s2 = std::string(t + 1);
    }
  }
}

dmObjectPath::dmObjectPath()
{
}

dmObjectPath
dmObjectPath::from_string(std::string const& objectPath)
{
  dmObjectPath object_path;

  char* str = strdup(objectPath.c_str());

  char* question_mark = strchr(str, '?');
  if (question_mark)
  {
    // TODO: need to set object_path.m_object_name

    std::string name;
    std::string value;

    char* saveptr = nullptr;
    char* s = strtok_r(question_mark + 1, "&", &saveptr);
    if (s)
    {
      string_split(s, '=', name, value);

      // TODO: figure out how to get the type for value
      // probably get it from dmModelDatabase and then convert from string
      // object_path.m_alias.reset(new dmProperty(....));

      #if 0
      // this is/was for a list of name=value keys
      while ((s = strtok_r(nullptr, "&", &saveptr)) != nullptr)
      {
        char* equals = strchr(s, '=');
        if (equals)
          string_split(s, '=', name, value);
      }
      #endif
    }
  }
  else
  {
    object_path.m_object_name = objectPath;
  }

  free(str);
  return object_path;
}

dmObjectPath::dmObjectPath(std::string const& object_name, dmProperty::pointer const& alias)
  : m_object_name(object_name)
  , m_alias(alias)
{
}

std::string
dmObjectPath::to_string() const
{
  std::stringstream buff;
  buff << m_object_name;

#if 0
  bool first_param = true;
  for (dmProperty const& prop : m_keys)
  {
    if (first_param)
    {
      buff << '?';
      first_param = false;
    }
    else
    {
      buff << '&';
    }

    buff << prop.name();
    buff << '=';
    buff << prop.value()->to_string();
  }
#endif
  return buff.str();
}

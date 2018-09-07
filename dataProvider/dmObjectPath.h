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
#ifndef __DM_OBJECT_PATH__
#define __DM_OBJECT_PATH__

#include "dmProperty.h"

#include <string>
#include <vector>


class dmObjectPath
{
public:
  using list = std::vector<dmObjectPath>;

  dmObjectPath();
  dmObjectPath(std::string const& object_name, dmProperty::pointer const& alias);

  inline void set_object_name(std::string const& object_name)
    { m_object_name = object_name; }

  inline void set_alias(dmProperty::pointer const& alias)
    { m_alias = alias; }

  inline std::string const& object_name() const
    { return m_object_name; }

  inline dmProperty::pointer key() const
    { return m_alias; }

  std::string to_string() const;

  static dmObjectPath from_string(std::string const& s);

private:
  std::string m_object_name;
  dmProperty::pointer m_alias;
};

#endif

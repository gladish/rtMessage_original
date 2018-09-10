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
  dmObjectPath(std::string const& object_name, dmProperty::list const& keys
     = dmProperty::list());

  inline void setObjectName(std::string const& object_name)
    { m_object_name = object_name; }

  inline void addKey(dmProperty const& k)
    { m_keys.push_back(k); }

  inline std::string const& objectName() const
    { return m_object_name; }

  inline dmProperty::list const& keys() const
    { return m_keys; }

  std::string toString() const;

  static dmObjectPath fromString(std::string const& s);

private:
  std::string m_object_name;
  dmProperty::list m_keys;
};

#endif

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
#ifndef __DM_PROPERTY_H__
#define __DM_PROPERTY_H__

#include "dmValue.h"

#include <string>
#include <vector>

class dmProperty
{
public:
  using list = std::vector<dmProperty>;
  using pointer = std::shared_ptr<dmProperty>;

  dmProperty() { }
  dmProperty(std::string const& name, dmValue::pointer const& value, bool isKey);

  inline bool is_key() const
    { return m_is_key; }

  inline std::string const& name() const
    { return m_name; }

  inline dmValue::pointer value() const
    { return m_value; }

private:
  std::string m_name;
  dmValue::pointer m_value;
  bool m_is_key;
};

#endif

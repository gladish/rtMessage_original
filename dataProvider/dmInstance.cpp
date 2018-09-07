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
#include "dmInstance.h"

dmInstance::dmInstance(dmObjectPath const& objectPath, dmProperty::list const& props)
  : m_object_path(objectPath)
  , m_properties(props)
{
}

dmProperty
dmInstance::property(std::string const& name) const
{
  for (dmProperty const& prop : m_properties)
  {
    if (prop.name() == name)
      return prop;
  }
  return dmProperty();
}


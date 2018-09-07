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
#ifndef __DM_INSTANCE_H__
#define __DM_INSTANCE_H__

#include <vector>
#include <memory>

#include "dmInstance.h"
#include "dmProperty.h"
#include "dmObjectPath.h"

class dmInstance
{
public:
  using pointer = std::shared_ptr<dmInstance>;
  using pointer_list = std::vector<pointer>;

  dmInstance(dmObjectPath const& objectPath, dmProperty::list const& props);

  inline dmProperty::list const& properties() const
    { return m_properties; }

  inline dmObjectPath const& object_path() const
    { return m_object_path; }

  dmProperty property(std::string const& name) const;

private:
  dmObjectPath m_object_path;
  dmProperty::list m_properties;
};

#endif

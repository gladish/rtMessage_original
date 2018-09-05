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
#ifndef __DM_PROVIDER_INFO_H__
#define __DM_PROVIDER_INFO_H__

#include "dmPropertyInfo.h"
#include "rtLog.h"

#include <string>
#include <vector>
#include <memory>

class dmProviderDatabase;
class dmPropertyInfo;

class dmProviderInfo
{
  friend class dmProviderDatabase;

public:
  inline std::string const& objectName() const
    { return m_objectName; }

  inline std::string const& providerName() const
    { return m_providerName; }

  inline std::vector<dmPropertyInfo> const& properties() const
    { return m_props; }

  dmPropertyInfo getPropertyInfo(char const* s) const;

  bool isList() const 
    { return m_isList; }

  void setIsList(bool b)
    { m_isList = b; }

  std::shared_ptr<dmProviderInfo> getParent()
  {
    return m_parent;
  }

  void setParent(std::shared_ptr<dmProviderInfo> parent)
  {
    m_parent = parent;
  }

  std::vector< std::shared_ptr<dmProviderInfo> > const& getChildren()
  {
    return m_children;
  }

  void addChild(std::shared_ptr<dmProviderInfo> child)
  {
    rtLog_Debug("addChild(%p) %s to parent %s", this, child->objectName().c_str(), objectName().c_str());
    m_children.push_back(child);
    child->setParent(std::shared_ptr<dmProviderInfo>(this));
  }  

private:
  dmProviderInfo();
  void setProviderName(std::string const& name);
  void setObjectName(std::string const& name);
  void addProperty(dmPropertyInfo const& propInfo);

private:
  std::string m_objectName;
  std::string m_providerName;
  std::vector<dmPropertyInfo> m_props;
  bool m_isList;
  std::shared_ptr<dmProviderInfo> m_parent;
  std::vector< std::shared_ptr<dmProviderInfo> > m_children;
};
#endif

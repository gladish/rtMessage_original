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

#include <string>
#include <vector>

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

private:
  dmProviderInfo();
  void setProviderName(std::string const& name);
  void setObjectName(std::string const& name);
  void addProperty(dmPropertyInfo const& propInfo);

private:
  std::string m_objectName;
  std::string m_providerName;
  std::vector<dmPropertyInfo> m_props;
};
#endif

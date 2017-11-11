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
#ifndef __DM_PROVIDER_HOST_H__
#define __DM_PROVIDER_HOST_H__

#include "dmPropertyInfo.h"
#include "dmQueryResult.h"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class dmProvider;

class dmProviderHost
{
public:
  dmProviderHost() { }
  virtual ~dmProviderHost() { }
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void run() = 0;

public:
  static dmProviderHost* create();

public:
  bool registerProvider(std::string const& name, std::unique_ptr<dmProvider> provider);

protected:
  virtual bool providerRegistered(std::string const& name) = 0;

  void doGet(std::string const& providerName, std::vector<dmPropertyInfo> const& params,
    dmQueryResult& result);

  void doSet(std::string const& providerName, std::vector<dmNamedValue> const& params,
    dmQueryResult& result);

private:
  std::map< std::string, std::unique_ptr<dmProvider> > m_providers;
};

#endif

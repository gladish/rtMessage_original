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
#include "dmModelDatabase.h"
#include "dmInstanceProvider.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <rtLog.h>
#include <cJSON.h>

dmModelDatabase::node::~node()
{
  if (json)
    cJSON_Delete(json);
}

dmModelDatabase::node::pointer
dmModelDatabase::node::child(char const* s, cJSON* json, bool create)
{
  node::pointer n;
  for (node::pointer const& d : descendents)
  {
    if (d->name == s)
      return d;
  }

  if (create)
  {
    rtLog_Debug("node '%s' doesn't exist, inserting", s);
    pointer new_node(new node());
    new_node->name = s;
    new_node->is_object = false;
    new_node->json = json;
    descendents.push_back(new_node);
    n = new_node;
  }

  return n;
};

// root node is always empty
// static dmModelDatabase::node::pointer model_root;
static dmModelDatabase* model_database = nullptr;

void
dmModelDatabase::initialize(std::string const& rootDirectory)
{
  if (model_database)
    delete model_database;
  model_database = new dmModelDatabase(rootDirectory);
}

dmModelDatabase&
dmModelDatabase::instance()
{
  return *model_database;
}

class dmInstanceProviderProxy : public dmInstanceProvider
{
public:
  dmInstanceProviderProxy(dmModelDatabase::node::pointer const& db_node)
    : m_db_node(db_node)
  {
  }

  virtual dmObjectPath::list enumerateInstanceNames(dmObjectPath const& path) override
  {
    // TODO: send rtRemote call to enumerate instance names
    dmObjectPath::list names;
    return std::move(names);
  }

  virtual dmInstance::pointer getInstance(dmObjectPath const& path, std::vector<std::string> const& propertyList) override
  {
    // TODO: send rtRemote call to get instance
    // DO THIS
    return dmInstance::pointer();
  }

private:
  std::string object_name() const
  {
    cJSON* p = cJSON_GetObjectItem(m_db_node->json, "name");
    return std::string(p->valuestring);
  }

  bool is_list() const
  {
    cJSON* p = cJSON_GetObjectItem(m_db_node->json, "is_list");
    return p != nullptr 
      ? (p->type == cJSON_True) 
      : false;
  }

private:
  dmModelDatabase::node::pointer m_db_node;
};

dmModelDatabase::dmModelDatabase(std::string const& rootDirectory)
{
  loadFromDir(rootDirectory);
}

void
dmModelDatabase::loadFromDir(std::string const& dirname)
{
  DIR* dir;
  struct dirent *ent;

  rtLog_Debug("loading database from directory:%s", dirname.c_str());
  if ((dir = opendir(dirname.c_str())) != NULL)
  {
    while ((ent = readdir(dir)) != NULL)
    {
      if (strcmp(ent->d_name,".") != 0 && strcmp(ent->d_name,"..") != 0)
        loadFromFile(dirname.c_str(), ent->d_name);
    }
    closedir(dir);
  } 
  else
  {
    rtLog_Warn("failed to open directory:%s. %s", dirname.c_str(),
      strerror(errno));
  }
}

void
dmModelDatabase::loadFromFile(std::string const& dirname, char const* fname)
{
  std::string path = dirname;
  path += "/";
  path += fname;

  rtLog_Debug("load file:%s", path.c_str());

  std::ifstream file;
  file.open(path.c_str(), std::ifstream::in);
  if (!file.is_open())
  {
    rtLog_Warn("failed to open fie. %s. %s", fname, strerror(errno));
    return;
  }

  file.seekg(0, file.end);
  int length = file.tellg();
  file.seekg(0, file.beg);

  std::vector<char> buff(length + 1, '\0');
  file.read(buff.data(), length);

  insertIntoTree(buff);
}

void
dmModelDatabase::insertIntoTree(std::vector<char> const& buff)
{
  cJSON* json = cJSON_Parse(&buff[0]);
  if (!json)
  {
    rtLog_Error("failed to parse json:%s", cJSON_GetErrorPtr());
  }
  else
  {
    cJSON* obj = nullptr;

    if ((obj = cJSON_GetObjectItem(json, "name")) == nullptr)
    {
      // TODO: ERROR: no 'name'
    }

    char* saveptr = obj->valuestring;
    char* token = nullptr;

    rtLog_Debug("object name:%s", obj->valuestring);

    if (!m_model_root)
      m_model_root.reset(new dmModelDatabase::node());

    node::pointer n = m_model_root;
    while ((token = strtok_r(saveptr, ".", &saveptr)) != nullptr)
      n = n->child(token, obj, true);
    n->json = obj;
    n->is_object = true;
  }
}

std::shared_ptr<dmInstanceProvider>
dmModelDatabase::get_provider(std::string const& object_name, bool /*proxy*/)
{
  std::shared_ptr<dmInstanceProvider> provider;

  dmModelDatabase::node::pointer n = find(object_name.c_str());

  // if node is present but is not an object, then the dmObjectPath is not valid
  // someone is trying to get a provider for something like Device.
  // This needs to be parsed as a query.
  if (n && n->is_object)
    provider.reset(new dmInstanceProviderProxy(n));

  return provider;
}

dmModelDatabase::node::pointer
dmModelDatabase::find(char const* s)
{
  if (!s) return dmModelDatabase::node::pointer();

  char* temp = strdup(s);
  char* saveptr = temp;
  char* token = nullptr;

  dmModelDatabase::node::pointer n = m_model_root;
  while ((token = strtok_r(saveptr, ".", &saveptr)) != nullptr)
  {
    n = n->child(token, nullptr, false);
    if (!n)
      break;
  }

  free(temp);

  return n;
}

void
dmModelDatabase::get_property_names(std::string const& object_name, std::vector<std::string>& properties)
{
  node::pointer n = find(object_name.c_str());
  if (n)
  {
    cJSON* props = cJSON_GetObjectItem(n->json, "properties");
    if (props)
    {
      for (int i = 0, n = cJSON_GetArraySize(props); i < n; ++i)
      {
        cJSON* prop = cJSON_GetArrayItem(props, i);
        cJSON* name = cJSON_GetObjectItem(prop, "name");
        properties.push_back(name->valuestring);
      }
    }
  }
}

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
#include "dmObjectPath.h"
#include "dmInstanceProvider.h"

#include <string>
#include <memory>
#include <vector>

struct cJSON;

class dmModelDatabase
{
public:
  static void initialize(std::string const& rootDirectory);
  static dmModelDatabase& instance();


  dmInstanceProvider::pointer get_provider(std::string const& object_name, bool proxy);

  void get_property_names(std::string const& object_name, std::vector<std::string>& properites);

  // TODO: should be private, which means that dmInstanceProviderProxy should
  // also be defined as a private class inside dmModelDatabase
  struct node
  {
    using pointer = std::shared_ptr<node>;

    std::string             name;
    bool                    is_object;
    std::vector<pointer>    descendents;
    cJSON*                  json;

    ~node();
    pointer child(char const* s, cJSON* json, bool create);
  };

private:
  dmModelDatabase(std::string const& rootDirectory);
  void loadFromDir(std::string const& dirname);
  void loadFromFile(std::string const& dirname, char const* fname);
  void insertIntoTree(std::vector<char> const& buff);

  node::pointer find(char const* s);

private:
  std::string m_root_directory;
  node::pointer m_model_root;
};

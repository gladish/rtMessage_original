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
#ifndef __DM_QUERY_H__
#define __DM_QUERY_H__

#include "dmObjectPath.h"

#include <string>

class dmQuery
{
public:
  using pointer = std::shared_ptr<dmQuery>;
  using pointer_list = std::vector<pointer>;

  static pointer_list compile(std::string const& s);

  inline dmObjectPath const& object_path() const
    { return m_object_path; }

  inline bool is_wildcard() const
    { return m_is_wildcard; }

  inline std::vector<std::string> const& property_list() const
    { return m_property_list; }

private:
  dmQuery(std::string const& s);
  void combine(pointer const& other);

private:
  bool m_is_wildcard;
  std::string m_query_string;
  dmObjectPath m_object_path;
  std::vector<std::string> m_property_list;
};

#endif

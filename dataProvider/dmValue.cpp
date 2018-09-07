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
#include "dmValue.h"

dmValue::~dmValue()
{
}

time_t
dmDateTime::timeFromString(std::string const& s)
{
  // TODO
  return 0;
}

std::string
dmDateTime::to_string() const
{
  char buff[256];
  memset(buff, 0, sizeof(buff));

  // http://wbemservices.sourceforge.net/javadoc/api/javax/cim/CIMDateTime.html
  // for format
  struct tm* t = localtime(&m_value);
  size_t n = strftime(buff, sizeof(buff), "YYYY", t);

  return std::string(buff);
}

std::string
dmObjectReference::to_string() const
{
  return m_value;
}

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

#ifndef __DM_VALUE_H__
#define __DM_VALUE_H__

#include "dmDataType.h"

#include <memory>
#include <sstream>
#include <vector>
#include <string>

#include <stdint.h>
#include <string.h>

class dmValue
{
public:
  using pointer = std::shared_ptr<dmValue>;
  using pointer_list = std::vector<pointer>;

  dmValue(dmDataType t) : m_type(t) { }
  virtual ~dmValue();
  virtual std::string to_string() const = 0;
  inline dmDataType type() const
    { return m_type; }
private:
  dmDataType m_type;
};

template<class T> class dmValueGeneric: public dmValue
{
public:
  dmValueGeneric(dmDataType type, T value) 
    : dmValue(type)
    , m_value(value) { }

  virtual ~dmValueGeneric() { }
  virtual std::string to_string() const override
  {
    std::stringstream buff;
    buff << m_value;
    return buff.str();
  }

protected:
  T m_value;
};

struct dmObjectReference : public dmValueGeneric<std::string>
{
  dmObjectReference(std::string const& s) : dmValueGeneric<std::string>(dmDataType::Reference, s)
    { }
  virtual std::string to_string() const override;
};

struct dmDateTime : public dmValueGeneric<time_t>
{
  dmDateTime(time_t t) : dmValueGeneric<time_t>(dmDataType::DateTime, t) { }
  dmDateTime(std::string const& s) 
    : dmValueGeneric<time_t>(dmDataType::DateTime, dmDateTime::timeFromString(s)) { }
  virtual std::string to_string() const override;
  static time_t timeFromString(std::string const& s);
};

struct dmValueUInt8 : public dmValueGeneric<uint8_t>
{
  dmValueUInt8(uint8_t n) : dmValueGeneric<uint8_t>(dmDataType::UInt8, n) { }
};

struct dmValueSInt8 : public dmValueGeneric<int8_t>
{
  dmValueSInt8(int8_t n) : dmValueGeneric<int8_t>(dmDataType::SInt8, n) { }
};

struct dmValueUInt16 : public dmValueGeneric<uint16_t>
{
  dmValueUInt16(uint16_t n) : dmValueGeneric<uint16_t>(dmDataType::UInt16, n) { }
};

struct dmValueSInt16 : public dmValueGeneric<int16_t>
{
  dmValueSInt16(int16_t n) : dmValueGeneric<int16_t>(dmDataType::SInt16, n) { }
};

struct dmValueUInt32 : public dmValueGeneric<uint32_t>
{
  dmValueUInt32(uint32_t n) : dmValueGeneric<uint32_t>(dmDataType::UInt32, n) { }
};

struct dmValueSInt32 : public dmValueGeneric<int32_t>
{
  dmValueSInt32(int32_t n) : dmValueGeneric<int32_t>(dmDataType::SInt32, n) { }
};

struct dmValueUInt64 : public dmValueGeneric<uint64_t>
{
  dmValueUInt64(uint64_t n) : dmValueGeneric<uint64_t>(dmDataType::UInt64, n) { }
};

struct dmValueSInt64 : public dmValueGeneric<int64_t>
{
  dmValueSInt64(int64_t n) : dmValueGeneric<int64_t>(dmDataType::SInt64, n) { }
};

#endif

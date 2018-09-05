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
#include "dmProviderHost.h"
#include "dmProvider.h"
#include <rtLog.h>
#include <rtError.h>
#include <unistd.h>

class AdapterObject : public dmProvider
{
public:
  AdapterObject(const char* name)
  {
    mname = name;
    onGet("Name", [this]() -> dmValue { return mname.c_str(); });
    onSet("Name", [this](dmValue const& value) -> void { mname = value.toString();});
  }
  std::string mname;
};

class InterfaceObject : public dmProvider
{
public:
  InterfaceObject(const char* name)
  {
    mname = name;
    onGet("Name", [this]() -> dmValue { return mname.c_str(); });
    onSet("Name", [this](dmValue const& value) -> void { mname = value.toString();});
  }
  std::string mname;
};

class RouterObject : public dmProvider
{
public:
  RouterObject()
  {
    mname = "Router";
    onGet("Name", [this]() -> dmValue { return mname.c_str(); });
    onSet("Name", [this](dmValue const& value) -> void { mname = value.toString();});
  }
  std::string mname;
};

class EndPointObject : public dmProvider
{
public:
  EndPointObject(const char* name, const char* security, const char* channel, const char* mode, const char* status) 
    : mssid(name), msecurityType(security), mchannel(channel), mmode(mode), mstatus(status), dmProvider(name)
  {
    onGet("Status", [this]() -> dmValue { return mstatus.c_str(); });
    onSet("Status", [this](dmValue const& value) -> void { mstatus = value.toString();});
    onGet("SSID", [this]() -> dmValue { return mssid.c_str(); });
    onSet("SSID", [this](dmValue const& value) -> void { mssid = value.toString();});
    onGet("AdapterNumberOfEntries", [this]() -> dmValue { return 1; });
    onGet("AdapterIdsOfEntries", [this]() -> dmValue { return mssid=="NETGEAR1" ? "a1" : (mssid=="RAWHIDE123" ? "a2" : "a3"); });
    onGet("InterfaceNumberOfEntries", [this]() -> dmValue { return 1; });
    onGet("InterfaceIdsOfEntries", [this]() -> dmValue { return mssid=="NETGEAR1" ? "i1" : (mssid=="RAWHIDE123" ? "i2" : "i3"); });
  }
private:
  std::string mssid;
  std::string msecurityType;
  std::string mchannel;
  std::string mmode;
  std::string mstatus;
};

class WiFiObject : public dmProvider
{
public:
  WiFiObject() : m_noiseLevel("10dB"), m_userName("xcam_user")
  {
    onGet("EndPointNumberOfEntries", [this]() -> dmValue { return 3; });
    onGet("EndPointIdsOfEntries", [this]() -> dmValue { return "NETGEAR1,RAWHIDE123,XB3"; });
    onGet("X_RDKCENTRAL-COM_NoiseLevel", [this]() -> dmValue { return "100db"; });
  }

private:
protected:
private:

  std::string m_noiseLevel;
  std::string m_userName;
};

int main(int argc, char **argv)
{
  //dmUtility::QueryParser::test(argv[1]);return 0;  
  
  dmProviderHost* host = dmProviderHost::create();
  host->start();

  host->registerProvider("Device.WiFi", std::unique_ptr<WiFiObject>(new WiFiObject()));
  host->registerProvider("Device.WiFi.EndPoint.NETGEAR1", std::unique_ptr<EndPointObject>(new EndPointObject("NETGEAR1","WPA","0","12","Online")));
  host->registerProvider("Device.WiFi.EndPoint.RAWHIDE123", std::unique_ptr<EndPointObject>(new EndPointObject("RAWHIDE123","WPA+PKI","1","3","Online")));
  host->registerProvider("Device.WiFi.EndPoint.XB3", std::unique_ptr<EndPointObject>(new EndPointObject("XB3","Comcast","0","11","Offline")));
  host->registerProvider("Device.WiFi.EndPoint.NETGEAR1.Adapter.a1", std::unique_ptr<AdapterObject>(new AdapterObject("a1")));
  host->registerProvider("Device.WiFi.EndPoint.RAWHIDE123.Adapter.a2", std::unique_ptr<AdapterObject>(new AdapterObject("a2")));
  host->registerProvider("Device.WiFi.EndPoint.XB3.Adapter.a3", std::unique_ptr<AdapterObject>(new AdapterObject("a3")));
  host->registerProvider("Device.WiFi.EndPoint.NETGEAR1.Interface.i1", std::unique_ptr<InterfaceObject>(new InterfaceObject("i1")));
  host->registerProvider("Device.WiFi.EndPoint.RAWHIDE123.Interface.i2", std::unique_ptr<InterfaceObject>(new InterfaceObject("i2")));
  host->registerProvider("Device.WiFi.EndPoint.XB3.Interface.i3", std::unique_ptr<InterfaceObject>(new InterfaceObject("i3")));
  host->registerProvider("Device.WiFi.Router", std::unique_ptr<RouterObject>(new RouterObject()));
  while (true)
  {
    sleep(1);
  }

  host->stop();
  return 0;
}

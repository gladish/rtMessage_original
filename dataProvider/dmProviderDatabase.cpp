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
#include "dmProviderDatabase.h"
#include "dmQuery.h"

#include <iostream>
#include <dirent.h>
#include <cstddef>
#include <string>
#include <cstring>
#include <map>
#include <iterator>
#include <fstream>
#include <iomanip>


extern "C"
{
#include "rtConnection.h"
#include "rtMessage.h"
#include "rtError.h"
#include "rtLog.h"
#include <cJSON.h>
}

void splitQuery(char const* query, char* model, char* parameter);
//void doGet(std::string const& providerName, std::vector<dmPropertyInfo> &propertyName);

void splitQuery(char const* query, char* model, char* parameter)
{
  std::string str(query);
  std::size_t position = str.find_last_of(".\\");
  model[0]= '\0';
  std::strcat(model , str.substr(0, position).c_str());
  parameter[0]= '\0';
  std::strcat(parameter, str.substr(position+1).c_str());
  str.clear();
}

// TODO: Implement me
class dmQueryImpl : public dmQuery
{
public:
  virtual bool exec()
  {
    if(m_provider.empty())
      return false;

    std::string topic("RDK.MODEL.");
    topic += m_provider;

    rtLog_Info("\nTOPIC TO SEND : %s", topic.c_str());

    rtLog_SetLevel(RT_LOG_DEBUG);

    rtConnection con;
    rtConnection_Create(&con, "DMCLIENT", "tcp://127.0.0.1:10001");

     /* TODO send request to provider using rtmessage" */
    rtMessage res;
    rtMessage req;
    rtMessage_Create(&req);
    rtMessage_SetString(req, "operation", m_operation.c_str());
    rtMessage_SetString(req, "propertyName", m_query.c_str());
    if (strcmp(m_operation.c_str(), "set") == 0)
      rtMessage_SetString(req, "value", m_value.c_str());

    rtError err = rtConnection_SendRequest(con, req, topic.c_str(), &res, 2000);
    rtLog_Info("SendRequest : %s", rtStrError(err));

    if (err == RT_OK)
    {
      char* p = NULL;
      uint32_t len = 0;

      rtMessage_ToString(res, &p, &len);
      rtLog_Info("\tResponse : %.*s\n", len, p);
      free(p);
    }

    rtMessage_Destroy(req);
    rtMessage_Destroy(res);

    rtConnection_Destroy(con);
    return true;
  }

  virtual void reset()
  {
    m_query.clear();
    m_provider.clear();
    m_value.clear();
  }

  void setProviderName(std::string provider)
  {
    m_provider = provider;
  }

  virtual bool setQueryString(dmProviderOperation op, char const* s)
  {
    switch (op)
    {
      case dmProviderOperation_Get:
      {
        m_operation = "get";
        m_query = s;
      }
      break;
      case dmProviderOperation_Set:
      {
        m_operation = "set";
        std::string data(s);
        if (data.find("=") != std::string::npos)
        {
          std::size_t position = data.find("=");
          m_query = data.substr(0, position);
          m_value = data.substr(position+1);
        }
        else
        {
          printf("\nSet operation expects value to be set\n");
          return false;
        }
        break;
      }
    }
    return true;
  }

  virtual dmQueryResult const& results()
  {
    return m_results;
  }

private:
  dmQueryResult m_results;
  std::string m_query;
  std::string m_provider;
  std::string m_value;
  std::string m_operation;
};

dmProviderDatabase::dmProviderDatabase(std::string const& dir)
  : m_dataModelDir(dir)
{
  loadFromDir(dir);
}

void
dmProviderDatabase::loadFromDir(std::string const& dir)
{
  DIR* directory;
  struct dirent *ent;

  if ((directory = opendir(m_dataModelDir.c_str())) != NULL) 
  {
     /* Print all the files and directories within directory */
    while ((ent = readdir(directory)) != NULL) 
    {
      if (strcmp(ent->d_name,".")!=0 && strcmp(ent->d_name,"..")!=0 )
        loadFile(dir.c_str(), ent->d_name);
    }
    closedir(directory);
  } 
  else
  {
     printf("\n Could not open directory");
  }
}

void
dmProviderDatabase::loadFile(char const* dir, char const* fname)
{
  cJSON* json = NULL;
  char *file = new char[80];
  
  std::strcat(file, dir);
  std::strcat(file, fname);

  std::string root(fname);
  root.erase(root.length()-5);

  printf("\nRoot %s", root.c_str());

  printf("\nFile %s", file);

  std::ifstream ifile;
  ifile.open(file, std::ifstream::in);

  if (ifile.is_open())
  {
    ifile.seekg(0, ifile.end);
    int length = ifile.tellg();
    printf("\n Length %d \n", length);
    ifile.seekg(0, ifile.beg);

    char *buffer = new char[length];
    ifile.read(buffer,length);

    json = cJSON_Parse(buffer);
    if (!json)
    {
      printf("\nError before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
      dmProviderInfo providerInfo;
      providerInfo.setProviderName(cJSON_GetObjectItem(json, "provider")->valuestring);
      providerInfo.setObjectName(cJSON_GetObjectItem(json, "name")->valuestring);

      cJSON *props = cJSON_GetObjectItem(json, "properties"); 
      for (int i = 0, n = cJSON_GetArraySize(props); i < n; ++i)
      {
        cJSON* item = cJSON_GetArrayItem(props, i);

        dmPropertyInfo propertyInfo;
        propertyInfo.setName(cJSON_GetObjectItem(item, "name")->valuestring);
        propertyInfo.setIsOptional(cJSON_IsTrue(cJSON_GetObjectItem(item, "optional")));
        propertyInfo.setIsWritable(cJSON_IsTrue(cJSON_GetObjectItem(item, "writable")));
        propertyInfo.setType(dmValueType_fromString(cJSON_GetObjectItem(item, "type")->valuestring));
        providerInfo.addProperty(propertyInfo);
      }
      m_providerInfo.insert(std::make_pair(root, providerInfo));
    }
    delete [] buffer;
  }
  else
  {
    printf("\n Cannot open json file");
  }

  root.clear();
  delete [] file;
}

std::string const
dmProviderDatabase::getProvider(char const* query)
{
  char* model = new char[100];
  char* parameter = new char[50];
  char* provider = new char[50];
  int found = 0;
  std::vector<dmPropertyInfo> getInfo;
  splitQuery(query, model, parameter);
  std::string dataProvider;

  std::string data(parameter);
  if (data.find("=") != std::string::npos)
  {
    std::size_t position = data.find("=");
    std::string param = data.substr(0, position);
    strcpy(parameter, param.c_str());
  }

  for (auto map_iter = m_providerInfo.cbegin() ; map_iter != m_providerInfo.cend() ; map_iter++)
  {
    for (auto vec_iter = map_iter->second.properties().cbegin(); vec_iter != map_iter->second.properties().cend() ; vec_iter++)
    {
      dmPropertyInfo* dI = new dmPropertyInfo();
      if (strcmp(map_iter->first.c_str(), model) == 0)
      {
        if (strcmp(parameter, "") == 0)
        {
          printf("\nWild Card will be supported in future\n");
          break;
          //exit(0);
        }
        else if (strcmp(vec_iter->name().c_str(), parameter) == 0)
        {
          dI->setName(parameter);
          provider = strdup(map_iter->second.providerName().c_str());
          getInfo.push_back(*dI);
          found = 1;
        }
      }
      delete(dI);
    }
  }

  if (!found)
  {
    printf("\n Parameter not found \n");
    dataProvider = "";
  }
  else
  {
    printf("\nPROVIDER : %s PARAMETER : %s\n", provider, parameter);
    dataProvider.clear();
    dataProvider = std::string(provider);
  }

  delete [] parameter;
  delete [] model;
  delete [] provider;
  provider = NULL;
  return dataProvider;
}

dmQuery*
dmProviderDatabase::createQuery()
{
  return new dmQueryImpl();
}

dmQuery* 
dmProviderDatabase::createQuery(dmProviderOperation op, char const* s)
{
  dmQuery* q = new dmQueryImpl();

  std::string provider = getProvider(s);

  if (!provider.empty())
  {
   bool status = q->setQueryString(op, s);
   if(status)
     q->setProviderName(provider);
  }
  return q;
}
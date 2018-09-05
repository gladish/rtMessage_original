#include "dmClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <rtError.h>
#include "dmProviderDatabase.h"
#include "dmQuery.h"
#include "dmUtility.h"

void getResultValue(std::vector<dmQueryResult::Param> const& results, std::string const& name, dmValue& value)
{
    rtLog_Debug("getResultValue %s", name.c_str());
    for (auto const& param: results)
    {
      if(dmUtility::trimPropertyName(param.Info.fullName()) == name)
      {
        rtLog_Debug("getResultValue found type=%d string=%s", param.Value.type(), param.Value.toString().c_str());
        value = param.Value;
      }
    }
}

class dmClientImpl : public dmClient
{
public:
  dmClientImpl(std::string const& datamodelDir, rtLogLevel logLevel)
  {
    rtLog_SetLevel(logLevel);
    rtLog_Warn("create data base dmClient");
    m_db = new dmProviderDatabase(datamodelDir);
  }

  ~dmClientImpl()
  {
    delete m_db;
  }

  bool runQuery(dmProviderOperation operation, std::string const& parameter, bool recursive, dmClientNotifier* notifier)
  {
    rtLog_Warn("runQuery %s", parameter.c_str());
    bool isWildcard = false;
    bool isList = false;
    bool isListItem;
    std::string objectName(parameter);
    std::shared_ptr<dmProviderInfo> providerInfo;
    if (dmUtility::isWildcard(objectName.c_str()))
    {
      rtLog_Warn("runQuery isWildcard %s", objectName.c_str());
      isWildcard = true;
      //objectName = dmUtility::trimWildcard(objectName);

      providerInfo = m_db->getProviderByParamterName(parameter,&isListItem);
      if(!providerInfo)
      {
        rtLog_Warn("runQuery no providerinfo");
        return false;
      }
      rtLog_Warn("runQuery providerInfo info=%s objectName=%s list=%d isListItem=%d", providerInfo->objectName().c_str(), objectName.c_str(), (int)providerInfo->isList(), (int)isListItem);
      if(!isListItem && providerInfo->isList())
        isList = true;
    }

    if(isWildcard && isList)
    {
      std::string list_name = dmUtility::trimWildcard(parameter);
      std::string parent_name = dmUtility::trimProperty(list_name);
      dmValue value(0);
      rtLog_Warn("dmcli_get list=%s parent=%s", list_name.c_str(), parent_name.c_str());

      std::string num_entries_param = list_name + "NumberOfEntries";
      if(!runOneQuery(operation, num_entries_param, nullptr, &value))
      {
        notifier->onError(RT_FAIL, "dmcli_get list number query failed");
        return false;
      }
      int num_entries = atoi(value.toString().c_str());
      rtLog_Warn("dmcli_get list num_entries=%d", num_entries);

      std::string alias_entries_param = list_name + "IdsOfEntries";
      if(!runOneQuery(operation, alias_entries_param , nullptr, &value))
      {

        notifier->onError(RT_FAIL, "dmcli_get list entry query failed");
        return false;
      }

      std::string entries = value.toString().c_str();
      rtLog_Warn("dmcli_get list entries=%s", entries.c_str());

      std::vector<std::string> out;
      dmUtility::splitString(entries, ',', out);
      rtLog_Warn("dmcli_get split=%d", (int)out.size());
      if(out.size() != num_entries)
      {
        notifier->onError(RT_FAIL, "dmcli_get list failed: size mismatch");
        return false;
      }

      bool success = true;
      for(int i = 0; i < num_entries; ++i)
      {
        std::stringstream list_item_param;
        list_item_param << list_name << "." << out[i] << ".";
        std::vector<dmQueryResult::Param> res;
        if(!runOneQuery(operation, list_item_param.str(), notifier))
          success = false;//if only a list item query fails, we keep going but still at end, return false

        if(operation == dmProviderOperation_Get && recursive)
        {
          std::shared_ptr<dmProviderInfo> objectInfo = m_db->getProviderByParamterName(list_item_param.str(), &isListItem); 
          rtLog_Warn("dmcli_get recurse parameter=%s objectName=%s", list_item_param.str().c_str(), objectInfo->objectName().c_str());
          for(size_t i = 0; i < objectInfo->getChildren().size(); ++i)
          {
            std::string childParameter = list_item_param.str().c_str() + dmUtility::trimPropertyName(objectInfo->getChildren()[i]->objectName()) + ".";
            rtLog_Warn("dmcli_get childObjectName %s", childParameter.c_str());

            success = runQuery(operation, childParameter, recursive, notifier);
          }
        }
      }
      return success;
    }
    else
    {
      bool success = runOneQuery(operation, parameter, notifier);

      if(operation == dmProviderOperation_Get && recursive)
      {
        std::shared_ptr<dmProviderInfo> objectInfo = m_db->getProviderByParamterName(parameter, &isListItem); 
        rtLog_Warn("dmcli_get recurse %s", objectInfo->objectName().c_str());
        for(size_t i = 0; i < objectInfo->getChildren().size(); ++i)
        {
          std::string childParameter = dmUtility::trimProperty(parameter) + "." + dmUtility::trimPropertyName(objectInfo->getChildren()[i]->objectName()) + ".";
          rtLog_Warn("dmcli_get childObjectName %s", childParameter.c_str());

          success = runQuery(operation, childParameter, recursive, notifier);
        }
      }
    }
  }

private:

  bool runOneQuery(dmProviderOperation op, std::string const& parameter, dmClientNotifier* notifier, dmValue* value = nullptr)
  {
    rtLog_Warn("runOneQuery %s", parameter.c_str());
    std::unique_ptr<dmQuery> query(m_db->createQuery(op, parameter.c_str()));

    if (!query)
    {
      std::string msg = "failed to create query";
      rtLog_Warn("%s", msg.c_str());
      if(notifier)
        notifier->onError(RT_FAIL, msg);
      return false;
    }

    if (!query->exec())
    {
      std::string msg = "failed to exec query";
      rtLog_Warn("%s", msg.c_str());
      if(notifier)
        notifier->onError(RT_FAIL, msg);
      return false;
    }
    dmQueryResult const& results = query->results();
    if (results.status() == RT_OK)
    {
      if(notifier)
        notifier->onResult(results);
      if(value)
      {
        *value = 0;
        getResultValue(results.values(), dmUtility::trimPropertyName(parameter), *value);
      }
      return true;
    }
    else
    {
      rtLog_Warn("Error: status=%d msg=%s", results.status(), results.statusMsg().c_str());
      if(notifier)
        notifier->onError(results.status(), results.statusMsg());
      return false;
    }
  }

  dmProviderDatabase* m_db;
};

dmClient* dmClient::create(std::string const& datamodelDir, rtLogLevel logLevel)
{
  return new dmClientImpl(datamodelDir, logLevel);
}

void dmClient::destroy(dmClient* client)
{
  if(client)
    delete client;
}



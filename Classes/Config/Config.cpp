#include "Config.h"
#include "cocos2d.h"
USING_NS_CC;

void GameConfig::initConfig(std::string configFile)
{
	std::string data = FileUtils::getInstance()->getStringFromFile(configFile);
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());
	if (doc.HasParseError())
	{
		log("error");
	}

	rapidjson::Value& itemArray = doc["item"];
	if (itemArray.IsArray())
	{
		for (int i = 0; i < itemArray.Capacity(); ++i)
		{
			rapidjson::Value& itemObject = itemArray[i];
			if (itemObject.IsObject())
			{
				ItemConfig* obj = new ItemConfig();
				obj->m_id = itemObject["ID"].GetInt();
				obj->m_imageName = itemObject["Image"].GetString();
				obj->m_type = static_cast<ItemType>(itemObject["Type"].GetInt());
				obj->m_addNum = itemObject["AddNum"].GetInt();
				m_itemConfig.insert(std::pair<int,ItemConfig*>(obj->m_id, obj));
			}
		}
	}

	rapidjson::Value& enemyArray = doc["enemy"];
	if (enemyArray.IsArray())
	{
		for (int i = 0; i < enemyArray.Capacity(); ++i)
		{
			rapidjson::Value& itemObject = enemyArray[i];
			if (itemObject.IsObject())
			{
				EnemyConfig* obj = new EnemyConfig();
				obj->m_id = itemObject["ID"].GetInt();
				obj->m_imageName = itemObject["Image"].GetString();
				obj->m_attack = itemObject["Attack"].GetInt();
				obj->m_defense = itemObject["Defense"].GetInt();
				obj->m_life = itemObject["Life"].GetInt();
				m_enemyConfig.insert(std::pair<int, EnemyConfig*>(obj->m_id, obj));
			}
		}
	}

	rapidjson::Value& doorArray = doc["door"];
	if (doorArray.IsArray())
	{
		for (int i = 0; i < doorArray.Capacity(); ++i)
		{
			rapidjson::Value& itemObject = doorArray[i];
			if (itemObject.IsObject())
			{
				DoorConfig* obj = new DoorConfig();
				obj->m_id = itemObject["ID"].GetInt();
				obj->m_imageName = itemObject["Image"].GetString();
				obj->m_expendType = static_cast<ItemType>(itemObject["ExpendType"].GetInt());
				obj->m_expendNum = itemObject["ExpendNum"].GetInt();
				m_doorConfig.insert(std::pair<int, DoorConfig*>(obj->m_id, obj));
			}
		}
	}

	rapidjson::Value& npcArray = doc["npc"];
	if (npcArray.IsArray())
	{
		for (int i = 0; i < npcArray.Capacity(); ++i)
		{
			rapidjson::Value& itemObject = npcArray[i];
			if (itemObject.IsObject())
			{
				NpcConfig* obj = new NpcConfig();
				obj->m_id = itemObject["ID"].GetInt();
				obj->m_imageName = itemObject["Image"].GetString();
				m_npcConfig.insert(std::pair<int, NpcConfig*>(obj->m_id, obj));
			}
		}
	}
}

ItemConfig* GameConfig::getItemConfig(int key)
{
	auto it = m_itemConfig.find(key);
	if (it != m_itemConfig.end())
	{
		return it->second;
	}
	return nullptr;
}

EnemyConfig* GameConfig::getEnemyConfig(int key)
{
	auto it = m_enemyConfig.find(key);
	if (it != m_enemyConfig.end())
	{
		return it->second;
	}
	return nullptr;
}

DoorConfig * GameConfig::getDoorConfig(int key)
{
	auto it = m_doorConfig.find(key);
	if (it != m_doorConfig.end())
	{
		return it->second;
	}
	return nullptr;
}

NpcConfig * GameConfig::getNpcConfig(int key)
{
	auto it = m_npcConfig.find(key);
	if (it != m_npcConfig.end())
	{
		return it->second;
	}
	return nullptr;
}

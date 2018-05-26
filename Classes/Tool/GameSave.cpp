#include <sstream>
#include "GameSave.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Config/Config.h"

USING_NS_CC;

template<class T>
std::string getJsonStr(const cocos2d::Map<int, T>& gameMap,const std::string& objectName)
{
	rapidjson::Document writedoc;
	writedoc.SetObject();
	auto& allocator = writedoc.GetAllocator();
	rapidjson::Value valueArray(rapidjson::kArrayType);
	for (auto const& it : gameMap)
	{
		rapidjson::Value valueObject(rapidjson::kObjectType);
		int index = it.first;
		T value = it.second;
		valueObject.AddMember("id", value->m_id, allocator);
		valueObject.AddMember("index", index, allocator);
		valueArray.PushBack(valueObject, allocator);
	}
	rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>> objectN(objectName.c_str(),objectName.size());
	writedoc.AddMember(objectN, valueArray, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	return buffer.GetString();
}

template<>
std::string getJsonStr(const cocos2d::Map<int, Teleport*>& gameMap, const std::string& objectName)
{
	rapidjson::Document writedoc;
	writedoc.SetObject();
	auto& allocator = writedoc.GetAllocator();
	rapidjson::Value valueArray(rapidjson::kArrayType);
	for (auto const& it : gameMap)
	{
		rapidjson::Value valueObject(rapidjson::kObjectType);
		int index = it.first;
		Teleport* value = it.second;
		valueObject.AddMember("id", value->m_id, allocator);
		valueObject.AddMember("index", index, allocator);
		valueObject.AddMember("targetNum", value->m_targetNum, allocator);
		valueObject.AddMember("heroDirection", value->m_heroDirection, allocator);
		valueArray.PushBack(valueObject, allocator);
	}
	rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>> objectN(objectName.c_str(), objectName.size());
	writedoc.AddMember(objectN, valueArray, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writedoc.Accept(writer);
	return buffer.GetString();
}

void GameSave::init(GameLayer * gameLayer)
{
	if (gameLayer && m_gameLayer != gameLayer)
	{
		m_gameLayer = gameLayer;
	}
}

bool GameSave::readData()
{
	bool ret = false;
	if (m_gameLayer == nullptr)
	{
		return ret;
	}
	if (m_sqlDB.openDB("save_data.db"))
	{
		if (readHeroData() && readMapData())
		{
			ret = true;
			Hero* hero = m_gameLayer->m_hero;
			m_gameLayer->switchMap(hero->m_curTier);
			hero->m_gameLayer = m_gameLayer;
			hero->setPosition(m_gameLayer->m_curGameMap->m_heroCurrPos);
			hero->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
			hero->updateAllData();
			m_gameLayer->addChild(hero, DO_HERO);
		}
	}
	m_sqlDB.closeDB();
	return ret;
}

bool GameSave::saveData()
{
	bool ret = false;
	if (m_gameLayer == nullptr)
	{
		return ret;
	}
	if (m_sqlDB.openDB("save_data.db"))
	{
		std::string createTableSql = FileUtils::getInstance()->getStringFromFile("Config/table.sql");
		if (!createTableSql.empty())
		{
			//������
			m_sqlDB.execSql(createTableSql);
			//��ձ�����
			m_sqlDB.execSql("DELETE FROM map_data;DELETE FROM hero_data");
			saveMapData();
			saveHeroData();

			ret = true;
		}
	}
	m_sqlDB.closeDB();
	return ret;
}

bool GameSave::readMapData()
{
	std::string sqlStr = "SELECT * FROM map_data;";
	//��յ�ǰ���е�ͼ
	m_gameLayer->m_gameMaps.clear();
	m_gameLayer->m_curGameMap->removeFromParentAndCleanup(true);
	m_gameLayer->m_curGameMap = nullptr;
	if (m_sqlDB.execSql(sqlStr, &GameSave::selectMapCallback, m_gameLayer))
	{
		return true;
	}
	return false;
}

bool GameSave::saveMapData()
{
	const Map<int, GameMap*>& gameMaps = m_gameLayer->m_gameMaps;
	for (auto const& it : gameMaps)
	{
		std::ostringstream oss;
		oss << "INSERT INTO map_data VALUES(";
		oss << it.first << ',';
		GameMap* gameMap = it.second;
		if (gameMap)
		{
			std::string enemyStr = getJsonStr<Enemy*>(gameMap->m_enemyMap, "enemy");
			std::string itemStr = getJsonStr<Item*>(gameMap->m_articlesMap, "item");
			std::string doorStr = getJsonStr<Door*>(gameMap->m_doorMap, "door");
			std::string npcStr = getJsonStr<Npc*>(gameMap->m_npcMap, "npc");
			std::string teleportStr = getJsonStr<Teleport*>(gameMap->m_teleportMap, "teleport");
			oss << "\'" << enemyStr << "\'" << ',';
			oss << "\'" << itemStr << "\'" << ',';
			oss << "\'" << doorStr << "\'" << ',';
			oss << "\'" << npcStr << "\'" << ',';
			oss << "\'" << teleportStr << "\'";
			oss << ");";
			std::string sql = oss.str();
			//log("%s", sql.c_str());
			if (!m_sqlDB.execSql(sql))
				return false;
		}
		else
		{
			return false;
		}
	}
	return true;
}

int GameSave::selectMapCallback(void * key, int c, char ** v, char ** cols)
{
	if (key != nullptr)
	{
		GameLayer* gameLayer = static_cast<GameLayer*>(key);
		Hero* hero = gameLayer->m_hero;
		GameMap* gameMap = nullptr;
		int number = 0;
		for (int i = 0; i < c; i++)
		{
			std::string name = cols[i];
			std::string value = v[i];
			if (name == "tier")
			{
				number = Value(value).asInt();
				std::string tmxFileName = "Map/" + Value(number).asString() + "/" + Value(number).asString() + ".tmx";
				gameMap = GameMap::createGameMapWithTMXFile(tmxFileName);
				gameMap->m_tier = number;
			}
			if (gameMap)
			{
				if (name == "enemy")
				{
					rapidjson::Document doc;
					doc.Parse<0>(v[i]);
					if (doc.HasParseError())
					{
						log("error");
					}

					rapidjson::Value& itemArray = doc["enemy"];
					if (itemArray.IsArray())
					{
						for (int i = 0; i < itemArray.Capacity(); ++i)
						{
							rapidjson::Value& itemObject = itemArray[i];
							if (itemObject.IsObject())
							{
								int id = itemObject["id"].GetInt();
								int index = itemObject["index"].GetInt();
								std::string frameName = "enemy" + Value(id).asString() + "_1.png";
								Enemy* item = Enemy::createEnemyWithFrameName(frameName);
								EnemyConfig* tmp = gGameConfig.getEnemyConfig(id);
								if (tmp != nullptr)
								{
									item->m_id = tmp->m_id;
									item->m_attack = tmp->m_attack;
									item->m_defense = tmp->m_defense;
									item->m_life = tmp->m_life;
									gameMap->m_enemyMap.insert(index, item);
								}
							}
						}
					}
				}
				else if (name == "item")
				{
					rapidjson::Document doc;
					doc.Parse<0>(v[i]);
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
								int id = itemObject["id"].GetInt();
								int index = itemObject["index"].GetInt();
								std::string frameName = "item" + Value(id).asString() + ".png";
								Item* item = Item::createArticlesWithFrameName(frameName);
								ItemConfig* tmp = gGameConfig.getItemConfig(id);
								if (tmp != nullptr)
								{
									item->m_id = tmp->m_id;
									item->m_type = tmp->m_type;
									item->m_addNum = tmp->m_addNum;
									gameMap->m_articlesMap.insert(index, item);
								}
							}
						}
					}
				}
				else if (name == "door")
				{
					rapidjson::Document doc;
					doc.Parse<0>(v[i]);
					if (doc.HasParseError())
					{
						log("error");
					}

					rapidjson::Value& itemArray = doc["door"];
					if (itemArray.IsArray())
					{
						for (int i = 0; i < itemArray.Capacity(); ++i)
						{
							rapidjson::Value& itemObject = itemArray[i];
							if (itemObject.IsObject())
							{
								int id = itemObject["id"].GetInt();
								int index = itemObject["index"].GetInt();
								std::string frameName = "door_1_" + Value(id).asString() + ".png";
								Door* item = Door::createDoorWithFrameName(frameName);
								DoorConfig* tmp = gGameConfig.getDoorConfig(id);
								if (tmp != nullptr)
								{
									item->m_id = tmp->m_id;
									item->m_expendType = tmp->m_expendType;
									item->m_expendNum = tmp->m_expendNum;
									gameMap->m_doorMap.insert(index, item);
								}
							}
						}
					}
				}
				else if (name == "npc")
				{
					rapidjson::Document doc;
					doc.Parse<0>(v[i]);
					if (doc.HasParseError())
					{
						log("error");
					}

					rapidjson::Value& itemArray = doc["npc"];
					if (itemArray.IsArray())
					{
						for (int i = 0; i < itemArray.Capacity(); ++i)
						{
							rapidjson::Value& itemObject = itemArray[i];
							if (itemObject.IsObject())
							{
								int id = itemObject["id"].GetInt();
								int index = itemObject["index"].GetInt();
								std::string frameName = "door_1_" + Value(id).asString() + ".png";
								NpcConfig* tmp = gGameConfig.getNpcConfig(id);
								if (tmp != nullptr)
								{
									Npc* item = Npc::createNpcWithFrameName(tmp->m_imageName);
									item->m_id = tmp->m_id;
									gameMap->m_npcMap.insert(index, item);
								}
							}
						}
					}
				}
				else if (name == "teleport")
				{
					rapidjson::Document doc;
					doc.Parse<0>(v[i]);
					if (doc.HasParseError())
					{
						log("error");
					}

					rapidjson::Value& itemArray = doc["teleport"];
					if (itemArray.IsArray())
					{
						for (int i = 0; i < itemArray.Capacity(); ++i)
						{
							rapidjson::Value& itemObject = itemArray[i];
							if (itemObject.IsObject())
							{
								int id = itemObject["id"].GetInt();
								int index = itemObject["index"].GetInt();

								std::string fileName = (id == 1 ? "images/down_floor.png" : "images/up_floor.png");
								Teleport* item = Teleport::createTeleportWithFile(fileName);
								item->m_id = id;
								item->m_targetNum = itemObject["targetNum"].GetInt();
								item->m_heroDirection = static_cast<Direction>(itemObject["heroDirection"].GetInt());
								gameMap->m_teleportMap.insert(index, item);
								if (hero->m_curTier > number && id == 2 || hero->m_curTier <= number && id == 1)
								{
									gameMap->m_heroCurrPos = gameMap->positionForIndex(index);
									gameMap->m_heroDirection = item->m_heroDirection;
								}
							}
						}
					}
				}
			}
		}
		gameMap->initWall();
		gameMap->initSprite();
		gameMap->initAllNode();
		gameLayer->m_gameMaps.insert(number, gameMap);
	}
	return 0;
}

int GameSave::selectHeroCallback(void * key, int c, char ** v, char ** cols)
{
	if (key != nullptr && c > 0)
	{
		GameLayer* gameLayer = static_cast<GameLayer*>(key);
		Hero* hero = Hero::createHeroWithFrameName("hero4_1.png");
		for (int i = 0; i < c; i++)
		{
			std::string name = cols[i];
			if (v[i] != nullptr)
			{
				std::string value = v[i];
				if (name == "attack")
					hero->m_attackNum = Value(value).asInt();
				else if (name == "defense")
					hero->m_defenseNum = Value(value).asInt();
				else if (name == "life")
					hero->m_lifeNum = Value(value).asInt();
				else if (name == "yellowKey")
					hero->m_yellowKeyNum = Value(value).asInt();
				else if (name == "blueKey")
					hero->m_blueKeyNum = Value(value).asInt();
				else if (name == "redKey")
					hero->m_redKeyNum = Value(value).asInt();
				else if (name == "curTier")
				{
					hero->m_curTier = Value(value).asInt();
				}
			}
		}
		gameLayer->m_hero = hero;
		return 0;
	}
	return 0;
}

bool GameSave::readHeroData()
{
	std::string sqlStr = "SELECT * FROM hero_data;";
	//��յ�ǰ���е�ͼ
	m_gameLayer->m_hero->removeFromParentAndCleanup(true);
	m_gameLayer->m_hero = nullptr;
	if (m_sqlDB.execSql(sqlStr, &GameSave::selectHeroCallback, m_gameLayer))
	{
		return true;
	}
	return false;
}

bool GameSave::saveHeroData()
{
	const Hero* hero = m_gameLayer->m_hero;
	std::ostringstream oss;
	oss << "INSERT INTO hero_data VALUES(1,";
	oss << hero->m_attackNum << ',';
	oss << hero->m_defenseNum << ',';
	oss << hero->m_lifeNum << ',';
	oss << hero->m_yellowKeyNum << ',';
	oss << hero->m_blueKeyNum << ',';
	oss << hero->m_redKeyNum << ',';
	oss << m_gameLayer->m_curGameMap->m_tier << ',';
	oss << "null);";
	std::string sql = oss.str();
	//log("%s", sql.c_str());
	if (m_sqlDB.execSql(sql))
		return true;
	return false;
}

CppSqlite::CppSqlite()
{
	m_db = nullptr;
}

bool CppSqlite::openDB(std::string dbFileName)
{
	if (dbFileName.empty())
	{
		return false;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string path =  FileUtils::getInstance()->getWritablePath();
	dbFileName = path + dbFileName;
#endif
	int errorCode = SQLITE_OK;
	if (errorCode = sqlite3_open(dbFileName.c_str(), &m_db) != SQLITE_OK)
	{
		const char* errMsg = sqlite3_errmsg(m_db);
		log("sqlite open failed!!! errMsg = %s; dbFileName = %s", errMsg, dbFileName.c_str());
		closeDB();
		return false;
	}
	return true;
}

bool CppSqlite::execSql(std::string sqlStr, int(*callback)(void *, int, char **, char **), void* object)
{
	if (sqlStr.empty())
	{
		return false;
	}

	if (sqlite3_exec(m_db, sqlStr.c_str(), callback, object, nullptr) != SQLITE_OK)
	{
		const char* errMsg = sqlite3_errmsg(m_db);
		log("sql exec failed!!! errMsg = %s; sqlStr = %s", errMsg, sqlStr.c_str());
		return false;
	}
	return true;
}

void CppSqlite::closeDB()
{
	sqlite3_close(m_db);
	m_db = nullptr;
}
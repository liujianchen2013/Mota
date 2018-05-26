#pragma once
#include "Tool/Singleton.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "Tool/GameDefine.h"
#include <map>

struct ItemConfig
{
	int m_id;
	std::string m_imageName;
	ItemType m_type;
	int m_addNum;
};

struct EnemyConfig
{
	int m_id;
	std::string m_imageName;
	int m_attack;
	int m_defense;
	int m_life;
};

struct DoorConfig
{
	int m_id;
	std::string m_imageName;
	ItemType m_expendType;
	int m_expendNum;
};

struct NpcConfig
{
	int m_id;
	std::string m_imageName;
};

class GameConfig : public Singleton<GameConfig>
{
public:
	void initConfig(std::string configFile);
	ItemConfig* getItemConfig(int key);
	EnemyConfig* getEnemyConfig(int key);
	DoorConfig* getDoorConfig(int key);
	NpcConfig* getNpcConfig(int key);
public:
	std::map<int, ItemConfig*> m_itemConfig;
	std::map<int, EnemyConfig*> m_enemyConfig;
	std::map<int, DoorConfig*> m_doorConfig;
	std::map<int, NpcConfig*> m_npcConfig;
};

#define gGameConfig GameConfig::Instance()
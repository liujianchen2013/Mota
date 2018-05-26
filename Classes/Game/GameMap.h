#pragma once

#include <iostream>
#include "cocos2d.h"
#include "Game/Item.h"
#include "Game/Door.h"
#include "Game/Enemy.h"
#include "Game/Npc.h"
#include "Game/Hero.h"
#include "Game/Teleport.h"
#include "Algorithm/AutoFindPath.h"

class GameMap : public cocos2d::TMXTiledMap
{
	CC_SYNTHESIZE(cocos2d::TMXLayer*, wallLayer, WallLayer)

public:
	GameMap();
	~GameMap();
	static GameMap* createGameMapWithTMXFile(const std::string tmxFile);

	cocos2d::Point tileCoordForPosition(const cocos2d::Point& position) const;
	cocos2d::Point positionForTileCoord(const cocos2d::Point& position) const;
	cocos2d::Point positionForIndex(int index) const;

//private:
	void initObject();
	void initAllNode();
	void initSprite();
	void initWall();
private:
	void initEnemy();
	void initItem();
	void initDoor();
	void initNpc();
	void initTeleport();
	
public:
	cocos2d::Map<int, Enemy*> m_enemyMap;
	cocos2d::Map<int, Npc*> m_npcMap;
	cocos2d::Map<int, Door*> m_doorMap;
	cocos2d::Map<int, Item*> m_articlesMap;
	cocos2d::Map<int, Teleport*> m_teleportMap;
	cocos2d::Point m_heroCurrPos;//英雄初始位置
	Direction m_heroDirection;//英雄初始方向
	//std::map<cocos2d::Point, ::Node*> m_allNode;
	AutoFindPath m_autoFindPath;
	int m_tier;	//层数
};
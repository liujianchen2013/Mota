#include "GameMap.h"
#include "Config/Config.h"
USING_NS_CC;

GameMap::GameMap()
{

}

GameMap::~GameMap()
{
	//for (auto it = m_enemyMap.begin(); it != m_enemyMap.end(); ++it)
	//{
	//	if (it->second)
	//	{
	//		delete it->second;
	//	}
	//}
}

GameMap * GameMap::createGameMapWithTMXFile(const std::string tmxFile)
{
	GameMap * pRet = new GameMap();
	if (pRet->initWithTMXFile(tmxFile))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void GameMap::initObject()
{
	initWall();
	initEnemy();
	initItem();
	initDoor();
	initNpc();
	initTeleport();
	initSprite();
}

void GameMap::initAllNode()
{
	Size size = this->getMapSize();
	for (int i = 0; i < size.height; ++i)
	{
		for (int j = 0; j < size.width; ++j)
		{
			::Node* node = new ::Node();
			node->m_pos = Point(i, j);
			//获取墙壁层对应坐标的图块ID
			int targetTileGID = this->getWallLayer()->getTileGIDAt(Point(i,j));
			//如果图块ID不为0，表示有墙
			if (targetTileGID)
			{
				node->m_isWall = true;
			}
			int index = i + j * size.width;
			if (m_enemyMap.find(index) != m_enemyMap.end() ||
				m_npcMap.find(index) != m_npcMap.end() ||
				m_doorMap.find(index) != m_doorMap.end() ||
				//m_articlesMap.find(index) != m_articlesMap.end() ||
				m_teleportMap.find(index) != m_teleportMap.end()
				//false
				)
			{
				node->m_isWall = true;
			}
			m_autoFindPath.m_allNode.insert(std::pair<cocos2d::Point, ::Node*>(node->m_pos,node));
		}
	}
}

void GameMap::initEnemy()
{
	//获取对象层
	TMXObjectGroup* group = this->objectGroupNamed("enemy");
	//获取对象层的所有对象
	const ValueVector& enemyObjects = group->getObjects();
	//转化成enemy类型
	for (auto const& enemy : enemyObjects)
	{
		const ValueMap& dict = enemy.asValueMap();
		//获取瓦片坐标
		Point tileCoord = tileCoordForPosition(Point(dict.at("x").asInt(), dict.at("y").asInt()));
		//获取索引
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;
		//获取类型ID
		int id = dict.at("enemy_id").asInt();
		std::string frameName = "enemy" + Value(id).asString() + "_1.png";
		Enemy* item = Enemy::createEnemyWithFrameName(frameName);
		EnemyConfig* tmp = gGameConfig.getEnemyConfig(id);
		if (tmp != nullptr)
		{
			item->m_id = tmp->m_id;
			item->m_attack = tmp->m_attack;
			item->m_defense = tmp->m_defense;
			item->m_life = tmp->m_life;
			m_enemyMap.insert(index, item);
		}
	}
}

void GameMap::initItem()
{
	//获取对象层
	TMXObjectGroup* group = this->objectGroupNamed("Articles");
	//获取对象层的所有对象
	const ValueVector& articlesObjects = group->getObjects();
	//转化成enemy类型
	for (auto const& articles : articlesObjects)
	{
		const ValueMap& dict = articles.asValueMap();
		//获取瓦片坐标
		Point pos = Point(dict.at("x").asInt(), dict.at("y").asInt());
		Point tileCoord = tileCoordForPosition(pos);
		//获取索引
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;
		//获取类型ID
		int id = dict.at("articles_id").asInt();

		std::string frameName = "item" + Value(id).asString() + ".png";
		Item* item = Item::createArticlesWithFrameName(frameName);
		ItemConfig* tmp = gGameConfig.getItemConfig(id);
		if (tmp != nullptr)
		{
			item->m_id = tmp->m_id;
			item->m_type = tmp->m_type;
			item->m_addNum = tmp->m_addNum;
			m_articlesMap.insert(index, item);
		}
	}
}

void GameMap::initDoor()
{
	//获取对象层
	TMXObjectGroup* group = this->objectGroupNamed("door");
	//获取对象层的所有对象
	const ValueVector& doorObjects = group->getObjects();
	//转化成enemy类型
	for (auto const& door : doorObjects)
	{
		const ValueMap& dict = door.asValueMap();
		//获取瓦片坐标
		Point pos = Point(dict.at("x").asInt(), dict.at("y").asInt());
		Point tileCoord = tileCoordForPosition(pos);
		//获取索引
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;
		//获取类型ID
		int id = dict.at("door_id").asInt();

		std::string frameName = "door_1_" + Value(id).asString() + ".png";
		Door* item = Door::createDoorWithFrameName(frameName);
		DoorConfig* tmp = gGameConfig.getDoorConfig(id);
		if (tmp != nullptr)
		{
			item->m_id = tmp->m_id;
			item->m_expendType = tmp->m_expendType;
			item->m_expendNum = tmp->m_expendNum;
			m_doorMap.insert(index, item);
		}
	}
}

void GameMap::initNpc()
{
	//获取对象层
	TMXObjectGroup* group = this->objectGroupNamed("Npc");
	//获取对象层的所有对象
	const ValueVector& npcObjects = group->getObjects();
	//转化成enemy类型
	for (auto const& npc : npcObjects)
	{
		const ValueMap& dict = npc.asValueMap();
		//获取瓦片坐标
		Point pos = Point(dict.at("x").asInt(), dict.at("y").asInt());
		Point tileCoord = tileCoordForPosition(pos);
		//获取索引
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;
		//获取类型ID
		int id = dict.at("npc_id").asInt();
		NpcConfig* tmp = gGameConfig.getNpcConfig(id);
		if (tmp != nullptr)
		{
			Npc* item = Npc::createNpcWithFrameName(tmp->m_imageName);
			item->m_id = tmp->m_id;
			m_npcMap.insert(index, item);
		}
	}
}

void GameMap::initTeleport()
{
	//获取对象层
	TMXObjectGroup* group = this->objectGroupNamed("Teleport");
	//获取对象层的所有对象
	const ValueVector& teleportObjects = group->getObjects();
	//转化成enemy类型
	for (auto const& teleport : teleportObjects)
	{
		const ValueMap& dict = teleport.asValueMap();
		//获取瓦片坐标
		Point pos = Point(dict.at("x").asInt(), dict.at("y").asInt());
		Point tileCoord = tileCoordForPosition(pos);
		//获取索引
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;
		//获取类型ID
		int id = dict.at("teleport_id").asInt();

		std::string fileName = (id == 1 ? "images/down_floor.png" : "images/up_floor.png");
		Teleport* item = Teleport::createTeleportWithFile(fileName);
		item->m_id = id;
		item->m_targetNum = dict.at("targetNum").asInt();
		item->m_heroDirection = static_cast<Direction>(dict.at("direction").asInt());
		m_teleportMap.insert(index, item);
		if (id == 1)
		{
			m_heroCurrPos = pos;
			m_heroDirection = item->m_heroDirection;
		}
	}
}

void GameMap::initSprite()
{
	auto spriteFrameCache = SpriteFrameCache::getInstance();

	for (auto const& it : m_enemyMap)
	{
		int index = it.first;
		Enemy* enemy = it.second;
		Vec2 pos = positionForIndex(index);
		enemy->setAnchorPoint(Point::ZERO);
		enemy->setPosition(pos);
		//构建动画
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(4);
		for (int i = 1; i < 5; ++i)
		{
			std::string name = "enemy" + Value(enemy->m_id).asString() + "_" + Value(i).asString() + ".png";
			animFrames.pushBack(spriteFrameCache->getSpriteFrameByName(name));
		}
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
		Animate* animate = Animate::create(animation);
		enemy->runAction(RepeatForever::create(animate));
		this->addChild(enemy);
	}

	for (auto const& it : m_articlesMap)
	{
		int index = it.first;
		Item* item = it.second;
		Vec2 pos = positionForIndex(index);
		item->setAnchorPoint(Point::ZERO);
		item->setPosition(pos);
		this->addChild(item);
	}

	for (auto const& it : m_doorMap)
	{
		int index = it.first;
		Door* door = it.second;
		Vec2 pos = positionForIndex(index);
		door->setAnchorPoint(Point::ZERO);
		door->setPosition(pos);
		this->addChild(door);
	}

	for (auto const& it : m_npcMap)
	{
		int index = it.first;
		Npc* npc = it.second;
		Vec2 pos = positionForIndex(index);
		npc->setAnchorPoint(Point::ZERO);
		npc->setPosition(pos);

		//构建动画
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(4);
		for (int i = 1; i < 5; ++i)
		{
			std::string name = "npc01_" + Value(npc->m_id).asString() + "_" + Value(i).asString() + ".png";
			animFrames.pushBack(spriteFrameCache->getSpriteFrameByName(name));
		}
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
		Animate* animate = Animate::create(animation);
		npc->runAction(RepeatForever::create(animate));
		this->addChild(npc);
	}

	for (auto const& it : m_teleportMap)
	{
		int index = it.first;
		Teleport* teleport = it.second;
		Vec2 pos = positionForIndex(index);
		teleport->setAnchorPoint(Point::ZERO);
		teleport->setPosition(pos);
		this->addChild(teleport);
		//if (teleport->m_id == 1)
		//{
		//	m_heroCurrPos = pos;
		//	m_heroDirection = teleport->m_heroDirection;
		//}
	}
}

void GameMap::initWall()
{
	wallLayer = this->getLayer("wall");
}

Point GameMap::tileCoordForPosition(const Point & position) const
{
	int x = position.x / this->getTileSize().width;
	int y = (((this->getMapSize().height - 1) * this->getTileSize().height) - position.y) / this->getTileSize().height;
	return Point(x, y);
}

cocos2d::Point GameMap::positionForTileCoord(const cocos2d::Point & tileCoord) const
{
	int x = tileCoord.x * this->getTileSize().width;
	int y = getMapSize().height * getTileSize().height - tileCoord.y * this->getTileSize().height;
	return Point(x, y);
}

cocos2d::Point GameMap::positionForIndex(int index) const
{
	int x = index % int(getMapSize().width);
	int y = index / int(getMapSize().height) + 1;
	return positionForTileCoord(Point(x, y));
}

#include "Hero.h"
#include "GameMap.h"
#include "Layer/GameLayer.h"
#include "Layer/ShowLayer.h"
#include "Config/EnToCh.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Hero * Hero::createHeroWithFrameName(const std::string frameName)
{
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = { 0 };
	sprintf(msg, "Invalid spriteFrameName: %s", frameName.c_str());
	CCASSERT(frame != nullptr, msg);
#endif

	Hero *sprite = new (std::nothrow) Hero();
	if (sprite && frame && sprite->initWithSpriteFrame(frame))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Hero::~Hero()
{

}

void Hero::move(Direction dir)
{
	Point point = this->getPosition();
	switch (dir)
	{
	case DIR_DOWN :
		point += Point(0, -32);
		break;
	case DIR_LEFT:
		point += Point(-32, 0);
		break;
	case DIR_RIGHT:
		point += Point(32, 0);
		break;
	case DIR_UP:
		point += Point(0, 32);
		break;
	}

	CollisionType type = Collision(point);
	if (type == CT_NONE || type == CT_ITEM || type == CT_ENEMY  )
	{
		this->setPosition(point);
		this->ChangeHeroDirection(dir);

	}
	else if (type == CT_WALL || type == CT_NPC || type == CT_DOOR)
	{
		this->ChangeHeroDirection(dir);
	}

	if (type == CT_NONE || type == CT_ITEM || type == CT_ENEMY || type == CT_DOOR)
	{
		Point targetTileCoord = m_gameLayer->m_curGameMap->tileCoordForPosition(point);
		std::map<cocos2d::Point, ::Node*>& allNode = m_gameLayer->m_curGameMap->m_autoFindPath.m_allNode;
		auto item = allNode.find(targetTileCoord);
		if (item != allNode.end())
		{
			if (item->second)
			{
				item->second->m_isWall = false;
			}
		}
	}
}

void Hero::ChangeHeroDirection(Direction dir)
{
	std::string spriteFrameName = "hero" + Value(dir).asString() + "_1.png";
	SpriteFrame* newspriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
	this->setSpriteFrame(newspriteFrame);
}

CollisionType Hero::Collision(Point position)
{
	Point targetTileCoord = m_gameLayer->m_curGameMap->tileCoordForPosition(position);

	//获取墙壁层对应坐标的图块ID
	int targetTileGID = m_gameLayer->m_curGameMap->getWallLayer()->getTileGIDAt(targetTileCoord);
	//如果图块ID不为0，表示有墙
	if (targetTileGID)
	{
		return CT_WALL;
	}

	int index = targetTileCoord.x + targetTileCoord.y * m_gameLayer->m_curGameMap->getMapSize().width;

	//检测碰撞怪物
	Map<int, Enemy*>& enemyMap = m_gameLayer->m_curGameMap->m_enemyMap;
	if (enemyMap.find(index) != enemyMap.end())
	{
		Enemy* item = enemyMap.at(index);
		if (item)
		{
			if (enemyCombat(item))
			{
				item->removeFromParentAndCleanup(true);
				enemyMap.erase(index);
				return CT_ENEMY;
			}
			else
			{
				m_gameLayer->showTip(en2ch("Be no fighting match"), this->getPosition() + Point(0,32));
				auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
				audio->playEffect("sounds/cuowu.mp3"/*, false, 1.0f, 1.0f, 1.0f*/);
			}
			return CT_WALL;
		}
	}
	//检测碰撞物品
	Map<int, Item*>& articlesMap = m_gameLayer->m_curGameMap->m_articlesMap;
	if (articlesMap.find(index) != articlesMap.end())
	{
		Item* item = articlesMap.at(index);
		if (item)
		{
			packupItem(item);
			item->removeFromParentAndCleanup(true);
			articlesMap.erase(index);
			return CT_ITEM;
		}
	}

	//检测碰撞门
	Map<int, Door*>& doorMap = m_gameLayer->m_curGameMap->m_doorMap;
	if (doorMap.find(index) != doorMap.end())
	{
		Door* item = doorMap.at(index);
		if (item)
		{
			if (openDoor(item))
			{
				//构建动画
				Vector<SpriteFrame*> animFrames;
				animFrames.reserve(4);
				for (int i = 1; i < 5; ++i)
				{
					std::string name = "door_" + Value(i).asString() + "_" + Value(item->m_id).asString() + ".png";
					animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
				}
				Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
				Animate* animate = Animate::create(animation);
				auto removeItem = CallFunc::create([item]() {
					item->removeFromParentAndCleanup(true);
				});
				item->runAction(Sequence::create(animate, removeItem,nullptr));
				doorMap.erase(index);
				return CT_DOOR;
			}
			return CT_WALL;
		}
	}

	//检测碰撞传送门
	Map<int, Teleport*>& teleportMap = m_gameLayer->m_curGameMap->m_teleportMap;
	if (teleportMap.find(index) != teleportMap.end())
	{
		Teleport* item = teleportMap.at(index);
		if (item)
		{
			//设置地图上人物位置方向为楼梯的位置
			m_gameLayer->m_curGameMap->m_heroCurrPos = item->getPosition();
			//设置地图上人物初始方向为楼梯的方向
			m_gameLayer->m_curGameMap->m_heroDirection = item->m_heroDirection;
			//切换地图
			m_gameLayer->switchMap(item->m_targetNum);
			return CT_TELEPORT;
		}
	}

	//检测碰撞NPC
	Map<int, Npc*>& npcMap = m_gameLayer->m_curGameMap->m_npcMap;
	if (npcMap.find(index) != npcMap.end())
	{
		Npc* item = npcMap.at(index);
		if (item)
		{
			//item->removeFromParentAndCleanup(true);
			//npcMap.erase(index);
			return CT_NPC;
		}
	}

	return CT_NONE;
}

void Hero::changeAttackNum(int num)
{
	m_attackNum += num;
	gShowLayer->setAttackNum(m_attackNum);
}

void Hero::changeDefenseNum(int num)
{
	m_defenseNum += num;
	gShowLayer->setDefenseNum(m_defenseNum);
}

void Hero::changeLifeNum(int num)
{
	m_lifeNum += num;
	gShowLayer->setLifeNum(m_lifeNum);
}

void Hero::changeYellowKeyNum(int num)
{
	m_yellowKeyNum += num;
	gShowLayer->setYellowKeyNum(m_yellowKeyNum);
}

void Hero::changeBlueKeyNum(int num)
{
	m_blueKeyNum += num;
	gShowLayer->setBlueKeyNum(m_blueKeyNum);
}

void Hero::changeRedKeyNum(int num)
{
	m_redKeyNum += num;
	gShowLayer->setRedKeyNum(m_redKeyNum);
}

void Hero::updateAllData()
{
	gShowLayer->setAttackNum(m_attackNum);
	gShowLayer->setDefenseNum(m_defenseNum);
	gShowLayer->setLifeNum(m_lifeNum);
	gShowLayer->setYellowKeyNum(m_yellowKeyNum);
	gShowLayer->setBlueKeyNum(m_blueKeyNum);
	gShowLayer->setRedKeyNum(m_redKeyNum);
}

void Hero::packupItem(Item * item)
{
	if (item != nullptr)
	{
		std::string tipStr = "";
		Color3B color;
		switch (item->m_type)
		{
		case IT_Attack:
			changeAttackNum(item->m_addNum);
			tipStr = "ATK";
			color = Color3B::RED;
			break;
		case IT_Defense:
			changeDefenseNum(item->m_addNum);
			tipStr = "DEF";
			color = Color3B::BLUE;
			break;
		case IT_Life:
			changeLifeNum(item->m_addNum);
			tipStr = "HP";
			color = Color3B::RED;
			break;
		case IT_Yellow_Key:
			changeYellowKeyNum(item->m_addNum);
			tipStr = "YellowKey";
			color = Color3B::YELLOW;
			break;
		case IT_Blue_Key:
			changeBlueKeyNum(item->m_addNum);
			tipStr = "BlueKey";
			color = Color3B::BLUE;
			break;
		case IT_Red_Key:
			changeRedKeyNum(item->m_addNum);
			tipStr = "RedKey";
			color = Color3B::RED;
			break;
		}
		tipStr = en2ch(tipStr);
		tipStr += " +" + Value(item->m_addNum).asString();
		m_gameLayer->showTip(tipStr, item->getPosition() + Point(0, 32),color);
		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->playEffect("sounds/huodewupin.mp3"/*, false, 1.0f, 1.0f, 1.0f*/);
	}
	
}

bool Hero::openDoor(Door * door)
{
	if (door)
	{
		bool isOpen = false;
		std::string tipStr;
		Color3B tipColor = Color3B::WHITE;
		switch (door->m_expendType)
		{
		case IT_Yellow_Key:
			if (m_yellowKeyNum)
			{
				changeYellowKeyNum(0 - door->m_expendNum);
				tipStr = "YellowKey";
				tipColor = Color3B::YELLOW;
				isOpen =  true;
			}
			break;
		case IT_Blue_Key:
			if (m_blueKeyNum)
			{
				changeBlueKeyNum(0 - door->m_expendNum);
				tipStr = "BlueKey";
				tipColor = Color3B::BLUE;
				isOpen = true;
			}
			break;
		case IT_Red_Key:
			if (m_redKeyNum)
			{
				changeRedKeyNum(0 - door->m_expendNum);
				tipStr = "RedKey";
				tipColor = Color3B::RED;
				isOpen = true;
			}
			break;
		default:
			isOpen = true;
			tipStr = "";
			break;
		}
		if (isOpen)
		{
			if (!tipStr.empty())
			{
				tipStr = en2ch(tipStr);
				tipStr += " -1";
				m_gameLayer->showTip(tipStr, this->getPosition() + Point(0, 32), tipColor);
			}
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect("sounds/kaimen.mp3"/*, false, 1.0f, 1.0f, 1.0f*/);
			return true;
		}
		else
		{
			m_gameLayer->showTip(en2ch("Not key"), this->getPosition() + Point(0, 32), Color3B::RED);
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect("sounds/cuowu.mp3"/*, false, 1.0f, 1.0f, 1.0f*/);
		}
	}
	return false;
}

bool Hero::enemyCombat(Enemy * enemy)
{
	//公式: 战斗次数 = 敌血 / (己攻 - 敌防)  取整; 己攻小于敌防说明打不过
	//损耗 = 战斗次数 * (敌攻 - 己防)
	if (m_attackNum <= enemy->m_defense)
	{
		return false;
	}
	//战斗次数
	int combatCount = enemy->m_life / (m_attackNum - enemy->m_defense);
	int life = (enemy->m_attack - m_defenseNum) * combatCount;
	if (life < 0)
		life = 0;
	if (m_lifeNum <= life)
	{
		return false;
	}
	changeLifeNum(0 - life);
	std::string tipStr = en2ch("HP");
	tipStr += " -";
	m_gameLayer->showTip(tipStr + Value(life).asString(), enemy->getPosition() + Point(0, 32), Color3B::RED);
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("sounds/jianji.mp3"/*, false, 1.0f, 1.0f, 1.0f*/);
	return true;
}

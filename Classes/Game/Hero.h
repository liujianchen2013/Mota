#pragma once
#include <memory>
#include "cocos2d.h"
#include "Tool/GameDefine.h"
#include "Game/Item.h"
//#include "GameMap.h"
class GameLayer;
class Door;
class Enemy;

class Hero : public cocos2d::Sprite
{
public:
	static Hero* createHeroWithFrameName(const std::string frameName);
	~Hero();
	void move(Direction dir);
	void ChangeHeroDirection(Direction dir);
	GameLayer* m_gameLayer = nullptr;
	std::vector<Direction> m_movePath;
private:
	CollisionType Collision(cocos2d::Point position);

public:
	void changeAttackNum(int num);
	void changeDefenseNum(int num);
	void changeLifeNum(int num);
	void changeYellowKeyNum(int num);
	void changeBlueKeyNum(int num);
	void changeRedKeyNum(int num);

	void updateAllData();

	//拾取物品
	void packupItem(Item* item);
	//开门
	bool openDoor(Door* door);
	//怪物战斗
	bool enemyCombat(Enemy* enemy);
public:
	int m_attackNum = 0;			//攻   击
	int m_defenseNum = 0;		//防   御
	int m_lifeNum = 0;				//生   命
	int m_yellowKeyNum = 0;	//黄钥匙
	int m_blueKeyNum = 0;		//蓝钥匙
	int m_redKeyNum = 0;		//红钥匙
	int m_curTier = -1;		//当前所在层
};
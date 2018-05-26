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

	//ʰȡ��Ʒ
	void packupItem(Item* item);
	//����
	bool openDoor(Door* door);
	//����ս��
	bool enemyCombat(Enemy* enemy);
public:
	int m_attackNum = 0;			//��   ��
	int m_defenseNum = 0;		//��   ��
	int m_lifeNum = 0;				//��   ��
	int m_yellowKeyNum = 0;	//��Կ��
	int m_blueKeyNum = 0;		//��Կ��
	int m_redKeyNum = 0;		//��Կ��
	int m_curTier = -1;		//��ǰ���ڲ�
};
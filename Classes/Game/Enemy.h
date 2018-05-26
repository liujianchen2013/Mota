#pragma once
#include "cocos2d.h"

class Enemy : public cocos2d::Sprite
{
public:
	static Enemy* createEnemyWithFrameName(const std::string frameName);
	int m_id;
	int m_attack;
	int m_defense;
	int m_life;
	int m_index;
};
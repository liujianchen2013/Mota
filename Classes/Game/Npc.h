#pragma once
#include "cocos2d.h"

class Npc : public cocos2d::Sprite
{
public:
	static Npc* createNpcWithFrameName(const std::string frameName);
	int m_id;
	int m_index;
};
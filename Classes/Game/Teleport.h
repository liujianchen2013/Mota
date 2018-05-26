#pragma once
#include "cocos2d.h"
#include "Tool/GameDefine.h"

class Teleport : public cocos2d::Sprite
{
public:
	static Teleport* createTeleportWithFrameName(const std::string frameName);
	static Teleport* createTeleportWithFile(const std::string fileName);
	int m_id;//1- 下楼， 2 - 上楼
	Direction m_heroDirection;//英雄的方向
	int m_targetNum;//目标楼层
	int m_index;
};
#pragma once
#include "cocos2d.h"
#include "Tool/GameDefine.h"

class Door : public cocos2d::Sprite
{
public:
	static Door* createDoorWithFrameName(const std::string frameName);
	int m_id;
	ItemType m_expendType;	//消耗物品的类型
	int m_expendNum;				//消耗物品的数量
	int m_index;

};
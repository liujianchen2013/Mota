#pragma once
#include "cocos2d.h"
#include "Tool/GameDefine.h"

class Item : public cocos2d::Sprite
{
public:
	static Item* createArticlesWithFrameName(const std::string frameName);
	int m_id;
	ItemType m_type;//物品加强属性类型
	int m_addNum;//增强数量
	int m_index;
	//defense
	//attack
	//life
};
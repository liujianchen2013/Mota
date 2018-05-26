#pragma once
#include "cocos2d.h"
#include "Tool/GameDefine.h"

class Item : public cocos2d::Sprite
{
public:
	static Item* createArticlesWithFrameName(const std::string frameName);
	int m_id;
	ItemType m_type;//��Ʒ��ǿ��������
	int m_addNum;//��ǿ����
	int m_index;
	//defense
	//attack
	//life
};
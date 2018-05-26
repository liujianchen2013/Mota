#pragma once
#include "cocos2d.h"
#include "Tool/GameDefine.h"

class Door : public cocos2d::Sprite
{
public:
	static Door* createDoorWithFrameName(const std::string frameName);
	int m_id;
	ItemType m_expendType;	//������Ʒ������
	int m_expendNum;				//������Ʒ������
	int m_index;

};
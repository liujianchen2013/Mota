#pragma once
#include "cocos2d.h"
#include "Tool/GameDefine.h"

class Teleport : public cocos2d::Sprite
{
public:
	static Teleport* createTeleportWithFrameName(const std::string frameName);
	static Teleport* createTeleportWithFile(const std::string fileName);
	int m_id;//1- ��¥�� 2 - ��¥
	Direction m_heroDirection;//Ӣ�۵ķ���
	int m_targetNum;//Ŀ��¥��
	int m_index;
};
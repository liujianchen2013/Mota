#pragma once
#include "cocos2d.h"
#include "Game/Hero.h"

class ShowLayer : public cocos2d::Layer
{
public:
	static ShowLayer* Instance();

	virtual bool init();

	CREATE_FUNC(ShowLayer);

public:
	void setAttackNum(int num);
	void setDefenseNum(int num);
	void setLifeNum(int num);
	void setYellowKeyNum(int num);
	void setBlueKeyNum(int num);
	void setRedKeyNum(int num);
	void setCurTier(int tier);
private:
	ShowLayer();
	cocos2d::Label* m_attackNum;
	cocos2d::Label* m_defenseNum;
	cocos2d::Label* m_lifeNum;
	cocos2d::Label* m_yellowKeyNum;
	cocos2d::Label* m_blueKeyNum;
	cocos2d::Label* m_redKeyNum;
	cocos2d::Label* m_curTier;
	static ShowLayer* createLayer();
	static ShowLayer* m_showLayer;
};

#define gShowLayer ShowLayer::Instance()
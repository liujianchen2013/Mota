#pragma once
#include "cocos2d.h"
#include "Layer/GameLayer.h"
USING_NS_CC;

class MainScene : Scene
{
public:
	~MainScene();
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MainScene);
};
#pragma once
#include "cocos2d.h"
#include "Game/GameMap.h"

class GameLayer : public cocos2d::Layer
{
public:
	~GameLayer();
	static GameLayer* createLayer();
	virtual void update(float delta);
	void autoMove(float delta);

	void showTip(std::string str, cocos2d::Point point, cocos2d::Color3B color = cocos2d::Color3B::RED);

	virtual bool init();

	CREATE_FUNC(GameLayer);

	//切换地图
	void switchMap(int tagetNum);

	//保存所有已加载地图
	cocos2d::Map<int, GameMap*> m_gameMaps;
	//当前地图
	GameMap* m_curGameMap;
	Hero* m_hero;

private:
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	//void onMouseDown(cocos2d::Event *event);
	//void onMouseUp(cocos2d::Event *event);
	//void onMouseMove(cocos2d::Event *event);
	//void onMouseScroll(cocos2d::Event *event);
private:
	bool m_isPressed = false;//按键是否被按下
	Direction m_curDirection = DIR_UP;//Hero当前移动方向

};
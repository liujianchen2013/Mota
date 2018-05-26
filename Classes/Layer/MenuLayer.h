#ifndef __MENULAYER_H__
#define __MENULAYER_H__
#include "cocos2d.h"

class MenuLayer : public cocos2d::LayerColor
{
public:
	static MenuLayer* Instance();
	virtual bool init();
	cocos2d::MenuItemSprite* createMenuItem(std::string fileName, const cocos2d::ccMenuCallback& callback);
	void setMenuVisible(bool state);

	CREATE_FUNC(MenuLayer);
	bool m_isStart = false;

private:
	static MenuLayer* m_menuLayer;
	static MenuLayer* createLayer();
	void menuStartGameCallback(Ref* sender);
	void menuLoadGameCallback(Ref* sender);
	void menuSaveGameCallback(Ref* sender);
	void menuResumeGameCallback(Ref* sender);
	void menuExitGameCallback(Ref* sender);

protected:
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

private:
	cocos2d::MenuItemSprite * m_startItem = nullptr;
	cocos2d::MenuItemSprite* m_loadItem = nullptr;
	cocos2d::MenuItemSprite* m_saveItem = nullptr;
	cocos2d::MenuItemSprite* m_exitItem = nullptr;
	cocos2d::MenuItemSprite* m_resumeItem = nullptr;
};

#define gMenuLayer MenuLayer::Instance()


#endif //__MENULAYER_H__
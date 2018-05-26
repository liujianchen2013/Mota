#include "MenuLayer.h"
#include "Tool/GameSave.h"
#include "Layer/popupLayer.h"
#include "Config/EnToCh.h"
USING_NS_CC;

MenuLayer* MenuLayer::m_menuLayer = nullptr;

MenuLayer * MenuLayer::Instance()
{
	if (!m_menuLayer)
	{
		m_menuLayer = MenuLayer::createLayer();
	}
	return m_menuLayer;
}

MenuLayer * MenuLayer::createLayer()
{
	return MenuLayer::create();
}

void MenuLayer::menuStartGameCallback(Ref * sender)
{
	// 定义一个弹出层，传入一张背景图
	PopupLayer* pl = PopupLayer::create("images/bg_1.png");
	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩
	pl->setContentSize(Size(400, 260));
	pl->setTitle(en2ch("Warning"));
	pl->setContentText(en2ch("Click ok to overwrite the archive. Are you sure you want to restart the game?"), 20, 30, 140);
	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
	// 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数
	pl->setCallbackFunc([this](Ref* sender) {
		if (sender)
		{
			MenuItem* button = dynamic_cast<MenuItem*>(sender);
			if (button->getTag() == 0)//确定
			{
				setMenuVisible(false);
				gGameSave.saveData();
				m_isStart = true;
			}
		}
	});
	// 添加按钮，设置图片，文字，tag 信息
	pl->addButton("images/menu/ok.png", "images/menu/ok.png", "", 0);
	pl->addButton("images/menu/cancle.png", "images/menu/cancle.png", "", 1);
	// 添加到当前层
	this->addChild(pl,1);
}

void MenuLayer::menuLoadGameCallback(Ref * sender)
{
	if (m_isStart)
	{
		// 定义一个弹出层，传入一张背景图
		PopupLayer* pl = PopupLayer::create("images/bg_1.png");
		// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩
		pl->setContentSize(Size(400, 260));
		pl->setTitle(en2ch("Warning"));
		pl->setContentText(en2ch("Are you sure you want to give up the current game?"), 20, 30, 140);
		// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
		// 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数
		pl->setCallbackFunc([this](Ref* sender) {
			if (sender)
			{
				MenuItem* button = dynamic_cast<MenuItem*>(sender);
				if (button->getTag() == 0)//确定
				{
					gGameSave.readData();
					setMenuVisible(false);
					m_isStart = true;
				}
			}
		});
		// 添加按钮，设置图片，文字，tag 信息
		pl->addButton("images/menu/ok.png", "images/menu/ok.png", "", 0);
		pl->addButton("images/menu/cancle.png", "images/menu/cancle.png", "", 1);
		// 添加到当前层
		this->addChild(pl, 1);
	}
	else
	{
		gGameSave.readData();
		setMenuVisible(false);
		m_isStart = true;
	}
}

void MenuLayer::menuSaveGameCallback(Ref * sender)
{
	// 定义一个弹出层，传入一张背景图
	PopupLayer* pl = PopupLayer::create("images/bg_1.png");
	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩
	pl->setContentSize(Size(400, 260));
	pl->setTitle(en2ch("Warning"));
	pl->setContentText(en2ch("Are you sure you want to overwrite the original archive with the current archive?"), 20, 30, 140);
	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
	// 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数
	pl->setCallbackFunc([this](Ref* sender) {
		if (sender)
		{
			MenuItem* button = dynamic_cast<MenuItem*>(sender);
			if (button->getTag() == 0)//确定
			{
				gGameSave.saveData();
				setMenuVisible(false);
			}
		}
	});
	// 添加按钮，设置图片，文字，tag 信息
	pl->addButton("images/menu/ok.png", "images/menu/ok.png", "", 0);
	pl->addButton("images/menu/cancle.png", "images/menu/cancle.png", "", 1);
	// 添加到当前层
	this->addChild(pl, 1);
}

void MenuLayer::menuResumeGameCallback(Ref * sender)
{
	setMenuVisible(false);
	m_isStart = true;
}

void MenuLayer::menuExitGameCallback(Ref * sender)
{
	if (m_isStart)
	{
		// 定义一个弹出层，传入一张背景图
		PopupLayer* pl = PopupLayer::create("images/bg_1.png");
		// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩
		pl->setContentSize(Size(400, 260));
		pl->setTitle(en2ch("Warning"));
		pl->setContentText(en2ch("Is the current progress saved?"), 20, 30, 140);
		// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
		// 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数
		pl->setCallbackFunc([this](Ref* sender) {
			if (sender)
			{
				MenuItem* button = dynamic_cast<MenuItem*>(sender);
				if (button->getTag() == 0)//确定
				{
					gGameSave.saveData();
				}
			}
			Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
		});
		// 添加按钮，设置图片，文字，tag 信息
		pl->addButton("images/menu/ok.png", "images/menu/ok.png", "", 0);
		pl->addButton("images/menu/cancle.png", "images/menu/cancle.png", "", 1);
		// 添加到当前层
		this->addChild(pl, 1);
	}
	else
	{
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
}

void MenuLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	
}

void MenuLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
}

void MenuLayer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

bool MenuLayer::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	return true;
}

bool MenuLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0,0,0,200)))
	{
		return false;
	}
	auto spriteFrameChache = SpriteFrameCache::getInstance();

	Size visibleSize = Director::sharedDirector()->getVisibleSize();
	this->setContentSize(visibleSize);

	m_startItem = createMenuItem("images/menu/gameStart", CC_CALLBACK_1(MenuLayer::menuStartGameCallback, this));
	m_startItem->setPosition(Vec2(0,120));
	m_loadItem = createMenuItem("images/menu/loadGame", CC_CALLBACK_1(MenuLayer::menuLoadGameCallback, this));
	m_loadItem->setPosition(Vec2(0, 40));
	m_saveItem = createMenuItem("images/menu/saveGame", CC_CALLBACK_1(MenuLayer::menuSaveGameCallback, this));
	m_saveItem->setPosition(Vec2(0, -40));
	m_exitItem = createMenuItem("images/menu/exitGame", CC_CALLBACK_1(MenuLayer::menuExitGameCallback, this));
	m_exitItem->setPosition(Vec2(0, -120));
	m_resumeItem = createMenuItem("images/menu/resumeGame", CC_CALLBACK_1(MenuLayer::menuResumeGameCallback, this));
	m_resumeItem->setPosition(Vec2(0, 120));

	Menu* menu = Menu::create();
	menu->addChild(m_resumeItem);
	menu->addChild(m_startItem);
	menu->addChild(m_loadItem);
	menu->addChild(m_saveItem);
	menu->addChild(m_exitItem);
	this->addChild(menu, 1);

	setVisible(false);
	setKeyboardEnabled(false);
	setTouchEnabled(false);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	return true;
}

MenuItemSprite * MenuLayer::createMenuItem(std::string fileName, const ccMenuCallback & callback)
{
	SpriteFrame* startFrame = SpriteFrame::create(fileName + ".png", Rect(59, 68, 162, 60));
	SpriteFrame* disFrame = SpriteFrame::create(fileName + "_dis.png", Rect(59, 68, 162, 60));
	Sprite* startNormal = Sprite::createWithSpriteFrame(startFrame);
	Sprite* startSelect = Sprite::createWithSpriteFrame(startFrame);
	Sprite* startDis = Sprite::createWithSpriteFrame(disFrame);
	startSelect->setScale(0.9);
	auto dSize = startNormal->getContentSize() - startSelect->getContentSize() * startSelect->getScale();
	startSelect->setPosition(dSize.width / 2, dSize.height / 2);
	return MenuItemSprite::create(startNormal, startSelect, startDis, callback);
}

void MenuLayer::setMenuVisible(bool state)
{
	setVisible(state);
	setKeyboardEnabled(state);
	setTouchEnabled(state);
	m_saveItem->setEnabled(m_isStart);

	m_resumeItem->setVisible(m_isStart);
	m_startItem->setVisible(!m_isStart);
	if (state)
	{
		Director::getInstance()->pause();
	}
	else
	{
		Director::getInstance()->resume();
	}
}

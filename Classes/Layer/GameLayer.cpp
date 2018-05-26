#include "GameLayer.h"
#include "Game/GameMap.h"
#include "Tool/GameSave.h"
#include "Layer/MenuLayer.h"
#include "Layer/ShowLayer.h"
USING_NS_CC;

GameLayer::~GameLayer()
{

}

GameLayer * GameLayer::createLayer()
{
	return GameLayer::create();
}

void GameLayer::update(float delta)
{
	Node::update(0);
	if (m_isPressed)
	{
		m_hero->move(m_curDirection);
	}
}

void GameLayer::autoMove(float delta)
{
	if (m_hero->m_movePath.empty())
	{
		unschedule(schedule_selector(GameLayer::autoMove));
		return;
	}
	Direction dir = m_hero->m_movePath.back();
	m_hero->m_movePath.pop_back();
	m_hero->move(dir);
}

void GameLayer::showTip(std::string str, cocos2d::Point point,Color3B color)
{
	auto label = Label::createWithTTF(str, "fonts/LKSS_HT.TTF", 18);
	if (label)
	{
		label->setColor(color);
		label->setPosition(point + Point(16, 16));
		this->addChild(label);
		auto moveBy = MoveBy::create(3, Point(0, 32));
		auto fadeOut = FadeOut::create(3.0f);
		auto callbackLabel = CallFunc::create([label]() {
			label->removeFromParentAndCleanup(true);
		});
		auto spawnAction = Spawn::create(moveBy, fadeOut, nullptr);
		auto seq = Sequence::create(spawnAction, callbackLabel, nullptr);
		label->runAction(seq);
	}
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/enemy.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/door.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/item.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/wall01.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/npc01.plist");

	int number = 1;
	std::string tmxFileName = "Map/" + Value(number).asString() + "/" + Value(number).asString() + ".tmx";
	GameMap* gameMap = GameMap::createGameMapWithTMXFile(tmxFileName);
	//SpriteFrameCache::getInstance()->removeSpriteFrames();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/hero.plist");
	m_hero = Hero::createHeroWithFrameName("hero4_1.png");
	if (gameMap && m_hero)
	{
		gameMap->initObject();
		gameMap->initAllNode();
		gameMap->m_tier = number;
		setContentSize(gameMap->getContentSize());
		m_hero->m_gameLayer = this;
		//初始参数
		m_hero->changeLifeNum(400);
		m_hero->changeYellowKeyNum(0);
		m_hero->changeAttackNum(10);
		m_hero->changeDefenseNum(10);
		m_hero->changeBlueKeyNum(0);
		m_hero->changeRedKeyNum(0);
		gameMap->setPosition(0, 0);
		gameMap->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		addChild(gameMap,DO_MAP);
		m_gameMaps.insert(number, gameMap);
		m_curGameMap = gameMap;

		//����Ӣ��λ��
		m_hero->setPosition(m_curGameMap->m_heroCurrPos);
		//m_hero->setPosition(0,32);
		m_hero->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		addChild(m_hero,DO_HERO);

		setKeyboardEnabled(true);
		setTouchEnabled(true);
		setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
		//auto _mouseListener = EventListenerMouse::create();
		//_mouseListener->onMouseMove = CC_CALLBACK_1(GameLayer::onMouseMove, this);
		//_mouseListener->onMouseUp = CC_CALLBACK_1(GameLayer::onMouseUp, this);
		//_mouseListener->onMouseDown = CC_CALLBACK_1(GameLayer::onMouseDown, this);
		//_mouseListener->onMouseScroll = CC_CALLBACK_1(GameLayer::onMouseScroll, this);
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
		//auto listener1 = EventListenerTouchOneByOne::create();
		//listener1->onTouchBegan = [](Touch* touch, Event* event) {
		//	// your code
		//	return true; // if you are consuming it
		//};
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
		return true;
	}
	return false;
}

void GameLayer::switchMap(int tagetNum)
{
	cocos2d::Map<int, GameMap*>::iterator item = m_gameMaps.find(tagetNum);
	if (item != m_gameMaps.end())
	{
		if (m_curGameMap)
		{
			//��ͼ�Ѿ����أ�ֱ���л�
			m_curGameMap->removeFromParentAndCleanup(false);
		}
		m_curGameMap = item->second;
		this->addChild(m_curGameMap,DO_MAP);
	}
	else
	{
		//��һ�μ���
		std::string tmxFileName = "Map/" + Value(tagetNum).asString() + "/" + Value(tagetNum).asString() + ".tmx";
		GameMap* gameMap = GameMap::createGameMapWithTMXFile(tmxFileName);
		if (gameMap)
		{
			gameMap->initObject();
			gameMap->initAllNode();
			gameMap->m_tier = tagetNum;
			auto visibleSize = Director::getInstance()->getVisibleSize();
			gameMap->setPosition(0, 0);
			gameMap->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
			if (m_curGameMap)
			{
				m_curGameMap->removeFromParentAndCleanup(false);
			}
			addChild(gameMap,DO_MAP);
			m_gameMaps.insert(tagetNum, gameMap);
			m_curGameMap = gameMap;
		}
		else
		{
			return;
		}
	}
	//����Ӣ��λ��
	m_hero->setPosition(m_curGameMap->m_heroCurrPos);
	m_hero->ChangeHeroDirection(m_curGameMap->m_heroDirection);
	gShowLayer->setCurTier(tagetNum);
}

void GameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	Direction curDirection = DIR_NONE;
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		curDirection = DIR_LEFT;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		curDirection = DIR_RIGHT;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		curDirection = DIR_UP;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		curDirection = DIR_DOWN;
		break;
	case EventKeyboard::KeyCode::KEY_ESCAPE://手机的返回键
		gMenuLayer->setMenuVisible(true);
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		gMenuLayer->setMenuVisible(true);
		break;
	}
	if(curDirection != DIR_NONE)
	{
		m_curDirection = curDirection;
		m_hero->move(m_curDirection);
		m_isPressed = true;
		schedule(schedule_selector(GameLayer::update), 0.05f, CC_REPEAT_FOREVER, 0.5f);
	}
}

void GameLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	m_isPressed = false;
	unschedule(schedule_selector(GameLayer::update));
}

void GameLayer::onTouchEnded(Touch * touch, Event * unused_event)
{
}

bool GameLayer::onTouchBegan(Touch * touch, Event * unused_event)
{
	m_hero->m_movePath.clear();
	unschedule(schedule_selector(GameLayer::autoMove));
	auto pos = this->convertToWorldSpace(Vec2(0, 0));
	auto touchPos = touch->getLocation();
	auto pos2 = this->getPosition();
	Point touchPoint = touchPos - pos;
	touchPoint.y -= m_curGameMap->getTileSize().width;
	Point endPoint = m_curGameMap->tileCoordForPosition(touchPoint);
	Point beginPoint = m_curGameMap->tileCoordForPosition(m_hero->getPosition());
	if (endPoint.x > 0 && endPoint.x <= 12 && endPoint.y > 0 && endPoint.y < 12)
	{
		std::vector<cocos2d::Point> path = m_curGameMap->m_autoFindPath.findShortPath(beginPoint, endPoint);
		if (path.size())
		{
			std::vector<Direction> movePath;
			path.pop_back();
			Point curPoint = beginPoint;
			for(int i = path.size() - 1; i >= 0; --i)
			{
				Point targetPoint = path[i];
				Point offset = targetPoint - curPoint;
				if (offset.y == 0 && offset.x < 0)
				{
					movePath.push_back(Direction::DIR_LEFT);
				}
				else if(offset.y == 0 && offset.x > 0)
				{
					movePath.push_back(Direction::DIR_RIGHT);
				}
				else if (offset.y > 0 && offset.x == 0)
				{
					movePath.push_back(Direction::DIR_DOWN);
				}
				else if (offset.y < 0 && offset.x == 0)
				{
					movePath.push_back(Direction::DIR_UP);
				}
				curPoint = targetPoint;
			}
			//����
			for (auto it = movePath.rbegin(); it != movePath.rend(); ++it)
			{
				m_hero->m_movePath.push_back(*it);
			}

			schedule(schedule_selector(GameLayer::autoMove), 0.05f, CC_REPEAT_FOREVER, 0);
		}
	}
	return true;
}

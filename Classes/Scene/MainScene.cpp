#include "MainScene.h"
#include "Layer/ShowLayer.h"
#include "Config/Config.h"
#include "ui/UILayout.h"
#include "Tool/GameSave.h"
#include "Layer/MenuLayer.h"
#include "Config/EnToCh.h"
#include "SimpleAudioEngine.h"

MainScene::~MainScene()
{
	//Ïú»ÙÉùÒô»º´æ
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->unloadEffect("sounds/kaimen.mp3");
}

cocos2d::Scene * MainScene::createScene()
{
	return MainScene::create();
}

bool MainScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//³õÊ¼»¯ÅäÖÃÎÄ¼ş
	gGameConfig.initConfig("Config/Config.json");
	gEnToCh.init("Config/EnToCh.json");
	//³õÊ¼»¯ÉùÒôÎÄ¼ş
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("sounds/kaimen.mp3");
	audio->preloadEffect("sounds/jianji.mp3");
	audio->preloadEffect("sounds/huodewupin.mp3");
	audio->preloadEffect("sounds/cuowu.mp3");
	audio->preloadEffect("sounds/queding.mp3");
	audio->preloadEffect("sounds/quxiao.mp3");
	audio->preloadEffect("sounds/shangxialou.mp3");
	audio->preloadEffect("sounds/xuanze.mp3");
	audio->preloadEffect("sounds/xueping.mp3");
	//audio->preloadBackgroundMusic("sounds/bgm.wma");
	audio->playBackgroundMusic("sounds/bgm.wma");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	GameLayer* gameLayer = GameLayer::createLayer();
	gGameSave.init(gameLayer);
	ShowLayer* showLayer = gShowLayer;
	int layoutWidth = gameLayer->getContentSize().width + showLayer->getContentSize().width;
	int layoutHeight = gameLayer->getContentSize().height;

	ui::Layout *layout1 = ui::Layout::create();
	layout1->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	layout1->setBackGroundColor(Color3B::RED); 
	layout1->setContentSize(Size(layoutWidth, layoutHeight));
	layout1->setLayoutType(ui::Layout::Type::HORIZONTAL);
	layout1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	layout1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	layout1->addChild(showLayer);
	layout1->addChild(gameLayer);
	gameLayer->setPosition(showLayer->getContentSize().width, showLayer->getPositionY());
	addChild(layout1);
	gMenuLayer->setMenuVisible(true);
	addChild(gMenuLayer);
	return true;
}

void MainScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();
}

#include "MainScene.h"
#include "Layer/ShowLayer.h"
#include "Config/Config.h"
#include "ui/UILayout.h"
#include "Tool/GameSave.h"
#include "Layer/MenuLayer.h"
#include "Config/EnToCh.h"

MainScene::~MainScene()
{

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
	//初始化配置文件
	gGameConfig.initConfig("Config/Config.json");
	gEnToCh.init("Config/EnToCh.json");

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

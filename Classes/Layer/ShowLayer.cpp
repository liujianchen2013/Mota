#include "ShowLayer.h"
#include "ui/UILayout.h"
#include "Config/EnToCh.h"
#include <functional>
USING_NS_CC;

ShowLayer* ShowLayer::m_showLayer = nullptr;

ShowLayer::ShowLayer()
{

}

ShowLayer * ShowLayer::createLayer()
{
	 return ShowLayer::create();
}

ShowLayer * ShowLayer::Instance()
{
	if (!m_showLayer)
	{
		m_showLayer = ShowLayer::createLayer();
	}
	return m_showLayer;
}

bool ShowLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Sprite* sprite = Sprite::create("images/showLayer.png");
	setContentSize(sprite->getContentSize());
	addChild(sprite);
	sprite->setPosition(0, 0);
	sprite->setAnchorPoint(Point::ZERO);

	ui::Layout* layout = ui::Layout::create();//创建一个布局
	//layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	//layout->setBackGroundColor(Color3B::GREEN);
	layout->setContentSize(Size(144, 352));
	layout->setPosition(Point(32, 32));
	layout->setAnchorPoint(Point::ZERO);
	layout->setLayoutType(ui::Layout::Type::VERTICAL);
	this->addChild(layout,1);

	{
		//TTFConfig labelConfig;
		//labelConfig.fontFilePath = "fonts/arial.ttf";
		//labelConfig.fontSize = 16;
		//labelConfig.glyphs = GlyphCollection::DYNAMIC;
		//labelConfig.outlineSize = 0;
		//labelConfig.customGlyphs = nullptr;
		//labelConfig.distanceFieldEnabled = false;

		//Label* attack_label = Label::createWithTTF(labelConfig, "Attack: ");

		TTFConfig ttfConfig;
		ttfConfig.fontFilePath = "fonts/LKSS_HT.TTF";
		ttfConfig.fontSize = 20;
		ttfConfig.bold = true;

		Label* attack_label = Label::createWithTTF(ttfConfig,en2ch("Attack"));
		attack_label->setColor(Color3B::RED);
		Label* defense_label = Label::createWithTTF(ttfConfig,en2ch("Defense"));
		defense_label->setColor(Color3B::BLUE);
		Label* life_label = Label::createWithTTF(ttfConfig,en2ch("Life"));
		life_label->setColor(Color3B::GREEN);
		Label* key1_label = Label::createWithTTF(ttfConfig,en2ch("YellowKey"));
		key1_label->setColor(Color3B::YELLOW);
		Label* key2_label = Label::createWithTTF(ttfConfig,en2ch("BlueKey"));
		key2_label->setColor(Color3B::BLUE);
		Label* key3_label = Label::createWithTTF(ttfConfig,en2ch("RedKey"));
		key3_label->setColor(Color3B::RED);
		Label* f_label = Label::createWithTTF("F", ttfConfig.fontFilePath, 60);
		f_label->setColor(Color3B::WHITE);

		m_attackNum = Label::createWithTTF(ttfConfig,"0");
		m_attackNum->setColor(Color3B::RED);
		m_defenseNum = Label::createWithTTF(ttfConfig,"0");
		m_defenseNum->setColor(Color3B::BLUE);
		m_lifeNum = Label::createWithTTF(ttfConfig,"0");
		m_lifeNum->setColor(Color3B::GREEN);
		m_yellowKeyNum = Label::createWithTTF(ttfConfig,"0");
		m_yellowKeyNum->setColor(Color3B::YELLOW);
		m_blueKeyNum = Label::createWithTTF(ttfConfig,"0");
		m_blueKeyNum->setColor(Color3B::BLUE);
		m_redKeyNum = Label::createWithTTF(ttfConfig,"0");
		m_redKeyNum->setColor(Color3B::RED);
		m_curTier = Label::createWithTTF("1", ttfConfig.fontFilePath, 40);
		m_curTier->setColor(Color3B::WHITE);


		//ui::LinearLayoutParameter *linerP1 = ui::LinearLayoutParameter::create();
		//linerP1->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);//水平居中
		//attack_label->setLayoutParameter(linerP1);
		layout->addChild(f_label);
		layout->addChild(m_curTier);
		layout->addChild(attack_label);
		layout->addChild(defense_label);
		layout->addChild(life_label);
		layout->addChild(key1_label);
		layout->addChild(key2_label);
		layout->addChild(key3_label);
		layout->addChild(m_attackNum);
		layout->addChild(m_defenseNum);
		layout->addChild(m_lifeNum);
		layout->addChild(m_yellowKeyNum);
		layout->addChild(m_blueKeyNum);
		layout->addChild(m_redKeyNum);

		f_label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		f_label->setPosition(Point(30, 280));
		m_curTier->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		m_curTier->setPosition(Point(70, 280));

		life_label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		life_label->setPosition(Point(6,240));
		m_lifeNum->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		m_lifeNum->setPosition(Point(76, 239));

		attack_label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		attack_label->setPosition(Point(life_label->getPositionX(), life_label->getPositionY() - 30));
		m_attackNum->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		m_attackNum->setPosition(Point(m_lifeNum->getPositionX(), m_lifeNum->getPositionY() - 30));

		defense_label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		defense_label->setPosition(Point(life_label->getPositionX(), life_label->getPositionY() - 60));
		m_defenseNum->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		m_defenseNum->setPosition(Point(m_lifeNum->getPositionX(), m_lifeNum->getPositionY() - 60));

		key1_label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		key1_label->setPosition(Point(life_label->getPositionX(), life_label->getPositionY() - 90));
		m_yellowKeyNum->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		m_yellowKeyNum->setPosition(Point(m_lifeNum->getPositionX(), m_lifeNum->getPositionY() - 90));

		key2_label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		key2_label->setPosition(Point(life_label->getPositionX(), life_label->getPositionY() - 120));
		m_blueKeyNum->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		m_blueKeyNum->setPosition(Point(m_lifeNum->getPositionX(), m_lifeNum->getPositionY() - 120));

		key3_label->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		key3_label->setPosition(Point(life_label->getPositionX(), life_label->getPositionY() - 150));
		m_redKeyNum->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		m_redKeyNum->setPosition(Point(m_lifeNum->getPositionX(), m_lifeNum->getPositionY() - 150));
		
	}
	return true;
}

void ShowLayer::setAttackNum(int num)
{
	m_attackNum->setString(Value(num).asString());
}

void ShowLayer::setDefenseNum(int num)
{
	m_defenseNum->setString(Value(num).asString());
}

void ShowLayer::setLifeNum(int num)
{
	m_lifeNum->setString(Value(num).asString());
}

void ShowLayer::setYellowKeyNum(int num)
{
	m_yellowKeyNum->setString(Value(num).asString());
}

void ShowLayer::setBlueKeyNum(int num)
{
	m_blueKeyNum->setString(Value(num).asString());
}

void ShowLayer::setRedKeyNum(int num)
{
	m_redKeyNum->setString(Value(num).asString());
}

void ShowLayer::setCurTier(int tier)
{
	m_curTier->setString(Value(tier).asString());
}

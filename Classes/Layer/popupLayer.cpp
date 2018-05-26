#include "popupLayer.h"
USING_NS_CC;

PopupLayer::PopupLayer() :
	m__pMenu(NULL)
	, m_contentPadding(0)
	, m_contentPaddingTop(0)
	, m_callback(NULL)
	, m__sfBackGround(NULL)
	, m__s9BackGround(NULL)
	, m__ltContentText(NULL)
	, m__ltTitle(NULL)
{

}

PopupLayer::~PopupLayer()
{
	CC_SAFE_RELEASE(m__pMenu);
	CC_SAFE_RELEASE(m__sfBackGround);
	CC_SAFE_RELEASE(m__ltContentText);
	CC_SAFE_RELEASE(m__ltTitle);
	CC_SAFE_RELEASE(m__s9BackGround);
}

bool PopupLayer::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		this->setContentSize( Size::ZERO);
		// 初始化需要的 Menu
		Menu* menu = Menu::create();
		menu->setPosition(Point::ZERO);
		setMenuButton(menu);
		setTouchEnabled(true);
		bRef = true;
	} while (0);
	return bRef;
}

bool PopupLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	log("PopupLayer touch");
	return true;
}


PopupLayer* PopupLayer::create(const std::string& backgroundImage)
{
	PopupLayer* ml = PopupLayer::create();
	ml->setSpriteBackGround(Sprite::create(backgroundImage));
	ml->setSprite9BackGround(Scale9Sprite::create(backgroundImage));
	return ml;
}


void PopupLayer::setTitle(const std::string& title, int fontsize)
{
	LabelTTF* ltfTitle = LabelTTF::create(title, "fonts/LKSS_HT.TTF", fontsize);
	setLabelTitle(ltfTitle);
}

void PopupLayer::setContentText(const std::string& text, int fontsize, int padding, int paddingTop) {
	LabelTTF* ltf = LabelTTF::create(text, "fonts/LKSS_HT.TTF", fontsize);
	setLabelContentText(ltf);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}

void PopupLayer::setCallbackFunc(const cocos2d::ccMenuCallback& callback)
{
	m_callback = callback;
}


bool PopupLayer::addButton(const std::string& normalImage, const std::string& selectedImage, const std::string& title, int tag) {
	Size winSize = Director::sharedDirector()->getWinSize();
	Point pCenter = Vec2(winSize.width / 2, winSize.height / 2);

	// 创建图片菜单按钮
	Sprite* spriteNormal = Sprite::create(normalImage);
	Sprite* spriteSelect = Sprite::create(selectedImage);
	spriteSelect->setScale(0.9);
	auto dSize = spriteNormal->getContentSize() - spriteSelect->getContentSize() * spriteSelect->getScale();
	spriteSelect->setPosition(dSize.width / 2, dSize.height / 2);

	MenuItemSprite* menuImage = MenuItemSprite::create(spriteNormal, spriteSelect, CC_CALLBACK_1(PopupLayer::buttonCallback,this));
	menuImage->setTag(tag);
	menuImage->setPosition(pCenter);

	// 添加文字说明并设置位置
	Size imenu = menuImage->getContentSize();
	LabelTTF* ttf = LabelTTF::create(title, "", 20);
	ttf->setColor(Color3B(0, 0, 0));
	ttf->setPosition(Vec2(imenu.width / 2, imenu.height / 2));
	menuImage->addChild(ttf);

	getMenuButton()->addChild(menuImage);
	return true;
}


void PopupLayer::buttonCallback(Ref *pSender) {
	if (m_callback)
	{
		m_callback(pSender);
	}
	this->removeFromParentAndCleanup(true);
}



void PopupLayer::onEnter()
{
	Layer::onEnter();

	Size winSize = Director::sharedDirector()->getWinSize();
	Point pCenter = Vec2(winSize.width / 2, winSize.height / 2);

	Size contentSize;
	// 设定好参数，在运行时加载
	if (getContentSize().equals( Size::ZERO))
	{
		getSpriteBackGround()->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		this->addChild(getSpriteBackGround(), 0, 0);
		contentSize = getSpriteBackGround()->getTexture()->getContentSize();
	}
	else
	{
		Scale9Sprite *background = getSprite9BackGround();
		background->setContentSize(getContentSize());
		background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		this->addChild(background, 0);
		contentSize = getContentSize();
	}


	// 添加按钮，并设置其位置
	this->addChild(getMenuButton());
	float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount() + 1);

	auto childr = getMenuButton()->getChildren();
	int i = 0;
	for(auto& pObj : childr)
	{
		pObj->setPosition(Vec2(winSize.width / 2 - contentSize.width / 2 + btnWidth * (i + 1), winSize.height / 2 - contentSize.height / 3));
		i++;
	}

	// 显示对话框标题
	if (getLabelTitle())
	{
		getLabelTitle()->setPosition(pCenter + Vec2(0, contentSize.height / 2 - 45.0f));
		this->addChild(getLabelTitle());
	}

	// 显示文本内容
	if (getLabelContentText())
	{
		LabelTTF* ltf = getLabelContentText();
		ltf->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		ltf->setDimensions(Size(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
		ltf->setHorizontalAlignment(TextHAlignment::LEFT);
		this->addChild(ltf);
	}

	Action* popupLayer = Sequence::create(ScaleTo::create(0.0, 0.0),
		ScaleTo::create(0.06, 1.05),
		ScaleTo::create(0.08, 0.95),
		ScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popupLayer);

}


void PopupLayer::onExit()
{
	log("popup on exit.");
	Layer::onExit();
}
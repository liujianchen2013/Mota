#ifndef __POPUPLAYER_H__
#define __POPUPLAYER_H__
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;



class PopupLayer :public Layer
{

public:
	PopupLayer();
	~PopupLayer();

	virtual bool init();
	CREATE_FUNC(PopupLayer);

	// 重写触摸函数，永远返回 true ，屏蔽其它层，达到 “模态” 效果
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	// 构架，并设置对话框背景图片
	static PopupLayer* create(const std::string&  backgroundImage);

	// 它可以显示标题，并且设定显示文字大小
	void setTitle(const std::string& title, int fontsize = 20);
	// 文本内容，padding 为文字到对话框两边预留的距离，这是可控的，距上方的距离亦是如此
	void setContentText(const std::string& text, int fontsize = 20, int padding = 50, int paddintTop = 100);
	// 回调函数，当点击按钮后，我们关闭弹出层的同事，需要一个回调函数，以通知我们点击了哪个按钮（如果有多个）
	void setCallbackFunc(const cocos2d::ccMenuCallback& callback);
	// 为了添加按钮方面，封装了一个函数，传入些必要的参数
	bool addButton(const std::string&  normalImage, const std::string&  selectedImage, const std::string&  title, int tag = 0);

	// 为了在显示层时之前的属性生效，选择在 onEnter 里动态展示
	virtual void onEnter();
	virtual void onExit();
private:

	void buttonCallback(Ref* pSender);

	// 文字内容两边的空白区
	int m_contentPadding;
	int m_contentPaddingTop;
	cocos2d::ccMenuCallback m_callback;

	CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(LabelTTF*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(LabelTTF*, m__ltContentText, LabelContentText);

};

#endif //__POPUPLAYER_H__
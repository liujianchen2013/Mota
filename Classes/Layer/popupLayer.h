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

	// ��д������������Զ���� true �����������㣬�ﵽ ��ģ̬�� Ч��
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	// ���ܣ������öԻ��򱳾�ͼƬ
	static PopupLayer* create(const std::string&  backgroundImage);

	// ��������ʾ���⣬�����趨��ʾ���ִ�С
	void setTitle(const std::string& title, int fontsize = 20);
	// �ı����ݣ�padding Ϊ���ֵ��Ի�������Ԥ���ľ��룬���ǿɿصģ����Ϸ��ľ����������
	void setContentText(const std::string& text, int fontsize = 20, int padding = 50, int paddintTop = 100);
	// �ص��������������ť�����ǹرյ������ͬ�£���Ҫһ���ص���������֪ͨ���ǵ�����ĸ���ť������ж����
	void setCallbackFunc(const cocos2d::ccMenuCallback& callback);
	// Ϊ����Ӱ�ť���棬��װ��һ������������Щ��Ҫ�Ĳ���
	bool addButton(const std::string&  normalImage, const std::string&  selectedImage, const std::string&  title, int tag = 0);

	// Ϊ������ʾ��ʱ֮ǰ��������Ч��ѡ���� onEnter �ﶯ̬չʾ
	virtual void onEnter();
	virtual void onExit();
private:

	void buttonCallback(Ref* pSender);

	// �����������ߵĿհ���
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
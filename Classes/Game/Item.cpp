#include "Item.h"
USING_NS_CC;

Item * Item::createArticlesWithFrameName(const std::string frameName)
{
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = { 0 };
	sprintf(msg, "Invalid spriteFrameName: %s", frameName.c_str());
	CCASSERT(frame != nullptr, msg);
#endif

	Item *sprite = new (std::nothrow) Item();
	if (sprite && frame && sprite->initWithSpriteFrame(frame))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

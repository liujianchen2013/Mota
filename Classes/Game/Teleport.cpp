#include "Teleport.h"
USING_NS_CC;

Teleport * Teleport::createTeleportWithFrameName(const std::string frameName)
{
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = { 0 };
	sprintf(msg, "Invalid spriteFrameName: %s", frameName.c_str());
	CCASSERT(frame != nullptr, msg);
#endif

	Teleport *sprite = new (std::nothrow) Teleport();
	if (sprite && frame && sprite->initWithSpriteFrame(frame))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Teleport * Teleport::createTeleportWithFile(const std::string fileName)
{
	Teleport *sprite = new (std::nothrow) Teleport();
	if (sprite && sprite->initWithFile(fileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

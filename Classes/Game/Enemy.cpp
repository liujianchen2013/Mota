#include "Enemy.h"
USING_NS_CC;

Enemy * Enemy::createEnemyWithFrameName(const std::string frameName)
{
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = { 0 };
	sprintf(msg, "Invalid spriteFrameName: %s", frameName.c_str());
	CCASSERT(frame != nullptr, msg);
#endif

	Enemy *sprite = new (std::nothrow) Enemy();
	if (sprite && frame && sprite->initWithSpriteFrame(frame))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

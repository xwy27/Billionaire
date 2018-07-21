#pragma once
#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class GameScene : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	virtual void mouseCheeseCollision();

	virtual void shootMenuCallback(Ref* pSender);

	CREATE_FUNC(GameScene);

private:
	Sprite * cheese;

	Sprite* mouse;

	Sprite* stone;

	Layer* mouseLayer;

	Layer* stoneLayer;

};
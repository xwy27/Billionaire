#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MenuScene::createScene() {
	return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// sky
	auto bg_sky = Sprite::create("menu-background-sky.jpg");
	bg_sky->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 150));
	this->addChild(bg_sky, 0);
	// background
	auto bg = Sprite::create("menu-background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(bg, 0);
	// greeting
	auto greet = Sprite::create("gold-miner-text.png");
	greet->setPosition(Vec2(500 + origin.x, visibleSize.height / 2 + origin.y + 150));
	this->addChild(greet, 1);
	// miner
	auto miner = Sprite::create("menu-miner.png");
	miner->setPosition(Vec2(150 + origin.x, visibleSize.height / 2 + origin.y - 60));
	this->addChild(miner, 1);
	// miner-animation
	auto leg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
	Animate* legAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("legAnimation"));
	leg->runAction(RepeatForever::create(legAnimate));
	leg->setPosition(110 + origin.x, origin.y + 102);
	this->addChild(leg, 1);
	// gold
	auto gold = Sprite::create("menu-start-gold.png");
	gold->setPosition(Vec2(780 + origin.x, visibleSize.height / 2 + origin.y - 150));
	this->addChild(gold, 1);
	// start
	auto startItem = MenuItemImage::create(
		"start-0.png",
		"start-1.png",
		CC_CALLBACK_1(MenuScene::gameStartCallback, this));

	if (startItem == nullptr ||
		startItem->getContentSize().width <= 0 ||
		startItem->getContentSize().height <= 0) {
		problemLoading("'start-0.png' and 'start-1.png'");
	} else {
		float x = 780 + origin.x;
		float y = visibleSize.height / 2 + origin.y - 100;
		startItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void MenuScene::gameStartCallback(Ref* pSender) {

	auto gamescene = GameScene::create();
	auto transitionScene = TransitionFade::create(1.0f, gamescene);
	Director::getInstance()->replaceScene(transitionScene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

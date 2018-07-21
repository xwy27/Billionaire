#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// background
	auto level = Sprite::create("level-background-0.jpg");
	level->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(level, 0);

	this->mouseLayer = Layer::create();
	this->mouseLayer->setPosition(Vec2(0, visibleSize.height / 2 + origin.y));
	this->addChild(mouseLayer);

	this->mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	this->mouse->runAction(RepeatForever::create(mouseAnimate));
	this->mouse->setPosition(Vec2(visibleSize.width / 2 + origin.x, 0));
	this->mouseLayer->addChild(mouse, 1);

	this->stoneLayer = Layer::create();
	this->addChild(stoneLayer);

	this->stone = Sprite::create("stone.png");
	this->stone->setPosition(Vec2(560, 480));
	this->stoneLayer->addChild(stone, 1);

	auto shoot = Label::createWithTTF("Shoot", "fonts/Marker Felt.ttf", 80);
	auto shootItem = MenuItemLabel::create(shoot,
		CC_CALLBACK_1(GameScene::shootMenuCallback, this));
	if (shootItem == nullptr ||
		shootItem->getContentSize().width <= 0 ||
		shootItem->getContentSize().height <= 0) {
		problemLoading("shoot");
	} else {
		shootItem->setPosition(Vec2(800, 480));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(shootItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {
	auto location = touch->getLocation();
	cheese = Sprite::create("cheese.png");
	cheese->setPosition(mouseLayer->convertToNodeSpace(location));
	this->mouseLayer->addChild(cheese);
	mouse->runAction(MoveTo::create(1.0f, cheese->getPosition()));
	cheese->runAction(runAction(Sequence::create(DelayTime::create(1.0), FadeOut::create(3.0f), NULL)));
	return true;
}

void GameScene::shootMenuCallback(Ref* pSender) {
	auto location = this->mouse->getPosition();
	stone->runAction(MoveTo::create(0.5f, stoneLayer->convertToNodeSpace(location) + mouseLayer->getPosition()));
	stone->runAction(runAction(Sequence::create(DelayTime::create(1.0), FadeOut::create(3.0f), NULL)));

	this->stone = Sprite::create("stone.png");
	this->stone->setPosition(Vec2(560, 480));
	this->stoneLayer->addChild(this->stone, 1);

	//auto diamond = Sprite::create("diamond.png");
	auto diamond = Sprite::createWithSpriteFrameName("diamond-0.png");
	Animate* diamondAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("diamondAnimation"));
	diamond->runAction(RepeatForever::create(diamondAnimate));
	diamond->setPosition(location);
	this->mouseLayer->addChild(diamond);

	int offsetx = (( int )(CCRANDOM_0_1() * 1000 + 100)) % 800;
	int offsety = (( int )(CCRANDOM_0_1() * 1000)) % 400;
	auto nextLoc =  Vec2(offsetx, offsety);
	this->mouse->runAction(MoveTo::create(0.5f, mouseLayer->convertToNodeSpace(nextLoc)));
}

void GameScene::mouseCheeseCollision() {
	if (mouse->getBoundingBox().intersectsRect(cheese->getBoundingBox())) {
		cheese->removeFromParent();
	} else {
		//cheese->runAction(runAction(Sequence::create(DelayTime::create(1.0), FadeOut::create(3.0f), NULL)));
	}
}
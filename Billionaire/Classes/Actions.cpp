#include "Actions.h"
#include "GameMainScene.h"
#include "BSystemController.h"
using namespace Emilia;

Actions* Actions::_instance = nullptr;

Actions::Actions(){ }

Actions::~Actions(){ }

Actions * Actions::getInstance()
{
	if (_instance == nullptr)
		_instance = new Actions();
	return _instance;
}

void Actions::initial()
{
	for (int i = 0; i < 4; i++) {
		char str[20];
		sprintf(str, "Character%dw.png", i + 1);
		players[i] = Sprite::create(str, CC_RECT_PIXELS_TO_POINTS(Rect(32, 32 * 3, 32, 32)));
		players[i]->setPosition(16 + 32 * 4, 32 * 3);

		auto texture = Director::getInstance()->getTextureCache()->addImage(str);


		for (int j = 0; j < 4; j++) {
			playerMoving[i * 4 + j].reserve(4);

			for (int k = 0; k < 3; k++) {
				auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(32 * k, 32 * j, 32, 32)));
				playerMoving[i * 4 + j].pushBack(frame);
			}

			auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(32, 32 * j, 32, 32)));
			playerMoving[i * 4 + j].pushBack(frame);
		}
	}
}

void Actions::skillBling(int skillID)
{
	if (skillID == 0) {
		skill = Sprite::create("up.png");
		skill->setPosition(12 * 32, 12 * 32);
		skill->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, Vec2(25 * 32, 48)), ScaleBy::create(0.5, 0.2)), nullptr));
	}
	else if (skillID == 1) {
		skill = Sprite::create("down.png");
		skill->setPosition(12 * 32, 12 * 32);
		skill->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, Vec2(25 * 32, 48)), ScaleBy::create(0.5, 0.2)), nullptr));
	}
	else if (skillID == 2) {
		skill = Sprite::create("demo.png");
		skill->setPosition(12 * 32, 12 * 32);
		skill->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, Vec2(25 * 32, 48)), ScaleBy::create(0.5, 0.2)), nullptr));
	}
	else if (skillID == 3) {
		skill = Sprite::create("sale.png");
		skill->setPosition(12 * 32, 12 * 32);
		skill->runAction(Sequence::create(Spawn::create(MoveTo::create(0.5, Vec2(25 * 32, 48)), ScaleBy::create(0.5, 0.2)), nullptr));
	}
}

void Actions::roll(int rollNumer) {
	roller = Sprite::create("roll.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 75 * (6 - rollNumer), 85, 75)));
	roller->setPosition(Vec2(75, 50));
	GameMainScene::getInstance()->addChild(roller, 2);
}

void Actions::playerMove(int id, int pos, int steps)
{
	auto pay = CallFunc::create([this, id]() {
		BSystemController::getInstance()->payCharge();
		players[id]->getActionManager()->removeAllActionsByTag(1, players[id]);
	});

	while (steps > 0) {
		if (pos < 9) {
			auto animation = Animation::createWithSpriteFrames(playerMoving[id * 4 + 3], 0.3f);
			auto animate = RepeatForever::create(Animate::create(animation));
			animate->setTag(1);
			players[id]->runAction(animate);

			int stepped = pos + steps > 9 ? 9 - pos : steps;
			players[id]->runAction(Sequence::create(MoveBy::create(steps, Vec2(0, stepped * 64)), pay, nullptr));
			steps -= stepped;
			pos += stepped;
		}
		else if (pos < 19) {
			auto animation = Animation::createWithSpriteFrames(playerMoving[id * 4 + 2], 0.3f);
			auto animate = RepeatForever::create(Animate::create(animation));
			animate->setTag(1);
			players[id]->runAction(animate);

			int stepped = pos + steps > 19 ? 19 - pos : steps;
			players[id]->runAction(Sequence::create(MoveBy::create(steps, Vec2(stepped * 64, 0)), pay, nullptr));
			steps -= stepped;
			pos += stepped;
		}
		else if (pos < 29) {
			auto animation = Animation::createWithSpriteFrames(playerMoving[id * 4], 0.3f);
			auto animate = RepeatForever::create(Animate::create(animation));
			animate->setTag(1);
			players[id]->runAction(animate);

			int stepped = pos + steps > 29 ? 29 - pos : steps;
			players[id]->runAction(Sequence::create(MoveBy::create(steps, Vec2(0, 0 - stepped * 64)), pay, nullptr));
			steps -= stepped;
			pos += stepped;
		}
		else if (pos < 39) {
			auto animation = Animation::createWithSpriteFrames(playerMoving[id * 4 + 1], 0.3f);
			auto animate = RepeatForever::create(Animate::create(animation));
			animate->setTag(1);
			players[id]->runAction(animate);

			int stepped = pos + steps > 39 ? 39 - pos : steps;
			players[id]->runAction(Sequence::create(MoveBy::create(steps, Vec2(0 - stepped * 64, 0)), pay, nullptr));
			steps -= stepped;
			pos += stepped;
		}
	}



}

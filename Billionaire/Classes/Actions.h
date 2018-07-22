#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class Actions {

public:
	Actions();

	~Actions();

	static Actions* getInstance();

	void initial();

	void skillBling(int skillID);

	void roll(int rollNumer);

	void playerMove(int id, int pos, int steps);

	Sprite *players[4];
	Sprite *skill;
	Sprite *roller;
private:
	static Actions* _instance;
	cocos2d::Vector<SpriteFrame*> playerMoving[16];
	int pos[40];
};
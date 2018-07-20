#ifndef __SCENE_H__
#define __SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

struct player{
  int money;
};

class GameMainScene : public cocos2d::Scene {
public:
  static cocos2d::Scene* createScene();

  GameMainScene* getInstance();

  virtual bool init();

  void GameStart();

  void RoundStart();

  void playerLose();

  void updatePlayerState();

  void log(std::string msg);

  void roundEnd();

  void UpdateTime(float dt);

  // implement the "static create()" method manually
  CREATE_FUNC(GameMainScene);

private:
  GameMainScene* instance;

  player players[4];

  int skillID;
  
  int infoIndex;

  std::string logInfo[5];

  Label *logLabel;
  Label *moneyLabel[4];
};
#endif
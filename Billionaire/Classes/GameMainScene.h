#ifndef __SCENE_H__
#define __SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameMainScene : public cocos2d::Scene {
public:
  static cocos2d::Scene* createScene();

  GameMainScene* getInstance();

  virtual bool init();

  void GameStart();

  void RoundStart();

  void playerLose();

  void updatePlayerState(int id);

  void log(std::string msg);

  void roundEnd();

  // implement the "static create()" method manually
  CREATE_FUNC(GameMainScene);

private:
  GameMainScene* instance;

  int skillID;
  
  int infoIndex;

  std::string logInfo[5];

  Label *moneyLabel[4];
  Sprite *playerImage[4];
  Label *logLabel;
};
#endif
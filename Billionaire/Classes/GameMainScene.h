#ifndef __SCENE_H__
#define __SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameMainScene : public cocos2d::Scene {
private:
  ~GameMainScene() {}

  static GameMainScene* instance;

  int skillID;
  
  CCLayerColor *rightBar;

  CCLayerColor *players[4];
  Label *moneyLabel[4];
  Sprite *playerImage[4];

  Label *logLabel;
  int infoIndex;
  std::string logInfo[5];

public:
  //static cocos2d::Scene* createScene();
  static GameMainScene* createScene();

  virtual bool init();

  static GameMainScene* getInstance() {
    if (instance == nullptr) {
      instance = new GameMainScene();
      instance->init();
    }
    return instance;
  }

  void GameStart();

  void RoundStart();

  void playerLose();

  void updatePlayerState(int id);

  void log(std::string msg);

  void roundEnd();

  // implement the "static create()" method manually
  CREATE_FUNC(GameMainScene);
};

#endif
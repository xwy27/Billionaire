#include "GameMainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameMainScene::createScene() {
  return GameMainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameMainSceneScene.cpp\n");
}

GameMainScene* GameMainScene::getInstance() {
  if (instance == NULL) {
    instance = new GameMainScene();
  }
  return instance;
}

bool GameMainScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  infoIndex = 0;

  for (int i = 0; i < 4; ++i) {
    players[i].money = 15000;
  }

  for (int i = 0; i < 4; ++i) {
    // 头像

    // 金钱
    moneyLabel[i] = Label::create("", "arial", 15);
    if (moneyLabel[i]) {
      float x = origin.x + visibleSize.width - 25;
      float y = origin.y + visibleSize.height - (i ? 25 + 50 * i : 25);
      moneyLabel[i]->setPosition(Vec2(x, y));
      this->addChild(moneyLabel[i], 1);
    }
  }

  logLabel = Label::create("Upgrade building!\ntest\ntest\ntest\ntest\n", "arial", 15);
  if (logLabel) {
    float x = origin.x + visibleSize.width - 50;
    float y = origin.y + 50;
    logLabel->setPosition(Vec2(x, y));
    this->addChild(logLabel, 1);
  }

  // 按钮
  // 结束回合
  auto endLabel = Label::create("End", "arial", 15);
  // 骰子
  auto rollLabel = Label::create("Roll", "arial", 15);
  // 技能
  auto skillLabel = Label::create("Skill", "arial", 15);
  // 卖房子
  auto sellLabel = Label::create("Sell", "arial", 15);
  // 买/建房子
  auto upgradeLabel = Label::create("Buy", "arial", 15);
  
  auto endButton = MenuItemLabel::create(endLabel, CC_CALLBACK_0(GameMainScene::roundEnd, this));
  auto rollButton = MenuItemLabel::create(rollLabel/*, CC_CALLBACK_0(Controller::roll, this)*/);
  auto skillButton = MenuItemLabel::create(skillLabel/*, CC_CALLBACK_0(Controller::useSkill, this)*/);
  auto sellButton = MenuItemLabel::create(sellLabel/*, CC_CALLBACK_0(Controller::sellLand, this)*/);
  auto upgradeButton = MenuItemLabel::create(upgradeLabel/*, CC_CALLBACK_0(Controller::upgradeLand, this)*/);
  endButton->setPosition(Vec2(origin.x + 25, origin.y + 25));
  rollButton->setPosition(Vec2(origin.x + 75, origin.y + 25));
  skillButton->setPosition(Vec2(origin.x + 200, origin.y + 25));
  sellButton->setPosition(Vec2(origin.x + 250, origin.y + 25));
  upgradeButton->setPosition(Vec2(origin.x + 300, origin.y + 25));

  auto menu = Menu::create(endButton, rollButton, skillButton, sellButton, upgradeButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  GameStart();

  schedule(schedule_selector(GameMainScene::UpdateTime), 1, -1, 0);
  return true;
}

void GameMainScene::GameStart() {
  //Action.getInstance().initial();
  //Control.getInstance().initial();
}


void GameMainScene::updatePlayerState() {
  //int playerID = Controller.getInstance().getUserID();
  //players[playerID].money = Controller.getInstacne().getMoney();
}

void GameMainScene::UpdateTime(float dt) {
  for (int i = 0; i < 4; ++i) {
    moneyLabel[i]->setString(std::to_string(players[i].money));
  }
}

void GameMainScene::RoundStart() {
  //int playerID = Control.getInstance().getUserID();
  /* 
   * 改底色
   */

  //skillID = Controller.getInstance().newSkillID();
  //Action.getInstance().skillBling();
}

void GameMainScene::playerLose() {
  // int playerID = Controller.getInstance().getUserID();
  // 底色变灰色
}

void GameMainScene::log(std::string msg) {
  if (infoIndex < 5) {
    logInfo[infoIndex] = msg;
    infoIndex++;
  } else {
    for (int i = 1; i < infoIndex; ++i) {
      logInfo[i - 1] = logInfo[i];
    }
    logInfo[4] = msg;
  }

  std::string temp = "";
  for (int i = 0; i < infoIndex; ++i) {
    temp += logInfo[i] + "\n";
  }
  logLabel->setString(temp);
}

void GameMainScene::roundEnd() {
  if (/* Controller.getInstance().isRolled()*/true) {
    //Controller.getInstance(),nextPlayer()
  }
}
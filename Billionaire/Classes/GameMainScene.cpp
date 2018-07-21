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

/*
 * 单例模式，返回 GameMainScene 的单例
 */
GameMainScene* GameMainScene::getInstance() {
  if (instance == NULL) {
    instance = new GameMainScene();
  }
  return instance;
}

/*
 * GameMainScene 初始化
 */
bool GameMainScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // log 信息数量初始化
  infoIndex = 0;

  // 初始化右侧栏玩家头像和金钱模型
  for (int i = 0; i < 4; ++i) {
    // 头像
    playerImage[i] = Sprite::create("closeNormal.png");
    // 金钱
    moneyLabel[i] = Label::create("15000", "arial", 15);
    
    // 头像位置
    float x = origin.x + visibleSize.width - 75;
    float y = origin.y + visibleSize.height - (i ? 25 + 50 * i : 25);
    playerImage[i]->setPosition(Vec2(x, y));
    playerImage[i]->setScale(0.5);
    
    // 金钱位置:头像位置右偏移30
    moneyLabel[i]->setPosition(Vec2(x + 30, y));
    
    this->addChild(playerImage[i], 1);
    this->addChild(moneyLabel[i], 1);
  }

  // Log 窗口
  logLabel = Label::create("Upgrade building!\ntest\ntest\ntest\ntest\n", "arial", 15);
  float logX = origin.x + visibleSize.width - 50;
  float logY = origin.y + 50;
  logLabel->setPosition(Vec2(logX, logY));
  this->addChild(logLabel, 1);

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
  skillButton->setPosition(Vec2(logX - 100, origin.y + 25));
  sellButton->setPosition(Vec2(logX - 150, origin.y + 25));
  upgradeButton->setPosition(Vec2(logX - 200, origin.y + 25));

  auto menu = Menu::create(endButton, rollButton, skillButton, sellButton, upgradeButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  // 游戏开始
  GameStart();

  return true;
}

/*
 * 初始化动画模块与控制模块
 */
void GameMainScene::GameStart() {
  //Action.getInstance().initial();
  //Control.getInstance().initial();
  RoundStart();
}

/*
 * 更新当前回合玩家金钱数量
 */
void GameMainScene::updatePlayerState(int id) {
  //moneyLabel[i]->setString(std::to_string(Controller.getInstacne().getMoney(id)));
}

/*回合开始
 */
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
    //Controller.getInstance().nextPlayer()
  }
}
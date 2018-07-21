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

  // 初始化右侧栏
  rightBar = CCLayerColor::create(Color4B::GRAY);
  rightBar->setContentSize(CCSizeMake(170, visibleSize.height));
  rightBar->setPosition(Vec2(visibleSize.width - 170, 0));
  this->addChild(rightBar, 1);
  // 初始化玩家头像和金钱模型
  auto PlayerInfo = Label::create("Players", "arial", 15);
  PlayerInfo->setPosition(85, visibleSize.height - 25);
  rightBar->addChild(PlayerInfo, 1);
  for (int i = 0; i < 4; ++i) {    
    players[i] = CCLayerColor::create(Color4B(200, 200, 0, 255));
    players[i]->setContentSize(CCSizeMake(150, 50));
    // layer 位置
    float X = 10;
    float Y = visibleSize.height - (i ? 100 + 60 * i : 100);
    players[i]->setPosition(Vec2(X, Y));
    rightBar->addChild(players[i], 1);
    
    // 头像
    playerImage[i] = Sprite::create("closeNormal.png");
    // 金钱
    moneyLabel[i] = Label::create("15000", "arial", 15);
    
    // 头像位置
    float x = 25;
    float y = 25;
    playerImage[i]->setPosition(Vec2(x, y));
    playerImage[i]->setScale(0.5);
    
    // 金钱位置:头像位置右偏移30
    moneyLabel[i]->setPosition(Vec2(x + 50, y));
    
    players[i]->addChild(playerImage[i], 1);
    players[i]->addChild(moneyLabel[i], 1);
  }

  // Log 窗口
  auto LogInfo = Label::create("Events", "arial", 15);
  LogInfo->setPosition(85, 125);
  rightBar->addChild(LogInfo, 1);
  logLabel = Label::create("Upgrade building!\ntest\ntest\ntest\ntest\n", "arial", 15);
  float logX = 75;
  float logY = 50;
  logLabel->setPosition(Vec2(logX, logY));
  rightBar->addChild(logLabel, 1);

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
  skillButton->setPosition(Vec2(visibleSize.width - 225, origin.y + 25));
  sellButton->setPosition(Vec2(visibleSize.width - 275, origin.y + 25));
  upgradeButton->setPosition(Vec2(visibleSize.width - 325, origin.y + 25));

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

/*
 * 回合开始
 * 更改当前回合玩家背景底色
 * 抽取技能
 */
void GameMainScene::RoundStart() {
  //int playerID = Control.getInstance().getUserID();
  //players[playerID]->setColor(Color3B::RED);

  //skillID = Controller.getInstance().newSkillID();
  //Action.getInstance().skillBling();
}

/*
 * 玩家破产
 * 玩家背景底色变灰色
 */
void GameMainScene::playerLose() {
  // int playerID = Controller.getInstance().getUserID();
  //players[playerID]->setColor(Color3B::GREY);
}

/*
 * 事件输出
 * 最多输出 5 条信息，多了删去最远一条
 * 可能存在信息过长而显示不全，注意传过参之前换行截断
 */
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

/*
 * 回合结束
 * 当前玩家背景底色恢复
 * 通知控制模块更新玩家状态
 */
void GameMainScene::roundEnd() {
  if (/* Controller.getInstance().isRolled()*/true) {
    //int playerID = Control.getInstance().getUserID();
    //players[playerID]->setColor(Color3B::YELLOW);
    
    //Controller.getInstance().nextPlayer()
  }
}
#include "GameMainScene.h"
#include "SimpleAudioEngine.h"
#include "BSystemController.h"
#include "Actions.h"

USING_NS_CC;
using namespace Emilia;
using namespace CocosDenshion;

GameMainScene* GameMainScene::instance = nullptr;

GameMainScene* GameMainScene::createScene() {
  instance = GameMainScene::create();
  return instance;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameMainSceneScene.cpp\n");
}

/*
 * GameMainScene 初始化
 */
bool GameMainScene::init() {
  if (!Scene::init()) {
    return false;
  }

  TTFConfig ttfConfig("fonts/Consola.ttf", 20);

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
  auto PlayerInfo = Label::createWithTTF(ttfConfig, "Players");
  PlayerInfo->enableBold();
  PlayerInfo->setColor(Color3B::BLACK);
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
	  char str[20];
	  sprintf(str, "Character%d.png", i + 1);
	  playerImage[i] = Sprite::create(str);

    // 金钱
    moneyLabel[i] = Label::create("10000", "arial", 15);
    moneyLabel[i]->enableBold();
    
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
  auto LogInfo = Label::createWithTTF(ttfConfig, "Events");
  LogInfo->enableBold();
  LogInfo->setColor(Color3B::BLACK);
  LogInfo->setPosition(85, 475);
  rightBar->addChild(LogInfo, 1);
  logLabel = Label::create("Test", "arial", 15);
  logLabel->enableBold();
  logLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
  logLabel->setLineBreakWithoutSpace(true);
  logLabel->setMaxLineWidth(125);
  logLabel->setDimensions(125, 425);
  float logX = 75;
  float logY = 250;
  logLabel->setPosition(Vec2(logX, logY));
  rightBar->addChild(logLabel, 1);

  // 按钮
  // 游戏结束
  auto overLabel = Label::createWithTTF(ttfConfig, "End Game");
  // 音乐
  auto musicLabel = Label::createWithTTF(ttfConfig, "Music");
  // 结束回合
  auto endLabel = Label::createWithTTF(ttfConfig, "End");
  // 骰子
  auto rollLabel = Label::createWithTTF(ttfConfig, "Roll");
  // 技能
  auto skillLabel = Label::createWithTTF(ttfConfig, "Skill");
  // 卖房子
  auto sellLabel = Label::createWithTTF(ttfConfig, "Sell");
  // 买/建房子
  auto upgradeLabel = Label::createWithTTF(ttfConfig, "Buy");
  
  auto overButton = MenuItemLabel::create(overLabel/*, CC_CALLBACK_0(BSystemController::gameOver, BSystemController::getInstance())*/);
  auto musicButton = MenuItemLabel::create(musicLabel, [=] (Ref* sender) {
    auto audio = SimpleAudioEngine::getInstance();
    if (audio->isBackgroundMusicPlaying()) {
      audio->stopBackgroundMusic();
    } else {
      audio->playBackgroundMusic("music/bgm.mp3");
    }
  });
  auto endButton = MenuItemLabel::create(endLabel, CC_CALLBACK_0(GameMainScene::roundEnd, this));
  auto rollButton = MenuItemLabel::create(rollLabel, CC_CALLBACK_0(BSystemController::roll, BSystemController::getInstance()));
  auto skillButton = MenuItemLabel::create(skillLabel, CC_CALLBACK_0(BSystemController::useSkill, BSystemController::getInstance()));
  auto sellButton = MenuItemLabel::create(sellLabel, CC_CALLBACK_0(BSystemController::sellLand, BSystemController::getInstance()));
  auto upgradeButton = MenuItemLabel::create(upgradeLabel, CC_CALLBACK_0(BSystemController::upgradeLand, BSystemController::getInstance()));
  overButton->setPosition(Vec2(origin.x + 50, origin.x + visibleSize.height - 25));
  musicButton->setPosition(Vec2(origin.x + 50, origin.x + visibleSize.height - 50));
  endButton->setPosition(Vec2(origin.x + 25, origin.y + 25));
  rollButton->setPosition(Vec2(origin.x + 75, origin.y + 25));
  skillButton->setPosition(Vec2(visibleSize.width - 215, origin.y + 25));
  sellButton->setPosition(Vec2(visibleSize.width - 275, origin.y + 25));
  upgradeButton->setPosition(Vec2(visibleSize.width - 325, origin.y + 25));

  auto menu = Menu::create(overButton, musicButton, endButton, rollButton, skillButton, sellButton, upgradeButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  //初始化地图
  TMXTiledMap* tmx = TMXTiledMap::create("map1.tmx");
  tmx->setPosition((visibleSize.width - 170) / 2, visibleSize.height / 2);
  tmx->setAnchorPoint(Vec2(0.5, 0.5));
  this->addChild(tmx, 0);

  // 游戏开始
  GameStart();

  //放置player
  for (int i = 0; i < 4; i++) {
	this->addChild(Actions::getInstance()->players[i], 1);
  }

  return true;
}

/*
 * 初始化动画模块与控制模块
 */
void GameMainScene::GameStart() {
  Actions::getInstance()->initial();
  BSystemController::getInstance()->initial();
  RoundStart();
}

/*
 * 更新当前回合玩家金钱数量
 */
void GameMainScene::updatePlayerState(int id) {
  moneyLabel[id]->setString(std::to_string(BSystemController::getInstance()->getMoney(id)));
}

/*
 * 回合开始
 * 更改当前回合玩家背景底色
 * 抽取技能
 */
void GameMainScene::RoundStart() {
  int playerID = BSystemController::getInstance()->getUserID();
  players[playerID]->setColor(Color3B::RED);

  skillID = BSystemController::getInstance()->newSkillID();
  Actions::getInstance()->skillBling(skillID);

  this->addChild(Actions::getInstance()->skill, 3);
}

/*
 * 玩家破产
 * 玩家背景底色变灰色
 */
void GameMainScene::playerLose() {
  int playerID = BSystemController::getInstance()->getUserID();
  players[playerID]->setColor(Color3B::GRAY);
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
    temp += logInfo[i] + "\n\n";
  }
  logLabel->setString(temp);
}

/*
 * 回合结束
 * 当前玩家背景底色恢复
 * 通知控制模块更新玩家状态
 */
void GameMainScene::roundEnd() {
  if (BSystemController::getInstance()->isRolled()) {
    int playerID = BSystemController::getInstance()->getUserID();
    players[playerID]->setColor(Color3B::YELLOW);
	this->removeChild(Actions::getInstance()->roller);
	this->removeChild(Actions::getInstance()->skill);
    BSystemController::getInstance()->nextPlayer();
  }
}

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
 * GameMainScene ��ʼ��
 */
bool GameMainScene::init() {
  if (!Scene::init()) {
    return false;
  }

  TTFConfig ttfConfig("fonts/Consola.ttf", 20);

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // log ��Ϣ������ʼ��
  infoIndex = 0;


  // ��ʼ���Ҳ���
  rightBar = CCLayerColor::create(Color4B::GRAY);
  rightBar->setContentSize(CCSizeMake(170, visibleSize.height));
  rightBar->setPosition(Vec2(visibleSize.width - 170, 0));
  this->addChild(rightBar, 1);
  // ��ʼ�����ͷ��ͽ�Ǯģ��
  auto PlayerInfo = Label::createWithTTF(ttfConfig, "Players");
  PlayerInfo->enableBold();
  PlayerInfo->setColor(Color3B::BLACK);
  PlayerInfo->setPosition(85, visibleSize.height - 25);
  rightBar->addChild(PlayerInfo, 1);
  for (int i = 0; i < 4; ++i) {    
    players[i] = CCLayerColor::create(Color4B(200, 200, 0, 255));
    players[i]->setContentSize(CCSizeMake(150, 50));
    // layer λ��
    float X = 10;
    float Y = visibleSize.height - (i ? 100 + 60 * i : 100);
    players[i]->setPosition(Vec2(X, Y));
    rightBar->addChild(players[i], 1);
    
    // ͷ��
	  char str[20];
	  sprintf(str, "Character%d.png", i + 1);
	  playerImage[i] = Sprite::create(str);

    // ��Ǯ
    moneyLabel[i] = Label::create("10000", "arial", 15);
    moneyLabel[i]->enableBold();
    
    // ͷ��λ��
    float x = 25;
    float y = 25;
    playerImage[i]->setPosition(Vec2(x, y));
    playerImage[i]->setScale(0.5);
    
    // ��Ǯλ��:ͷ��λ����ƫ��30
    moneyLabel[i]->setPosition(Vec2(x + 50, y));
    
    players[i]->addChild(playerImage[i], 1);
    players[i]->addChild(moneyLabel[i], 1);
  }

  // Log ����
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

  // ��ť
  // ��Ϸ����
  auto overLabel = Label::createWithTTF(ttfConfig, "End Game");
  // ����
  auto musicLabel = Label::createWithTTF(ttfConfig, "Music");
  // �����غ�
  auto endLabel = Label::createWithTTF(ttfConfig, "End");
  // ����
  auto rollLabel = Label::createWithTTF(ttfConfig, "Roll");
  // ����
  auto skillLabel = Label::createWithTTF(ttfConfig, "Skill");
  // ������
  auto sellLabel = Label::createWithTTF(ttfConfig, "Sell");
  // ��/������
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

  //��ʼ����ͼ
  TMXTiledMap* tmx = TMXTiledMap::create("map1.tmx");
  tmx->setPosition((visibleSize.width - 170) / 2, visibleSize.height / 2);
  tmx->setAnchorPoint(Vec2(0.5, 0.5));
  this->addChild(tmx, 0);

  // ��Ϸ��ʼ
  GameStart();

  //����player
  for (int i = 0; i < 4; i++) {
	this->addChild(Actions::getInstance()->players[i], 1);
  }

  return true;
}

/*
 * ��ʼ������ģ�������ģ��
 */
void GameMainScene::GameStart() {
  Actions::getInstance()->initial();
  BSystemController::getInstance()->initial();
  RoundStart();
}

/*
 * ���µ�ǰ�غ���ҽ�Ǯ����
 */
void GameMainScene::updatePlayerState(int id) {
  moneyLabel[id]->setString(std::to_string(BSystemController::getInstance()->getMoney(id)));
}

/*
 * �غϿ�ʼ
 * ���ĵ�ǰ�غ���ұ�����ɫ
 * ��ȡ����
 */
void GameMainScene::RoundStart() {
  int playerID = BSystemController::getInstance()->getUserID();
  players[playerID]->setColor(Color3B::RED);

  skillID = BSystemController::getInstance()->newSkillID();
  Actions::getInstance()->skillBling(skillID);

  this->addChild(Actions::getInstance()->skill, 3);
}

/*
 * ����Ʋ�
 * ��ұ�����ɫ���ɫ
 */
void GameMainScene::playerLose() {
  int playerID = BSystemController::getInstance()->getUserID();
  players[playerID]->setColor(Color3B::GRAY);
}

/*
 * �¼����
 * ������ 5 ����Ϣ������ɾȥ��Զһ��
 * ���ܴ�����Ϣ��������ʾ��ȫ��ע�⴫����֮ǰ���нض�
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
 * �غϽ���
 * ��ǰ��ұ�����ɫ�ָ�
 * ֪ͨ����ģ��������״̬
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

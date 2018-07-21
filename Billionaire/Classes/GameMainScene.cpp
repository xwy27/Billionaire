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
 * ����ģʽ������ GameMainScene �ĵ���
 */
GameMainScene* GameMainScene::getInstance() {
  if (instance == NULL) {
    instance = new GameMainScene();
  }
  return instance;
}

/*
 * GameMainScene ��ʼ��
 */
bool GameMainScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // log ��Ϣ������ʼ��
  infoIndex = 0;

  // ��ʼ���Ҳ������ͷ��ͽ�Ǯģ��
  for (int i = 0; i < 4; ++i) {
    // ͷ��
    playerImage[i] = Sprite::create("closeNormal.png");
    // ��Ǯ
    moneyLabel[i] = Label::create("15000", "arial", 15);
    
    // ͷ��λ��
    float x = origin.x + visibleSize.width - 75;
    float y = origin.y + visibleSize.height - (i ? 25 + 50 * i : 25);
    playerImage[i]->setPosition(Vec2(x, y));
    playerImage[i]->setScale(0.5);
    
    // ��Ǯλ��:ͷ��λ����ƫ��30
    moneyLabel[i]->setPosition(Vec2(x + 30, y));
    
    this->addChild(playerImage[i], 1);
    this->addChild(moneyLabel[i], 1);
  }

  // Log ����
  logLabel = Label::create("Upgrade building!\ntest\ntest\ntest\ntest\n", "arial", 15);
  float logX = origin.x + visibleSize.width - 50;
  float logY = origin.y + 50;
  logLabel->setPosition(Vec2(logX, logY));
  this->addChild(logLabel, 1);

  // ��ť
  // �����غ�
  auto endLabel = Label::create("End", "arial", 15);
  // ����
  auto rollLabel = Label::create("Roll", "arial", 15);
  // ����
  auto skillLabel = Label::create("Skill", "arial", 15);
  // ������
  auto sellLabel = Label::create("Sell", "arial", 15);
  // ��/������
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

  // ��Ϸ��ʼ
  GameStart();

  return true;
}

/*
 * ��ʼ������ģ�������ģ��
 */
void GameMainScene::GameStart() {
  //Action.getInstance().initial();
  //Control.getInstance().initial();
  RoundStart();
}

/*
 * ���µ�ǰ�غ���ҽ�Ǯ����
 */
void GameMainScene::updatePlayerState(int id) {
  //moneyLabel[i]->setString(std::to_string(Controller.getInstacne().getMoney(id)));
}

/*�غϿ�ʼ
 */
void GameMainScene::RoundStart() {
  //int playerID = Control.getInstance().getUserID();
  /* 
   * �ĵ�ɫ
   */

  //skillID = Controller.getInstance().newSkillID();
  //Action.getInstance().skillBling();
}

void GameMainScene::playerLose() {
  // int playerID = Controller.getInstance().getUserID();
  // ��ɫ���ɫ
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
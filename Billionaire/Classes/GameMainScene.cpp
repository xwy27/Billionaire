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

  // ��ʼ���Ҳ���
  rightBar = CCLayerColor::create(Color4B::GRAY);
  rightBar->setContentSize(CCSizeMake(170, visibleSize.height));
  rightBar->setPosition(Vec2(visibleSize.width - 170, 0));
  this->addChild(rightBar, 1);
  // ��ʼ�����ͷ��ͽ�Ǯģ��
  auto PlayerInfo = Label::create("Players", "arial", 15);
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
    playerImage[i] = Sprite::create("closeNormal.png");
    // ��Ǯ
    moneyLabel[i] = Label::create("15000", "arial", 15);
    
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
  auto LogInfo = Label::create("Events", "arial", 15);
  LogInfo->setPosition(85, 125);
  rightBar->addChild(LogInfo, 1);
  logLabel = Label::create("Upgrade building!\ntest\ntest\ntest\ntest\n", "arial", 15);
  float logX = 75;
  float logY = 50;
  logLabel->setPosition(Vec2(logX, logY));
  rightBar->addChild(logLabel, 1);

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
  skillButton->setPosition(Vec2(visibleSize.width - 225, origin.y + 25));
  sellButton->setPosition(Vec2(visibleSize.width - 275, origin.y + 25));
  upgradeButton->setPosition(Vec2(visibleSize.width - 325, origin.y + 25));

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

/*
 * �غϿ�ʼ
 * ���ĵ�ǰ�غ���ұ�����ɫ
 * ��ȡ����
 */
void GameMainScene::RoundStart() {
  //int playerID = Control.getInstance().getUserID();
  //players[playerID]->setColor(Color3B::RED);

  //skillID = Controller.getInstance().newSkillID();
  //Action.getInstance().skillBling();
}

/*
 * ����Ʋ�
 * ��ұ�����ɫ���ɫ
 */
void GameMainScene::playerLose() {
  // int playerID = Controller.getInstance().getUserID();
  //players[playerID]->setColor(Color3B::GREY);
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
    temp += logInfo[i] + "\n";
  }
  logLabel->setString(temp);
}

/*
 * �غϽ���
 * ��ǰ��ұ�����ɫ�ָ�
 * ֪ͨ����ģ��������״̬
 */
void GameMainScene::roundEnd() {
  if (/* Controller.getInstance().isRolled()*/true) {
    //int playerID = Control.getInstance().getUserID();
    //players[playerID]->setColor(Color3B::YELLOW);
    
    //Controller.getInstance().nextPlayer()
  }
}
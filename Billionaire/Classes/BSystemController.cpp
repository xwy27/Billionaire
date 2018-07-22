#include "BSystemController.h"
#include <string>
using namespace Emilia;
using std::string;

BSystemController* BSystemController::_instance = nullptr;

BSystemController::BSystemController() {}

BSystemController::~BSystemController() {
  if (players != nullptr)
    delete[] players;
  if (lands != nullptr)
    delete[] lands;
}

BSystemController* BSystemController::getInstance() {
  if (_instance == nullptr)
    _instance = new BSystemController();
  return _instance;
}

void BSystemController::initial() {
  presentPlayer = 0;
  rollNumer = 0;
  skillID = 0;
  skillUsed = false;
  isGameOver = false;
  playersLeft = 4;
  aniInstance = Actions::getInstance();

  if (players == nullptr)
    players = new PlayerStatus[4];
  if (lands == nullptr)
    lands = new Land[MAX_LANDS_NUMBER];

  for (auto i = 0; i < 4; ++i)
    players[i].initial();
  
  for (auto i = 0; i < MAX_LANDS_NUMBER; ++i) {
    lands[i].initial();
    lands[i].price = 100 * random(10, 20);
  }
}

int BSystemController::getUserID() {
  return presentPlayer;
}

int BSystemController::newSkillID() {
  skillID = random() % 4;
  return skillID;
}

int BSystemController::getMoney(int playerID) {
  return players[playerID].wealth;
}

int* BSystemController::getLandsPrice() {
  int *prices = new int[MAX_LANDS_NUMBER];
  for (auto i = 0; i < MAX_LANDS_NUMBER; ++i) {
    prices[i] = lands[i].price;
  }

  return prices;
  // need to be deleted
}

int BSystemController::roll() {
  if (!isRolled()) {
    rollNumer = random() % 6 + 1;
    string temp = "You rolled " ;
    GameMainScene::getInstance()->log(temp + std::to_string(rollNumer));
	aniInstance->roll(rollNumer);
	aniInstance->playerMove(presentPlayer, players[presentPlayer].location, rollNumer);
    players[presentPlayer].location += rollNumer;
    if (players[presentPlayer].location >= MAX_LANDS_NUMBER) {
      players[presentPlayer].wealth += 2000;
      GameMainScene::getInstance()->log("As a reward of passing the start point, you gained 2000!");
    }
    players[presentPlayer].location %= MAX_LANDS_NUMBER;
    return rollNumer;
  } else {
    GameMainScene::getInstance()->log("You've rolled!");
    return rollNumer;
  }
}

void BSystemController::payCharge() {
  if (isGameOver)
    return;
  auto landOwner = lands[players[presentPlayer].location].owner;
  if (landOwner != -1 && landOwner != presentPlayer){
    players[presentPlayer].wealth -= lands[players[presentPlayer].location].getCharge();
    players[landOwner].wealth += lands[players[presentPlayer].location].getCharge();
    string temp = "Player ";
    temp += std::to_string(presentPlayer);
    temp += " pay ";
    temp += std::to_string(lands[players[presentPlayer].location].getCharge());
    temp += " to Player ";
    temp += landOwner;
    GameMainScene::getInstance()->log(temp);
    GameMainScene::getInstance()->updatePlayerState(presentPlayer);
    GameMainScene::getInstance()->updatePlayerState(landOwner);
  }

  if (players[presentPlayer].wealth < 0) {
    players[presentPlayer].order = playersLeft--;
    players[presentPlayer].isAlive = false;
    GameMainScene::getInstance()->log("You have no money to pay for the charge! \nYou lose.");
    GameMainScene::getInstance()->playerLose();
  }
}

void BSystemController::useSkill() {
  if (isGameOver)
    return;
  if (skillUsed) {
    GameMainScene::getInstance()->log("You've used your skill!");
    return;
  }
  auto temp = lands[players[presentPlayer].location].price;
  switch(skillID) {
    case 0:
      lands[players[presentPlayer].location].price *= 1.3;
      GameMainScene::getInstance()->log("The basic price of this land rise!");
    break;
    case 1:
      lands[players[presentPlayer].location].price /= 1.3;
      GameMainScene::getInstance()->log("The basic price of this land fall!");
    break;
    case 2:
      if (players[presentPlayer].wealth > 1000) {
        if (lands[players[presentPlayer].location].level > 0) {
          players[presentPlayer].wealth -= 1000;
          lands[players[presentPlayer].location].level -= 1;
          GameMainScene::getInstance()->log("You spent 1000 to destroy one building!");
        }
        else {
          GameMainScene::getInstance()->log("No building on this land...");
        }
      }
      else {
        GameMainScene::getInstance()->log("Insufficient fund...");
      }
    break;
    case 3:
      lands[players[presentPlayer].location].price *= 0.8;
      buyLand();
      lands[players[presentPlayer].location].price = temp;
    break;
    default:
    break;
  }
  skillUsed = true;
}

void BSystemController::buyLand() {
  if (isGameOver)
    return;
  auto& presentLand = lands[players[presentPlayer].location];
  if (presentLand.owner == -1) {
    if (presentLand.price <= players[presentPlayer].wealth) {
      players[presentPlayer].wealth -= presentLand.price;
      players[presentPlayer].lands.push_back(players[presentPlayer].location);
      presentLand.owner = presentPlayer;
      GameMainScene::getInstance()->updatePlayerState(presentPlayer);
      GameMainScene::getInstance()->log("Successfully bought!");
    }
    else {
      GameMainScene::getInstance()->log("Insufficient fund...");
    }
  }
  else {
    GameMainScene::getInstance()->log("This land has its owner");
  }
}

void BSystemController::upgradeLand() {
  if (isGameOver)
    return;
  auto& presentLand = lands[players[presentPlayer].location];
  if (presentLand.owner == presentPlayer && players[presentPlayer].wealth >= 2000 &&
    presentLand.level < 3) {
    presentLand.level += 1;
    players[presentPlayer].wealth -= 2000;
    GameMainScene::getInstance()->log("Successfully Upgrade!");
    GameMainScene::getInstance()->updatePlayerState(presentPlayer);
  }
  else {
    GameMainScene::getInstance()->log("Failed! \nDue to insufficient funds, land ownership or current level of land.");
  }
}

void BSystemController::sellLand() {
  if (isGameOver)
    return;
  auto& presentLand = lands[players[presentPlayer].location];
  if (presentLand.owner == presentPlayer) {
    presentLand.owner = -1;
    int presentLocation = players[presentPlayer].location;
    for(auto iter = players[presentPlayer].lands.begin();
      iter != players[presentPlayer].lands.end();
      ++iter) {
      if (*iter == presentLocation) {
        players[presentPlayer].lands.erase(iter);
        break;
      }
    }
    auto gain = (int)((presentLand.price + presentLand.level * 2000) * 0.5);
    players[presentPlayer].wealth += gain;
    string temp = "You gained ";
    temp += std::to_string(gain);
    temp += " for selling.";
    GameMainScene::getInstance()->log(temp);
    GameMainScene::getInstance()->updatePlayerState(presentPlayer);
  }
  else {
    GameMainScene::getInstance()->log("You're not able to sell others' land...");
  }
}

bool BSystemController::isRolled() {
  return rollNumer != 0;
}

void BSystemController::nextPlayer() {
  if (isGameOver)
    return;
  do {
    presentPlayer = (presentPlayer + 1) % 4;
  } while(!players[presentPlayer].isAlive);

  rollNumer = 0;
  skillUsed = false;

  GameMainScene::getInstance()->RoundStart();
}

void BSystemController::gameOver() {
  isGameOver = true;
  int wealths[4], houses[4];
  for (int i = 0; i < 4; ++i) {
    wealths[i] = players[i].wealth;
    houses[i] = 0;
    for (auto iter : players[i].lands) {
      houses[i] += lands[iter].price + 2000 * lands[iter].level;
    }
  }
  int orders[4] = { 0, 1, 2, 3 };
  for (auto i = 0; i < 4; ++i) {
    for (auto t = 0; t < 3; ++t) {
      if (players[orders[t]].order > players[orders[t + 1]].order) {
        int temp = orders[t];
        orders[t] = orders[t + 1];
        orders[t + 1] = temp;
      }
      else if (players[orders[t]].order == players[orders[t + 1]].order)
      {
        if (wealths[orders[t]] + houses[orders[t]] < wealths[orders[t + 1]] + houses[orders[t + 1]]) {
          int temp = orders[t];
          orders[t] = orders[t + 1];
          orders[t + 1] = temp;
        }
      }
    }
  }

  string result = "GameOver!\n";
  result += "===Rank===\n";
  result += "1st: Player " + std::to_string(orders[0]) + "\n";
  result += "\tWealth: " + std::to_string(wealths[orders[0]]) + "\n";
  result += "\tWorth of Houses: " + std::to_string(houses[orders[0]]) + "\n";
  result += "2nd: Player " + std::to_string(orders[1]) + "\n";
  result += "\tWealth: " + std::to_string(wealths[orders[1]]) + "\n";
  result += "\tWorth of Houses: " + std::to_string(houses[orders[1]]) + "\n";
  result += "3rd: Player " + std::to_string(orders[2]) + "\n";
  result += "\tWealth: " + std::to_string(wealths[orders[2]]) + "\n";
  result += "\tWorth of Houses: " + std::to_string(houses[orders[2]]) + "\n";
  result += "4th: Player " + std::to_string(orders[3]) + "\n";
  result += "\tWealth: " + std::to_string(wealths[orders[3]]) + "\n";
  result += "\tWorth of Houses: " + std::to_string(houses[orders[3]]) + "\n";
  initial();
  Director::getInstance()->replaceScene(GameMainScene::createScene());
  GameMainScene::getInstance()->log(result);
}
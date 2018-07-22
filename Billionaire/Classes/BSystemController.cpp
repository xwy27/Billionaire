#include "BSystemController.h"
#include <string>
using namespace Emilia;
using std::string;

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
  sceneInstance = GameMainScene::getInstance();
  // aniInstance = AnimationController::getInstance();

  if (players == nullptr)
    players = new PlayerStatus[4];
  if (lands == nullptr)
    lands = new Land[MAX_LANDS_NUMBER];

  for (auto i = 0; i < 4; ++i)
    players[i].initial();
  
  for (auto i = 0; i < MAX_LANDS_NUMBER; ++i) {
    lands[i].initial();
    lands[i].price = 1000 + i * 47;
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
  rollNumer = random() % 6 + 1;
  sceneInstance->log("You rolled " + rollNumer);
  // aniInstance->playerMove(presentPlayer)
  players[presentPlayer].location += rollNumer;
  players[presentPlayer].location %= MAX_LANDS_NUMBER;
  return rollNumer;
}

void BSystemController::payCharge() {
  auto landOwner = lands[players[presentPlayer].location].owner;
  if (landOwner != -1 && landOwner != presentPlayer){
    players[presentPlayer].wealth -= lands[players[presentPlayer].location].getCharge();
    players[landOwner].wealth += lands[players[presentPlayer].location].getCharge();
    string temp = "Player ";
    temp += presentPlayer;
    temp += " pay ";
    temp += lands[players[presentPlayer].location].getCharge();
    temp += " to Player ";
    temp += landOwner;
    sceneInstance->log(temp);
    sceneInstance->updatePlayerState(presentPlayer);
    sceneInstance->updatePlayerState(landOwner);
  }

  if (players[presentPlayer].wealth < 0) {
    sceneInstance->log("You have no money to pay for the charge! You lose.");
    sceneInstance->playerLose();
  }
}

void BSystemController::useSkill() {
  if (skillUsed) {
    sceneInstance->log("You've used your skill!");
    return;
  }
  switch(skillID) {
    case 0:
      lands[players[presentPlayer].location].price *= 1.3;
      sceneInstance->log("The basic price of this land rise!");
    break;
    case 1:
      lands[players[presentPlayer].location].price /= 1.3;
      sceneInstance->log("The basic price of this land fall!");
    break;
    case 2:
      if (players[presentPlayer].wealth > 1000) {
        if (lands[players[presentPlayer].location].level > 0) {
          players[presentPlayer].wealth -= 1000;
          lands[players[presentPlayer].location].level -= 1;
          sceneInstance->log("You spent 1000 to destroy one building!");
        }
        else {
          sceneInstance->log("No building on this land...");
        }
      }
      else {
        sceneInstance->log("Insufficient fund...");
      }
    break;
    case 3:
      auto temp = lands[players[presentPlayer].location].price;
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
  auto& presentLand = lands[players[presentPlayer].location];
  if (presentLand.owner == -1) {
    if (presentLand.price <= players[presentPlayer].wealth) {
      players[presentPlayer].wealth -= presentLand.price;
      players[presentPlayer].lands.push_back(players[presentPlayer].location);
      presentLand.owner = presentPlayer;
      sceneInstance->log("Successfully bought!");
    }
    else {
      sceneInstance->log("Insufficient fund...");
    }
  }
  else {
    sceneInstance->log("This land has its owner");
  }
}

void BSystemController::upgradeLand() {
  auto& presentLand = lands[players[presentPlayer].location];
  if (presentLand.owner == presentPlayer && players[presentPlayer].wealth >= 2000 &&
    presentLand.level < 3) {
    presentLand.level += 1;
    players[presentPlayer].wealth -= 2000;
    sceneInstance->log("Successfully Upgrade!");
    sceneInstance->updatePlayerState(presentPlayer);
  }
  else {
    sceneInstance->log("Failed! Due to insufficient funds, land ownership or current level of land.");
  }
}

void BSystemController::sellLand() {
  auto& presentLand = lands[players[presentPlayer].location];
  if (presentLand.owner == presentPlayer) {
    presentLand.owner = -1;
    int presentLocation = players[presentPlayer].location;
    for(auto iter = players[presentPlayer].lands.begin();
      iter != players[presentPlayer].lands.end();
      ++iter) {
      if (*iter == presentLocation) {
        players[presentPlayer].lands.erase(iter);
      }
    }
    auto gain = (int)((presentLand.price + presentLand.level * 2000) * 0.5);
    players[presentPlayer].wealth += gain;
    string temp = "You gained ";
    temp += gain;
    temp += " for selling.";
    sceneInstance->log(temp);
  }
  else {
    sceneInstance->log("You're not able to sell others' land...");
  }
}

bool BSystemController::isRolled() {
  return rollNumer != 0;
}

void BSystemController::nextPlayer() {
  do {
    presentPlayer = (presentPlayer + 1) % 4;
  } while(!players[presentPlayer].isAlive);

  rollNumer = 0;
  skillUsed = false;

  sceneInstance->RoundStart();
}
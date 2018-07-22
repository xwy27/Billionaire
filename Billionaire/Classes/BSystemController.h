#ifndef B_SYSTEM_CONTROLLER
#define B_SYSTEM_CONTROLLER

#include <vector>
#include "GameMainScene.h"
#define MAX_LANDS_NUMBER 64
#define INIT_MONEY 10000

namespace Emilia {
  struct PlayerStatus {
    int wealth;
    int location;
    bool isAlive;
    std::vector<int> lands;
    // id is stored as 'index' + 1
    void initial() {
      wealth = INIT_MONEY;
      location = 0;
      isAlive = true;
      lands.clear();
    }
  };

  struct Land {
    int price;
    int level;
    int owner;

    void initial() {
      level = 0; // no level
      owner = -1; // no owner
    }

    int getCharge() {
      return price * 0.1 * pow(2, level);
    }
  };

  class BSystemController {
    private:
    
    static BSystemController* _instance;

    PlayerStatus* players = nullptr;
    Land* lands = nullptr;

    int presentPlayer;
    int rollNumer;
    int skillID;
    bool skillUsed;

    //GameMainScene* sceneInstance;
    // AnimationController* aniInstance;

    BSystemController();
    ~BSystemController();

    public:

    static BSystemController* getInstance();
    
    void initial();
    int getUserID();
    int newSkillID();
    int getMoney(int playerID);
    int* getLandsPrice();

    int roll();
    void payCharge();
    void useSkill();
    void buyLand();
    void upgradeLand();
    void sellLand();
    bool isRolled();
    void nextPlayer();

  };
}

#endif
# 游戏项目概况

### 开始游戏页面

开始游戏按钮
退出游戏按钮
背景音乐开关

### 游戏页面

#### 右侧栏

- 人物状态

  头像
  金钱

- CUI

  输出骰子点数／金钱变化（log）

#### 游戏区域

##### 右下角

- 卖房子
- 买房子／升级
- 技能

  每回合，blingbling 出技能图片，然后缩小移动到游戏区域的右下角

##### 左下角

- 骰子图片
- 结束回合

## 实现

### 场景模块(Scene)

```cpp
void GameStart(); // 调用动画模块／控制系统初始化方法
void RoundStart(){
  /* 显示当前玩家
   * getUserID()
   * 从游戏状态获取当前玩家id
   * 改底色
   */

  /*
    * updatePlayerState();
    */
  
  /* 技能 bling
   * newSkillID()
   * 从游戏状态获取技能id
   * skillBling();
   * 通知动画模块显示技能 bling 动画
   */
}

void playerLose(); // 玩家破产，底色变灰色

void updatePlayerState() {
  // 更新右上角玩家信息栏
  // 更新金钱 getMoney()
}

void log(string msg) {
  // 显示 msg 信息
}

void roundEnd() {
  if (扔了筛子 isRolled()) {
    // 当前玩家回合结束 nextPlayer()
  }
}
```

### 动画模块

控制人物移动／技能 bling

```cpp
void initial() {
  //放置人物
}

void skillBling();

void playerMove(int id, int pos, int steps) {
  // 移动
  // 回调 payCharge()
}
```

### 控制系统

控制游戏状态

- 游戏状态
  - 人物状态
    金钱／位置／房产
  - 地皮状态
    价格／升级次数／拥有者
  - 回合状态
    当前玩家／骰子点数(0标志未投)／技能id／技能释放标志

```cpp
void initial(); // 初始化游戏状态

int getUserID();
int newSkillID(); // random 一下
int getSkillID();
int getMoney(int id); // 获取玩家金钱

int roll() {
  // random 修改当前状态下的骰子点数
  // log(骰子点数)
  // playerMove(int id, int pos, int steps)
  // 更新人物位置
}

void payCharge(){
  // 给过路费
  // log()
  // 更新金钱
  // updatePlayerState(id) 扣钱
  // updatePlayerState(id) 加钱
  // 判断破产(yes的话，调用UI模块的playerLose(), log())
}

void useSkill() {
  // 更新技能释放标志
  // log()
}

void buyLand() {
  /* if (地无主 && 钱够) {
      更新当前地皮的所有者
      地皮id插入当前人物的房产列表
      扣钱
      log()
      updatePlayerState()
    }
  */
}

void upgradeLand() {
  /*
    if (地属于自己 && 钱够 && 地还能升级) {
      更新地皮升级次数
      扣钱
      log()
      updatePlayerState()
    }
   */
}

void sellLand() {
  /*
    if (地属于自己) {
      更新当前地皮的所有者
      地皮id从当前人物的房产列表中删除
      加钱
      log()
      updatePlayerState()
    }
  */
}

bool isRolled();

void nextPlayer() {
  // 重置为回合初始状态
  // roundStart()
}
```
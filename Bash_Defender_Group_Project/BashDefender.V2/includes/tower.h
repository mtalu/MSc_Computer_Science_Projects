#ifndef TEST_Tower_h
#define TEST_Tower_h

#include "../includes/projectiles.h"
#include "../includes/Sound.h"

typedef struct tower *Tower;
typedef struct towerGroup *TowerGroup;
typedef struct towerPos *TowerPos;
typedef struct towerPosNode *TowerPosNode;

/*needed to move these hash defines here for costOfUpgradeFactoringInTheUpgradesOnTheQueue in actionQueue mod - ben */
#define UPGRADES_PER_LEVEL 5 // the number of upgrades that can be done to a tower before its level increases
//#defines for upgrade amounts
#define DAMAGE_UPGR_VAL 5
#define SPEED_UPGR_VAL 5
#define RANGE_UPGR_VAL 10
#define AOE_RANGE_UPGR_VAL 10
#define AOE_POWER_UPGR_VAL 10
#define SLOW_POWER_UPGR_VAL 1
#define SLOW_DUR_UPGR_VAL 20
#define NUMBER_OF_TOWER_STATS 7


// tower positon handling
int isTowerPositionAvailable(int position);
void createTowerPos();
double scaleTowerPos(int coord, int scaleAxis, int scaleMax);
int getSpecifiedTowerPosY(int postion);
int getSpecifiedTowerPosX(int postion);
int maxTowerPosition();
char maxTowerPositionChar();
TowerPos getTowerPos(TowerPos tPos);
void makeAllTowPosAvailable();
void addTowerPosNode(int x, int y);
int getNumOfTowerPositions();

// tower pointer functions
void createTowerGroup();
TowerGroup getTowerGrp();
Tower getTowerID(int target);

// tower creation
int userCreateTower(int inputTowerPositionX, int inputTowerPositionY);
void initialiseNewTower(Tower newTow, int TowerPositionX, int TowerPositionY );
int createTowerFromPositions(int position);
void createTowerTypeFromPositions(int position, int tType);

// tower info returning functions
int getTowerType(int towerID);
int getTowerRange(int towerID);
int getTowerSpeed(int towerID);
int getTowerDamage(int towerID);
int getTowerAOErange(int towerID);
int getTowerAOEpower(int towerID);
int getTowerSlowPower(int towerID);
int getTowerSlowDuration(int towerID);
int getTowerLevel(int towerID);
int getTowerX(int towerID);
int getTowerY(int towerID);
unsigned int getNumberOfTowers();
int getTowerWidth(int towerID);
int getUpgradesCompleted(int target);
void getStats(int *towerType, int *range, int *damage, int *speed, int *AOEpower, int *AOErange, int *sp, int *sd, unsigned int towerID);
int checkCharType();

// upgrading tower functions
int upgradeDmg(int target);
int upgradeRange(int target);
int upgradeSpeed(int target);
int upgradeAOEpower(int target);
int upgradeAOErange(int target);
int upgradeSlowPower(int target);
int upgradeSlowDuration(int target);
int setTowerType(int towerID, int newType);
void makePostUpgradeChanges(int TowerID);
void assignCalculatedFiringType(int towerID);

// tower actions
void fire();

void findTarget(Tower currentTower);
void launchProjectile(Tower currentTower);


// presentation functions
void present_tower();
void drawAllTowerPositions();

// freeing functions
void freeTower(Tower t);
void freeAllTowers();
void freeTowerGroupStructure();
void freeAllTowerPositions();

// tower testing functions
int setTowerX(int towerID,int newX);
int setTowerY(int towerID, int newY);
int setTowerRange(int towerID, int newRange);
int setTowerDamage(int towerID, int newDamage);
void testingTowerModule();
void testGetTower();
int getLastTowerPositionY();
int getLastTowerPositionX();
void testTowerCreation();
int getTowerPositionX(int position);
int getTowerPositionY(int position);
void testingProjectiles();
void testProjectileHandling();
void testTowerFiring();
void resetTowerCooldown(int towerID);



#endif


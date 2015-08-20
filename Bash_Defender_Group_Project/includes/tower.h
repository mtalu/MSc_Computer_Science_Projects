#ifndef TEST_Tower_h
#define TEST_Tower_h

#include "../includes/projectiles.h"

typedef struct tower *tower;
typedef struct towerGroup *TowerGroup;
typedef struct towerPos *TowerPos;
typedef struct towerPosNode *TowerPosNode;

#define UPGRADES_PER_LEVEL 5 // the number of upgrades that can be done to a tower before its level increases

// tower positon handling
int isTowerPositionAvailable(int position);
void createTowerPos();
double scaleTowerPos(int coord, int scaleAxis, int scaleMax);
int getSpecifiedTowerPosY(int postion);
int getSpecifiedTowerPosX(int postion);
int maxTowerPosition();
char maxTowerPositionChar();
TowerPos getTowerPos(TowerPos tPos);
void addTowerPosNode(int x, int y);
int getNumOfTowerPositions();

// tower pointer functions
void createTowerGroup();
TowerGroup getTowerGrp();
tower getTowerID(int target);

// tower creation
int isTowerPositionAvailable(int position);
int userCreateTower(int inputTowerPositionX, int inputTowerPositionY);
int getTowerType(int towerID);
void initialiseNewTower(tower newTow, int TowerPositionX, int TowerPositionY );
int createTowerFromPositions(int position);
void createTowerTypeFromPositions(int position, int tType);

// tower info returning functions
int getTowerRange(int towerID);
int getTowerSpeed(int towerID);
int getTowerDamage(int towerID);
int getTowerAOErange(int towerID);
int getTowerAOEpower(int towerID);
int getTowerLevel(int towerID);
int getTowerX(int towerID);
int getTowerY(int towerID);
unsigned int getNumberOfTowers();
int getTowerWidth(int towerID);
int getUpgradesCompleted(int target);

// upgrading tower functions
int upgradeDmg(int target);
int upgradeRange(int target);
int upgradeSpeed(int target);
int upgradeAOEpower(int target);
int upgradeAOErange(int target);
int setTowerType(int towerID, int newType);
void makePostUpgradeChanges(int TowerID);
void assignCalculatedFiringType(int towerID);

// tower actions
void fire();
void findTarget(tower currentTower);
void launchProjectile(tower currentTower);
void getStats(int *towerType, int *range, int *damage, int *speed, int *AOEpower, int *AOErange, unsigned int towerID);

// presentation functions
void present_tower();
void drawAllTowerPositions();

// freeing functions
void freeTower(tower t);
void freeAllTowers();
void freeAllTowerPositions();

// tower testing functions
int setTowerX(int towerID,int newX);
int setTowerY(int towerID, int newY);
int setTowerRange(int towerID, int newRange);
int setTowerDamage(int towerID, int newDamage);
void testingTowerModule();
void testGetTower();
void testUpgradeTowerStat();
int getLastTowerPositionY();
int getLastTowerPositionX();
void testTowerCreation();
int createTowerFromPositions(int position);
void drawAllTowerPositions();
void createTowerTypeFromPositions(int position, int tType);
int getTowerPositionX(int position);
int getTowerPositionY(int position);
void makeAllTowPosAvailable();
void testingTowerPositions();
int checkCharType();
void testingProjectiles();
void testProjectileHandling();
void testTowerFiring();



#endif


#ifndef TEST_Enemy_h
#define TEST_Enemy_h

#include "../includes/gameProperties.h"
#include "../includes/Display.h"
#include "../includes/Sound.h"

// damage modifiers for types
#define TYPE_MATCH_MODIFIER 2  // multiply the damage by this much if the damage and enemy types match
#define TYPE_MISMATCH_MODIFIER 2 // divide the damage by this much if the damage and enemy types don't match

// enemy initial characteristics
#define DELAY_BETWEEN_ENEMIES	1

#define INT_BASIC_HEALTH 150
#define INT_BASIC_ARMOUR 0
#define INT_BASIC_SPEED	6
#define INT_BASIC_DAMAGE 10
#define INT_BASIC_HEIGHT 90
#define INT_BASIC_WIDTH 90

#define INT_HEAVY_HEALTH 700
#define INT_HEAVY_ARMOUR 1
#define INT_HEAVY_SPEED 3
#define INT_HEAVY_DAMAGE 30
#define INT_HEAVY_HEIGHT 120
#define INT_HEAVY_WIDTH 120

#define CHAR_BASIC_HEALTH 150
#define CHAR_BASIC_ARMOUR 0
#define CHAR_BASIC_SPEED 6
#define CHAR_BASIC_DAMAGE 10
#define CHAR_BASIC_HEIGHT 90
#define CHAR_BASIC_WIDTH 90

#define CHAR_HEAVY_HEALTH 700
#define CHAR_HEAVY_ARMOUR 1
#define CHAR_HEAVY_SPEED 3
#define CHAR_HEAVY_DAMAGE 30
#define CHAR_HEAVY_HEIGHT 120
#define CHAR_HEAVY_WIDTH 120

typedef struct path *Path;
typedef struct levelPaths *LevelPaths;
typedef struct enemy *Enemy;
typedef struct enemyGroup *EnemyGroup;



typedef enum typeOfEnemy	{

	intBasic = 1,
	intHeavy = 2,
	charBasic =3,
	charHeavy =4
} TypeOfEnemy;


typedef enum BOOL {FALSE, TRUE} BOOL;

// ENEMIES //

// enemy group handling functions
void       createEnemyGroup();
EnemyGroup getEnemyGroup(EnemyGroup enemyList);
void       freeEnemyGroup();

// enemy handling functions
Enemy createEnemy();
int   createSpecificEnemy(TypeOfEnemy eType, int lvl, int entranceNum);
void  initialiseEnemy(Enemy newEnemy, int lvl, int fam, TypeOfEnemy eType, int health, int armour, int speed, int damage, int height, int width);
void  freeEnemy(int enemyID);
void  freeAllEnemies();

// enemy movement/interaction functions
void moveEnemy(int enemyID);
void slowEnemy(int targetID, int slowPower, int slowDuration);
void damageEnemy(int damage, int enemyID, int damageType);
void killEnemy(int enemyID);
void present_enemy(Display d);

// enemy information returning functions
int  getNumberOfEnemies();
int  isDead(int enemyID);
int  inRange(int tX, int tY, int tRange, int enemyID);
void getProjectileTargetPos(int enemyID, int *targetCoords, int bulletMoves);
int  getEnemyHealth();
int  getEnemyArmour(int enemyID);


// PATHS //

// level paths structure handling functions
void       createLevelPaths();
LevelPaths getLevelPaths(LevelPaths pathList);
void       freeLevelPaths();

// path handling functions
void assignMemoryForPaths(int numberOfPaths);
void layPaths(int numberOfPaths, int levelNum);
void readInPath(int levelNum, int pathNum);
char *getFilePath(int levelNum, int pathNum);
void freePath(Path p);

// path information returning functions
int getNumberOfPaths();
int distanceToEndOfPath(int enemyID);


// TESTING //

// testing functions
void  Test_createEnemy();
Enemy createTestEnemy();
void  testInitialiseEnemy();
void  testEnemyMovement();
void  testSetEnemyPathNum(int enemyID, int pathNum);
void  test_KillAnEnemy();
void  layTestPath();
void  testEnemy();
int   setEnemyX(int enemyID, int newX);
int   setEnemyY(int enemyID, int newY);
void  setEnemyType(int enemyID, int newType);
int   setEnemyHealth(int enemyID, int newHealth);
int   setEnemyArmour(int enemyID, int newArmour);
void  printEnemy(int enemyID);






#endif

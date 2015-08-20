#ifndef TEST_Enemy_h
#define TEST_Enemy_h

#include "../includes/gameProperties.h"
#include "../includes/Display.h"

// damage modifiers for types
#define TYPE_MATCH_MODIFIER 2  // multiply the damage by this much if the damage and enemy types match
#define TYPE_MISMATCH_MODIFIER 2 // divide the damage by this much if the damage and enemy types don't match

// enemy initial characteristics
#define DELAY_BETWEEN_ENEMIES	1
#define INT_BASIC_HEALTH 100
#define INT_BASIC_ARMOUR 0
#define INT_BASIC_SPEED	2 
#define INT_BASIC_DAMAGE 10
#define INT_BASIC_HEIGHT 50
#define INT_BASIC_WIDTH 50

#define INT_HEAVY_HEALTH 1000
#define INT_HEAVY_ARMOUR 1
#define INT_HEAVY_SPEED 1
#define INT_HEAVY_DAMAGE 100
#define INT_HEAVY_HEIGHT 50
#define INT_HEAVY_WIDTH 50

#define CHAR_BASIC_HEALTH 100
#define CHAR_BASIC_ARMOUR 0
#define CHAR_BASIC_SPEED 2 
#define CHAR_BASIC_DAMAGE 10
#define CHAR_BASIC_HEIGHT 50
#define CHAR_BASIC_WIDTH 50

#define CHAR_HEAVY_HEALTH 1000
#define CHAR_HEAVY_ARMOUR 1
#define CHAR_HEAVY_SPEED 1
#define CHAR_HEAVY_DAMAGE 100
#define CHAR_HEAVY_HEIGHT 50
#define CHAR_HEAVY_WIDTH 50

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

void createEnemyGroup();
void freeEnemyGroup();
Enemy createEnemy();
int createSpecificEnemy(TypeOfEnemy eType, int lvl, int entranceNum);
int getNumberOfEnemies();
void Test_createEnemy();
EnemyGroup getEnemyGroup(EnemyGroup enemyList);
int getEnemyHealth();
void freeEnemy(int enemyID);
void freeAllEnemies();
void moveEnemy(int enemyID);
int isDead(int enemyID);
void killEnemy(int enemyID);
int inRange(int tX, int tY, int tRange, int enemyID);
void damageEnemy(int damage, int enemyID, int damageType);
void towerGetTargetPos(int * towerTargetPosition, int enemyID);
void printEnemy(int enemyID);
void createPath();
void freePath(Path p);
void freeLevelPaths();
int **getPathPointer(int **newPath);
void testEnemy();
int setEnemyX(int enemyID, int newX);
int setEnemyY(int enemyID, int newY);
int setEnemyHealth(int enemyID, int newHealth);
int setEnemyArmour(int enemyID, int newArmour);
void present_enemy(Display d);
void createLevelPaths();
LevelPaths getLevelPaths(LevelPaths pathList);
void assignMemoryForPaths(int numberOfPaths);

void layPaths(int numberOfPaths, int levelNum);
char *getFilePath(int levelNum, int pathNum);
void readInPath(int levelNum, int pathNum);
int getNumberOfPaths();

void initialiseEnemy(Enemy newEnemy, int lvl, int fam, TypeOfEnemy eType, int health, int armour, int speed, int damage, int height, int width);
int distanceToEndOfPath(int enemyID);

void getBulletTargetPos(int enemyID, int *targetCoords, int bulletMoves);
void layTestPath();

Enemy createTestEnemy();
void testInitialiseEnemy();
void testEnemyMovement();
void testSetEnemyPathNum(int enemyID, int pathNum);
void test_KillAnEnemy();


#endif

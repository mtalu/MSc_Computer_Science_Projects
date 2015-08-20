#ifndef TEST_Enemy_h
#define TEST_Enemy_h

#include "../includes/Display.h"

typedef struct enemy *Enemy;
typedef struct enemyGroup *EnemyGroup;

typedef enum BOOL {TRUE, FALSE} BOOL;
void createEnemyGroup();
void createEnemy();
int getNumberOfEnemies();
void Test_createEnemy();
void initialiseEnemy(Enemy newEnemy);
EnemyGroup getEnemyGroup(EnemyGroup enemyList);
int getEnemyHealth();
void freeEnemy(int enemyID);
int moveEnemy(int enemyID);
int isDead(int enemyID);
int inRange(int tX, int tY, int tRange, int enemyID);
void damageEnemy(int damage, int enemyID);
void towerGetTargetPos(int * towerTargetPosition, int enemyID);
void printEnemy(int enemyID);
int **createPathArray(int rows);
void freePathArray(int **path);
void layPath(int **path);
void createPath();
int **getPathPointer(int **newPath);
void testEnemy();
int setEnemyX(int enemyID, int newX);
int setEnemyY(int enemyID, int newY);
int setEnemyHealth(int enemyID, int newHealth);
void present_enemy(Display d);

#endif

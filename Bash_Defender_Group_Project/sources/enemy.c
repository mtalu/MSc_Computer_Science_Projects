#include <stdio.h>
#include <stdlib.h>
#include "../includes/sput.h"
#include <math.h>
#include <string.h>

#include "../includes/enemy.h"

struct path {
    int pathLength;
    int **pathCoords;
};

struct levelPaths {
    int numberOfPaths;
    Path *paths;
};

struct enemy {

    TypeOfEnemy eType;
    int eFamily;
    int level;
    int enemyID;
    
    int x, y;
    Path enemyPath;
    int pathProgress;
    int maxHealth;
    int health;
    int armour;
    int speed;
    int damage;
    
    int slowEffect;
    int slowEffectStepsRemaining;
    int poisonEffect;
    int poisonEffectStepsRemaining;

    int height;
    int width;

    int dead;
};

struct enemyGroup {
    int numberOfEnemies;
    Enemy * enemyArray;
};

/*
* mallocs memory for the pathList Structure
*/
void createLevelPaths()
{
    LevelPaths pathList = (LevelPaths) malloc(sizeof(*pathList));
    getLevelPaths(pathList);
  
}

/*
* frees all paths, and then frees the level path structure itself
*/
void freeLevelPaths()
{
    LevelPaths lP = getLevelPaths(NULL);
    for(int i = 0; i < lP->numberOfPaths; i++) {
        freePath(lP->paths[i]);
    }
    free(lP->paths);
    free(lP);
}

/*
* frees the array of a path structure and then the path structure itself
*/
void freePath(Path p)
{
    for(int i = 0; i < p->pathLength; i++) {
        free(p->pathCoords[i]);
    }
    free(p->pathCoords);
    free(p);
}

/*
* creates the specified number of path structures within the level paths structure
*/

void layPaths(int numberOfPaths, int levelNum)
{
  
    assignMemoryForPaths(numberOfPaths);
    
    for(int i = 1; i <= numberOfPaths; i++) {
      readInPath(levelNum, i);
    }
}

/*
* lays one straight, horizontal path for testing
*/
void layTestPath()
{
    assignMemoryForPaths(1);
    Path p = getLevelPaths(NULL)->paths[0];
    p->pathLength = 500;
    p->pathCoords = (int **) malloc(sizeof(int *) * p->pathLength);
    for(int i = 0; i < 500; i++) {
        p->pathCoords[i] = (int *)malloc(sizeof(int) * 2);
        p->pathCoords[i][0] = i;
        p->pathCoords[i][1] = 100;
    }
}
    

void readInPath(int levelNum, int pathNum) {
  
  char *filePath = getFilePath(levelNum, pathNum);
  FILE *fp = fopen(filePath, "r");
  if(fp == NULL) {
    fprintf(stderr,"****ERROR Unable to open path file at '%s' ****\n", filePath);
    exit(1);
  }
  LevelPaths lP = getLevelPaths(NULL);
  Path P = lP->paths[pathNum-1];
  
  int lastRowScanned = 0;
    int backW, backH;
  int x, y;
    getBackgroundDimensions(&backW, &backH);
  if(fscanf(fp,"%d\n",&P->pathLength) != 1) {
    fprintf(stderr,"****ERROR Unable to read path length from path file at '%s' ****\n", filePath);
    exit(1);
  }
  lastRowScanned++;
  
  P->pathCoords = (int **) malloc(sizeof(int *) * P->pathLength);
  for(int i = 0; i < P->pathLength; i++) {
    
    if(fscanf(fp,"%d,%d\n", &x, &y) != 2) {
      fprintf(stderr,"****ERROR Unable to read path coordinates in file at '%s', last row successfully scanned was %d ****\n", filePath, lastRowScanned);
      exit(1);
    }
    
    lastRowScanned++;
    x = (int)((double) x * ((double) SCREEN_WIDTH_GLOBAL/ (double) backW) );
    y = (int)((double) y * ((double) SCREEN_HEIGHT_GLOBAL/ (double) backH) );
  
    P->pathCoords[i] = (int *)malloc(sizeof(int) * 2);
    P->pathCoords[i][0] = x;
    P->pathCoords[i][1] = y;
  }
  
  fclose(fp);
  
}

/*
** returns file path of .txt to be opened and read to make a path
*/
char *getFilePath(int levelNum, int pathNum) {
  
  char levelNumStr[10];
  sprintf(levelNumStr, "%d", levelNum);
  
  char pathNumStr[10];
  sprintf(pathNumStr, "%d", pathNum);
  
  char *filePath = calloc((strlen("../data/levels/lvl_") + strlen(levelNumStr) + strlen("/paths/path_") +strlen(pathNumStr) + strlen(".txt") ), sizeof(char) );
  if(filePath == NULL) {
    fprintf(stderr,"Unable to calloc space for path filePath\n");
    exit(1);
  }
  
  strcat(filePath, "../data/levels/lvl_");
  strcat(filePath, levelNumStr);
  strcat(filePath, "/paths/path_");
  strcat(filePath, pathNumStr);
  strcat(filePath, ".txt");
  
  return filePath;
}

/*
* assigns memory in the level paths structure for the desired number of path structure pointers
*/
void assignMemoryForPaths(int numberOfPaths)
{

      LevelPaths lP = getLevelPaths(NULL);
      lP->numberOfPaths = numberOfPaths;
      lP->paths = (Path *)malloc(sizeof(Path) * numberOfPaths);
      if(lP->paths == NULL) {
          fprintf(stderr,"****ERROR malloc in create path pointers failed****\n");
          exit(1);
      }
      for(int i = 0; i < numberOfPaths; i++) {
          lP->paths[i] = (Path)malloc(sizeof(struct path));
      }
}

int getNumberOfPaths()	{
	return getLevelPaths(NULL)->numberOfPaths;
}

/*
* if passed NULL, returns static LevelPaths pointer. If passed pointer, reassigns static LevelPaths pointer.
*/
LevelPaths getLevelPaths(LevelPaths pathList)
{
    
    static LevelPaths lP;
    
    if(pathList != NULL) {
        lP = pathList;
    }
    
    return lP;
}

/*
* creates an empty structure to hold all enemies
*/
void createEnemyGroup()
{
    EnemyGroup enemyList = (EnemyGroup) malloc(sizeof(*enemyList));
    getEnemyGroup(enemyList);
    enemyList->enemyArray=malloc(sizeof(Enemy));
    enemyList->numberOfEnemies = 0;
}

/*
* frees up all enemies and enemy pointers from the enemy group, then frees the group structire itself
*/
void freeEnemyGroup()
{
    EnemyGroup enemyList =  getEnemyGroup(NULL);
    for(int i = 1; i <= enemyList->numberOfEnemies; i++) {
        free(enemyList->enemyArray[i]);
    }
    free(enemyList->enemyArray);
    free(enemyList);
}

void freeAllEnemies()
{
    EnemyGroup enemyList =  getEnemyGroup(NULL);
    for(int i = 1; i <= enemyList->numberOfEnemies; i++) {
        free(enemyList->enemyArray[i]);
    }
    enemyList->numberOfEnemies = 0;
}


/*
* creates a new blank enemy within the enemy list structure, updates the enemy list structure to reflect the new number of enemies and populates the enemy (currently running for standard enemy only)
*/
Enemy createEnemy()
{
    EnemyGroup enemyList =  getEnemyGroup(NULL);
    ++(enemyList->numberOfEnemies);
    
    enemyList->enemyArray = (Enemy*)realloc(enemyList->enemyArray, (enemyList->numberOfEnemies+1)*(sizeof(Enemy)));
    if(enemyList->enemyArray==NULL) {
	      printf("****ERROR realloc in createEnemy failed****\n");
	      exit(1);
    }
    
    enemyList->enemyArray[enemyList->numberOfEnemies]=(Enemy)malloc(sizeof(struct enemy));
    if( enemyList->enemyArray[enemyList->numberOfEnemies]==NULL) {
	      printf("****ERROR malloc in createEnemy failed****\n");
        exit(1);
    }
    
     // initially set health to 100. overwritten by InitialiseEnemy, but createEnemy() is used alone for testing
    enemyList->enemyArray[enemyList->numberOfEnemies]->health = 100;
    
    return enemyList->enemyArray[enemyList->numberOfEnemies];

}

/*
 returns the number of enemies that currently exist (dead & alive)
*/

int getNumberOfEnemies()
{
    return getEnemyGroup(NULL)->numberOfEnemies;
}

/*
* creates two enemies and checks their defaut values
*/
void Test_createEnemy()
{
    freeAllEnemies();

    createEnemy();
    sput_fail_unless(getNumberOfEnemies() == 1, "Valid: Number of enemies held in group is one.");
    
    sput_fail_unless(getEnemyHealth(getNumberOfEnemies()) == 100,"Valid: Enemy health is default." );

    createEnemy();
    sput_fail_unless(getNumberOfEnemies() == 2, "Valid: Number of enemies held in group is two.");
    sput_fail_unless(getEnemyHealth(getNumberOfEnemies()) == 100,"Valid: Enemy 2  health is default." );

}


int createSpecificEnemy(TypeOfEnemy eType, int lvl, int entranceNum) {

  Enemy e = createEnemy();
  switch (eType) {
    case intBasic :
      initialiseEnemy(e, lvl, INT_TYPE, eType, INT_BASIC_HEALTH, INT_BASIC_ARMOUR, INT_BASIC_SPEED, INT_BASIC_DAMAGE, INT_BASIC_HEIGHT, INT_BASIC_WIDTH);
      break;
    case intHeavy :
      initialiseEnemy(e, lvl, INT_TYPE, eType, INT_HEAVY_HEALTH, INT_HEAVY_ARMOUR, INT_HEAVY_SPEED, INT_HEAVY_DAMAGE, INT_HEAVY_HEIGHT, INT_HEAVY_WIDTH);
      break;
    case charBasic :
      initialiseEnemy(e, lvl, CHAR_TYPE, eType, CHAR_BASIC_HEALTH, CHAR_BASIC_ARMOUR, CHAR_BASIC_SPEED, CHAR_BASIC_DAMAGE, CHAR_BASIC_HEIGHT, CHAR_BASIC_WIDTH);
      break;
    case charHeavy :
      initialiseEnemy(e, lvl, CHAR_TYPE, eType, CHAR_HEAVY_HEALTH, CHAR_HEAVY_ARMOUR, CHAR_HEAVY_SPEED, CHAR_HEAVY_DAMAGE, CHAR_HEAVY_HEIGHT, CHAR_HEAVY_WIDTH);
      break;
    default :
      fprintf(stderr,"ERROR**** incorrect value for TypeOfEnemy (value = %d) passed to createSpecificEnemy() ****\n", eType);
      exit(1);
  }

  return e->enemyID;
}
    
    


/*
* populates relevant fields for standard enemies
*/                                            
void initialiseEnemy(Enemy newEnemy, int lvl, int fam, TypeOfEnemy eType, int health, int armour, int speed, int damage, int height, int width)
{
    LevelPaths lP = getLevelPaths(NULL);
    newEnemy->enemyPath = lP->paths[rand()%lP->numberOfPaths];
    newEnemy->pathProgress = 0;
    newEnemy->x = newEnemy->enemyPath->pathCoords[0][0];
    newEnemy->y = newEnemy->enemyPath->pathCoords[0][1];

    newEnemy->eFamily = fam;
    newEnemy->level = lvl;
    newEnemy->eType = eType;
    newEnemy->maxHealth = health*lvl;
    newEnemy->health = newEnemy->maxHealth;
    newEnemy->armour = armour*lvl;
    newEnemy->speed = speed;

    newEnemy->enemyID=getNumberOfEnemies();
    newEnemy->dead = 0;

    newEnemy->height = height;
    newEnemy->width = width;
    newEnemy->damage = damage*lvl;

}

/*
* manually sets enemy health. Used in testing.
*/
int setEnemyHealth(int enemyID, int newHealth)	{
	  getEnemyGroup(NULL)->enemyArray[enemyID]->health = newHealth;
	  return getEnemyGroup(NULL)->enemyArray[enemyID]->health;	
}                      

/*
* manually sets enemy armour. Used in testing.
*/
int setEnemyArmour(int enemyID, int newArmour)	{
	  getEnemyGroup(NULL)->enemyArray[enemyID]->armour = newArmour;
	  return getEnemyGroup(NULL)->enemyArray[enemyID]->armour;	
}   
/*
* returns pointer to the enemy group structure if input is NULL. If input is not NULL, reassigns pointer to the pointer passed to the function
*/
EnemyGroup getEnemyGroup(EnemyGroup enemyList)
{
    
    static EnemyGroup e;
    
    if(enemyList != NULL) {
        e = enemyList;
    }
    
    return e;
}

/*
* for each enemy, if not dead, calls the draw enemy function using their relevant information
*/
void present_enemy(Display d)
{
    EnemyGroup enemyList = getEnemyGroup(NULL);
    for(int i=1; i<=enemyList->numberOfEnemies; ++i)
    {
        Enemy e = enemyList->enemyArray[i];

        if(!isDead(i))
        {
            drawEnemy(e->x, e->y, e->width, e->height, 2010, 121, e->eType, 15, 200);
            drawRect(e->x, e->y -20, 0, 0, 80, 10, (double)e->health, (double)e->maxHealth);
        }
    }
}

/*
* returns the current health of the specified enemy. Used in testing
*/
int getEnemyHealth(int enemyIndex)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyIndex];
    return e->health;
}

/*
* frees contents of specified enemy structure
*/
void freeEnemy(int enemyID)
{
    free(getEnemyGroup(NULL)->enemyArray[enemyID]);
}

/*
* moves specified enemy along their assigned path by a set number of steps as designated by the enemy's speed characteristic
*/
void moveEnemy(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    if(!isDead(enemyID) ) {
        if(e->pathProgress < e->enemyPath->pathLength - e->speed) {
            e->pathProgress += e->speed;
            e->x = e->enemyPath->pathCoords[e->pathProgress][0];
            e->y = e->enemyPath->pathCoords[e->pathProgress][1];
        }
        else {
            damageHealth(e->damage); 
            e->dead = 1;
			      increaseDeathCnt();
        }
    }
}


/*
* manually set the enemy's x coord - used in testing
*/
int setEnemyX(int enemyID, int newX)
{
	  Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
	  e->x = newX;
	  return e->x;
}

/*
* manually set the enemy's y coord - used in testing
*/
int setEnemyY(int enemyID, int newY)
{
	  Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
	  e->y = newY;
	  return e->y;
}

/*
* Checks if specified enemy is dead. To be used before doing anything to the enemy (moving/firing etc.)
*/
int isDead(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    if(e->dead == 1) {
        return 1;
    }
    else {
        return 0;
    }
}

/*
* for a given tower co-ord and range, returns 1 if the specified enemy is in range. retruns 0 if out of range.
*/
int inRange(int tX, int tY, int tRange, int enemyID)
{
    if( enemyID > getNumberOfEnemies() )
    {
        fprintf(stderr,"ERROR: tower calling inRange on enemy that never existed");
        return -1;
    }
    
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    

    int distanceBetweenTowerAndEnemy = (int)sqrt( pow((double)(e->x+(e->width/2)-tX),2) +
                                              pow((double)(e->y+(e->height/2)-tY),2)    );
    if(distanceBetweenTowerAndEnemy<tRange){
        return 1;
    }
    else {
        return 0;
    }

}

/*
* Does the specified ammount of damage to the specified enemy. Reduces damage by the amount of armour the enemy has first.
* If damage reduces health to less than 0, kills enemy and adds memory equivalent to enemy's max health
*/
void damageEnemy(int damage, int enemyID, int damageType)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    if(!isDead(enemyID)) {
      int damageToBeDone;
      
        // modify the damge based on the type of enemy and type of damage
      if(e->eFamily == damageType) {
        damageToBeDone = (damage*TYPE_MATCH_MODIFIER) - e->armour;
      } else {
        damageToBeDone = (damage/TYPE_MISMATCH_MODIFIER) - e->armour;
      }
        
      if(damageToBeDone <= 0) {
        damageToBeDone = 0;
      }
      
      e->health -= damageToBeDone;
      if(e->health<=0 && e->dead != 1)
      {
		  increaseDeathCnt();
		      e->health=0;
          e->dead=1;
          addMemory(e->maxHealth);
          // drawDeath(e->x, e->y);
          //drawKillAll();
      }
    }
}

/*
* kills specified enemy
*/
void killEnemy(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    e->dead = 1;
    e->health = 0;
	increaseDeathCnt();
}


/*
* calculates how far the specified enemy is from the end of their path. Used for tower target aquisition.
*/
int distanceToEndOfPath(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    return e->enemyPath->pathLength - e->pathProgress;
}

/*
* when passed a pointer to a tower's target position array, updates this array to contain target x & y co-ords for specified enemy
*/
void towerGetTargetPos(int * towerTargetPosition, int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    

    towerTargetPosition[0] = e->x+(e->width/2);
    towerTargetPosition[1] = e->y+(e->height/2);
}

/*
* for a given number of moves a bullet will take to get to an enemy, sets the target coordinates to where the enemy will be when it gets there
*/
void getBulletTargetPos(int enemyID, int *targetCoords, int bulletMoves)
{
  Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
  
    //if it's not gonna hit it, target the end of the enemy's path
  if(distanceToEndOfPath(enemyID) <= bulletMoves*e->speed) {
    targetCoords[0] = e->enemyPath->pathCoords[e->enemyPath->pathLength-1][0] + (e->width/2);
    targetCoords[1] = e->enemyPath->pathCoords[e->enemyPath->pathLength-1][1] + (e->height/2);
  } else {
    int impactProgress = e->pathProgress + (e->speed * bulletMoves);
    targetCoords[0] = e->enemyPath->pathCoords[impactProgress][0] + (e->width/2);
    targetCoords[1] = e->enemyPath->pathCoords[impactProgress][1] + (e->height/2);
  }
}
    
    

/*
* function for unit testing - prints enemy stats
*/
void printEnemy(int enemyID)
{
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    printf("Enemy x = %d, enemy y = %d, enemy health = %d\n\n", e->x, e->y, e->health);
}

void testEnemy()
{
    SCREEN_WIDTH_GLOBAL = 1000;
    SCREEN_HEIGHT_GLOBAL = 1000;
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    sput_enter_suite("Test_createEnemy(): Creating valid enemies in enemy group");
    sput_run_test(Test_createEnemy);
    sput_leave_suite();
    
    sput_enter_suite("testInitialiseEnemy(): Creating valid initialised enemies");
    sput_run_test(testInitialiseEnemy);
    sput_leave_suite();
    
    sput_enter_suite("testEnemyMovement(): Moving an enemy");
    sput_run_test(testEnemyMovement);
    sput_leave_suite();
    
    sput_finish_testing();
}

Enemy createTestEnemy()
{
    Enemy e = createEnemy();
    int testEnemyLevel = 1;
    EnemyGroup eGroup = getEnemyGroup(NULL);
    if(eGroup == NULL) {
        fprintf(stderr,"Unable to find enemy group in testEnemyMovement()");
        exit(1);
    }
    
    initialiseEnemy(e, testEnemyLevel, INT_TYPE, intBasic, INT_BASIC_HEALTH, INT_BASIC_ARMOUR, INT_BASIC_SPEED, INT_BASIC_DAMAGE, INT_BASIC_HEIGHT, INT_BASIC_WIDTH);
    
    return e;
}

void testInitialiseEnemy()
{
    Enemy e = createTestEnemy();
    
    sput_fail_unless(e->health == INT_BASIC_HEALTH, "Valid: Enemy has correct starting health");
    sput_fail_unless(e->armour == INT_BASIC_ARMOUR, "Valid: Enemy has correct starting armour");
    sput_fail_unless(e->speed == INT_BASIC_SPEED, "Valid: Enemy has correct starting speed");
    sput_fail_unless(e->damage == INT_BASIC_DAMAGE, "Valid: Enemy has correct starting damage");
    sput_fail_unless(e->height == INT_BASIC_HEIGHT, "Valid: Enemy has correct starting height");
    sput_fail_unless(e->width == INT_BASIC_WIDTH, "Valid: Enemy has correct starting width");
    
    freeAllEnemies();
    
}

void testEnemyMovement()
{
    Enemy e = createTestEnemy();
    
    sput_fail_unless(distanceToEndOfPath(getNumberOfEnemies()) == e->enemyPath->pathLength, "Valid: Enemy has the correct number of steps to the end of its path");
    
    
    int enemyOriginX = e->x;
    int enemyOriginY = e->y;
    for(int i = 0; i < 200; i++) {
        moveEnemy(getNumberOfEnemies());
    }
    sput_fail_unless(e->x != enemyOriginX, "Valid: Enemy has changed X coordinate after 200 moves");
    sput_fail_unless(e->y != enemyOriginY, "Valid: Enemy has changed Y coordinate after 200 moves");
    sput_fail_unless(distanceToEndOfPath(getNumberOfEnemies()) == e->enemyPath->pathLength-(200*e->speed), "Valid: Enemy has the correct number of steps to the end of its path after 200 moves");
    
    int startingHealth = getHealth(getGame(NULL));
    int howFarToGo = distanceToEndOfPath(getNumberOfEnemies());
    int howManyMovesToEnd = (howFarToGo/e->speed)+1;
    for(int i = 0; i < howManyMovesToEnd; i++) {
        moveEnemy(getNumberOfEnemies());
    }
    
    
    sput_fail_unless(isDead(getNumberOfEnemies()), "Valid: Enemy has died after reaching end of path");
    sput_fail_unless(getHealth(getGame(NULL)) == startingHealth-e->damage, "Valid: Enemy has damaged health by correct amount at end of path");
    
}

/*
* puts specified enemy at start of specified path. Used in testing
*/
void testSetEnemyPathNum(int enemyID, int pathNum)
{
    LevelPaths lP = getLevelPaths(NULL);
    Enemy e = getEnemyGroup(NULL)->enemyArray[enemyID];
    
    e->enemyPath = lP->paths[pathNum];
    e->pathProgress = 0;
    e->x = e->enemyPath->pathCoords[0][0];
    e->y = e->enemyPath->pathCoords[0][1];
}

void test_KillAnEnemy()
{
    freeAllEnemies();
    createTestEnemy();
    damageEnemy(INT_BASIC_HEALTH, getNumberOfEnemies(), INT_TYPE);
}
    
/*
* main function for unit testing


int main()
{
    createLevelPaths();
    createEnemyGroup();
    createSpecificEnemy(charBasic, 1, 0);
    createSpecificEnemy(charBasic, 1, 0);
    createSpecificEnemy(charHeavy, 1, 0);
    for(int i = 0; i < 100; i++) {
      moveEnemy(3);
      printEnemy(3);
    }
}
*/

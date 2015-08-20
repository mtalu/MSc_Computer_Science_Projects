#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../includes/tower.h"
#include "../includes/sput.h"
#include "../includes/debug.h"

#define MAX_COOLDOWN 100 //  the longest number of ticks that a tower can take between shots

//#defines for tower type weighting
#define DAMAGE_MOD 9
#define SPEED_MOD 6
#define RANGE_MOD 3

//#defines for upgrade amounts
#define DAMAGE_UPGR_VAL 5
#define SPEED_UPGR_VAL 5
#define RANGE_UPGR_VAL 10


struct tower {
    int towerType;
    int towerID;
    int x, y;
    int damage;
    int range;
    int speed;
    int AOErange; //! not yet implemented
    int AOEpower; //!not yet implemented
    int targetID;
    int firing;
    int targetPosition[2];
    
    int level;
    int height;
    int upgradesCompleted;
    int width;
    
    FiringMethod firingType;
    int gunX;
    int gunY;
    
    int firingCoolDown;
};

struct towerGroup	{

	tower *listOfTowers;
	unsigned int numOfTowers;
	
};

struct towerPos	{

	int numberOfPositions;
	TowerPosNode *towerPositions;

};

struct towerPosNode	{
	tPosIcon tIcon;
	int x;
	int y;
	BOOL empty; 	// True is empty, false is full 

};




int getNumOfTowerPositions() {

    return getTowerPos(NULL)->numberOfPositions;
}

/*
 * Creates structure holding array of allowed tower positions
 */
void createTowerPos()	{

	TowerPos newPositions = (TowerPos) malloc(sizeof(*newPositions));
	newPositions->numberOfPositions = 0;
	newPositions->towerPositions = malloc(sizeof(TowerPosNode));
	getTowerPos(newPositions);
}

/*
 *Returns structure holding allowed tower positions
 */
TowerPos getTowerPos(TowerPos tPos)	{

	static TowerPos currTPos;

	if(tPos != NULL)	{
		currTPos = tPos;
	}

	return currTPos;
}

/*
 *Add a new tower position
 */
void addTowerPosNode(int x, int y)	{
		TowerPos tPos = getTowerPos(NULL);
		tPos->numberOfPositions++;
		tPos->towerPositions = (TowerPosNode*) realloc(tPos->towerPositions, (tPos->numberOfPositions+1)*(sizeof(*(tPos->towerPositions))));
		TowerPosNode newTower = (TowerPosNode) malloc(sizeof(*newTower));
		newTower->empty = TRUE; //! Tower position is available
		newTower->x = (int) scaleTowerPos(x,SCREEN_WIDTH_GLOBAL,MAX_TOWER_X);
		newTower->y = (int) scaleTowerPos(y,SCREEN_HEIGHT_GLOBAL,MAX_TOWER_Y);
		newTower->tIcon = tPos->numberOfPositions;
		tPos->towerPositions[tPos->numberOfPositions] = newTower;
}

void makeAllTowPosAvailable()	{
	TowerPos tPos = getTowerPos(NULL);
	int i;
	for(i = 1; i <= tPos->numberOfPositions; i++)	{
		tPos->towerPositions[i]->empty = TRUE;
	}
}

double scaleTowerPos(int coord, int scaleAxis, int scaleMax)	{
	return ((double) coord * ((double) scaleAxis/ (double) scaleMax) );
}

void drawAllTowerPositions()	{

	TowerPos tPos = getTowerPos(NULL);
	int t;
	for(t = 1; t <= tPos->numberOfPositions;t++)	{
		if(tPos->towerPositions[t]->empty == TRUE)	{
			drawTowerPosition(tPos->towerPositions[t]->x,tPos->towerPositions[t]->y,50,50,tPos->towerPositions[t]->tIcon);
		}
	}

}
void freeAllTowerPositions()	{
	
	TowerPos tPos = getTowerPos(NULL);
	int positions;
	for(positions = 1; positions <= tPos->numberOfPositions; positions++){
		free(tPos->towerPositions[positions]);
	}

	free(tPos);

}

void testingProjectiles()
{
    sput_start_testing();
	  sput_set_output_stream(stderr);

	  sput_enter_suite("testProjectileHandling(): Checking projectiles are created, moved and deleted from linked list");
	  sput_run_test(testProjectileHandling);
	  sput_leave_suite();
	  
  	sput_finish_testing();
}

void testProjectileHandling()
{
    
    freeAllEnemies();
    createSpecificEnemy(intBasic, 1, 1); //create 1 enemy
    int enemyOriginalHealth = getEnemyHealth(getNumberOfEnemies());
    
    int towerID = userCreateTower(5000,5000); //create standard tower at position 5000, 5000
    tower t = getTowerID(towerID);
    
    t->towerType = INT_TYPE;
    t->firingType = bullet;
    t->targetID = 1;
    towerGetTargetPos(t->targetPosition, t->targetID); //target the enemy
    
    for(int i = 0; i < 1000; i++) { // fire 1000 projectiles
      launchProjectile(t);
    }
    
    sput_fail_unless(test_numOfProjectiles() == 1000, "1000 bullets created, all are in projectile list");
    
    int firstMoveX, firstMoveY;
    firstMoveX = test_startingProjectileXcoord();
    firstMoveY = test_startingProjectileYcoord();
    
    moveProjectiles();
    sput_fail_unless(test_startingProjectileXcoord() != firstMoveX, "Bullets have changed X position on move");
    sput_fail_unless(test_startingProjectileYcoord() != firstMoveY, "Bullets have changed Y position on move");
    
    for(int i = 0; i < 10000; i++) {//move projectiles 10000 times
        moveProjectiles();
    }
    
    sput_fail_unless(test_numOfProjectiles() == 0, "After 10,000 moves all bullets have disappeared");
    sput_fail_unless(getEnemyHealth(getNumberOfEnemies()) < enemyOriginalHealth, "After 10,000 projectile moves, targeted enemy's heath has been depleted");
    sput_fail_unless(isDead(getNumberOfEnemies()) == 1, "After being hit by 1000 bullets, enemy is good and dead");
    
    freeAllEnemies();
    freeAllTowers();
}

void testTowerCreation()	{
	
	addTowerPosNode(100,200);
	sput_fail_unless(getLastTowerPositionX() == (int) scaleTowerPos(100,SCREEN_WIDTH_GLOBAL,MAX_TOWER_X), "Newly Added Tower position x coord is correct");
	sput_fail_unless(getLastTowerPositionY() == (int) scaleTowerPos(200,SCREEN_HEIGHT_GLOBAL,MAX_TOWER_Y), "Newly Added Tower position y coord is correct");
}

int getSpecifiedTowerPosX(int postion)	{
	TowerPos tPos = getTowerPos(NULL);
	return tPos->towerPositions[postion]->x;
}

int getSpecifiedTowerPosY(int postion)	{
	TowerPos tPos = getTowerPos(NULL);
	return tPos->towerPositions[postion]->y;
}

int getLastTowerPositionY()	{
	return getSpecifiedTowerPosY(getTowerPos(NULL)->numberOfPositions);
}

int getLastTowerPositionX()	{
	return getSpecifiedTowerPosX(getTowerPos(NULL)->numberOfPositions);
}

int getTowerRange(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->range;     
	}
	return 0;
}

int getTowerType(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->towerType;
	}
	return 0;
}
int getTowerSpeed(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->speed;   
	}	
	return 0;
}
int getTowerDamage(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->damage;     
	}
	return 0;
}
int getTowerAOErange(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->level;     
	}
	return 0;
}
int getTowerAOEpower(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->AOEpower;     
	}
	return 0;
}
int getTowerLevel(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->level;     
	}
	return 0;
}

void testingTowerModule()	{
	sput_start_testing();
	sput_set_output_stream(NULL);	

	sput_enter_suite("testGetTower(): Tower creation at valid positions and being placed in tower array");
	sput_run_test(testGetTower);
	sput_leave_suite();

	sput_enter_suite("testTowerCreation():  Checking they exist in group once created");
	sput_run_test(testTowerCreation);
	sput_leave_suite();
	
	sput_enter_suite("testTowerEnemyInteraction():  Checing firing on enemies is correct");
	sput_run_test(testTowerFiring);
	sput_leave_suite();

	sput_finish_testing();

}

void testTowerFiring()
{
    freeAllTowers();
    freeAllEnemies();
    TowerGroup TG = getTowerGrp(NULL);
    
    // create an enemy and tower within range of each other to check range calcs
    createTestEnemy();
    setEnemyX(getNumberOfEnemies(), 40);
    setEnemyY(getNumberOfEnemies(), 40);
    
    userCreateTower(50,50);
    tower t = TG->listOfTowers[TG->numOfTowers];
    t->range = 20;
    
    findTarget(t);
    sput_fail_unless(t->firing == 1, "Valid: tower fires at enemy in range");
    
    // set enemy out of range
    t->range = 5;
    t->firing = 0;
    findTarget(t);
    sput_fail_unless(t->firing == 0, "Valid: tower not firing when no enemies in range");
    
    // set range to be massive, but kill the enemy
    t->range = 200000;
    t->firing = 0;
    killEnemy(getNumberOfEnemies());
    findTarget(t);
    sput_fail_unless(t->firing == 0, "Valid: tower not firing when in range enemy is dead");
    
    // try when there are no enemies
    freeAllEnemies();
    t->firing = 0;
    findTarget(t);
    sput_fail_unless(t->firing == 0, "Valid: tower not firing when no enemies created");
    
    // priority checking - create two enemies in range and see which one the tower fires at
    createTestEnemy();
    testSetEnemyPathNum(1, 0);
    createTestEnemy();
    testSetEnemyPathNum(2, 0);
    
    moveEnemy(2);
    findTarget(t);
    sput_fail_unless(t->targetID == 2, "Valid: tower targets enemy closest to firewall");
    
    // move enemy 1 in front of enemy 2
    moveEnemy(1);
    moveEnemy(1);
    
    t->firingCoolDown = 0;
    t->firingType = bullet;
    fire();
    fire();
    sput_fail_unless(test_numOfProjectiles() == 1, "Valid: tower creates one projectile when fired twice within cooldown period");
    
    t->firingType = laser;
    for(int i = 0; i < (MAX_COOLDOWN-t->speed); i++) {
        fire();
    }
    t->firingType = missile;
    for(int i = 0; i < (MAX_COOLDOWN-t->speed) + 1; i++) {
        fire();
    }
    
    sput_fail_unless(test_numOfProjectiles() == 3, "Valid: tower has created three projectiles after firing three times");
    sput_fail_unless(test_checkStartingProjectileTarget() == 1, "Valid: created projrctiles are targetting correct enemy");
}
    

void createTowerGroup()	{

	TowerGroup Group = (TowerGroup) malloc(sizeof(*Group));
	getTowerGrp(Group);
	Group->listOfTowers=malloc(sizeof(tower));
	Group->numOfTowers = 0;
}

/*
 *Wrapper to pass in allowed tower positions to useCreateTower function
 */
int createTowerFromPositions(int position)	{
	TowerPos tPos = getTowerPos(NULL);
	if((position > 0) && (position <= tPos->numberOfPositions) && (tPos->towerPositions[position]->empty == TRUE))	{
		printf("###can create tower###\n");
		userCreateTower(tPos->towerPositions[position]->x,tPos->towerPositions[position]->y);
		tPos->towerPositions[position]->empty = FALSE;
		return 1;
	}
	return 0;
}


int getTowerPositionX(int position)	{
	return getTowerPos(NULL)->towerPositions[position]->x;
}

int getTowerPositionY(int position)	{
	return getTowerPos(NULL)->towerPositions[position]->y;
}


/*
* wrapper for createTowerFromPositions - adds tower type to creation process
*/
void createTowerTypeFromPositions(int position, int tType)	{
	TowerGroup TG = getTowerGrp(NULL);
	createTowerFromPositions(position);
	TG->listOfTowers[TG->numOfTowers]->towerType = tType;
}

char maxTowerPositionChar()
{
    TowerPos tPos = getTowerPos(NULL);
    char positionChar = tPos->numberOfPositions + 'a' - 1;
    return toupper(positionChar);
}
int maxTowerPosition() {
    TowerPos tPos = getTowerPos(NULL);
    return  tPos->numberOfPositions;
}

int isTowerPositionAvailable(int position)	{
	TowerPos tPos = getTowerPos(NULL);
    if( position <= tPos->numberOfPositions ) {
        return tPos->towerPositions[position]->empty;
    }
    else {
        return 0;
    }
}

/*
* changes the type of the tower (int/char) to the specified type. Returns 1 if successful, 0 if tower ID doesn't exist.
*/
int setTowerType(int towerID, int newType)
{
  tower t;
  if((t = getTowerID(towerID)) == NULL) {
    return 0;
  }
  else {
    t->towerType = newType;
    return 1;
  }
}
  

/* called when create tower command input by player. Places a tower at the specified x y.
    returns total number of towers if succesful
    returns 0 if failled
 */
int userCreateTower(int inputTowerPositionX, int inputTowerPositionY)
{
    TowerGroup TG = getTowerGrp(NULL);

    TG->numOfTowers++; //!increased number of towers when one is created
    TG->listOfTowers = realloc(TG->listOfTowers, (TG->numOfTowers+1)*sizeof(tower));
    if(TG->listOfTowers==NULL)
    {
        fprintf(stderr,"ERROR: createTower() \n towergroup array realloc failed\n");
        return 0;
    }
    tower t = malloc(sizeof(*t));
    if(t==NULL)
    {
        fprintf(stderr,"ERROR: createTower()\n tower  malloc failed\n");
        return 0;
    }
    TG->listOfTowers[TG->numOfTowers] = t;
    initialiseNewTower(t, inputTowerPositionX, inputTowerPositionY);
    
    
    return TG->numOfTowers;
    
}



void initialiseNewTower(tower newTow, int TowerPositionX, int TowerPositionY )
{
    TowerGroup TG = getTowerGrp(NULL);

    newTow->towerID = TG->numOfTowers;//new tower ID is the same as the number of towers in the group
    newTow->x = TowerPositionX;
    newTow->y = TowerPositionY;
    newTow->towerType = INT_TYPE;

    newTow->upgradesCompleted = 0;

    newTow->damage = 20;
    newTow->range = 100;
    newTow->firing = 0;
	  newTow->level = 1;
    newTow->speed = 50;
    newTow->AOEpower = 10;
    newTow->AOErange = 10;
    newTow->height = 50;
    newTow->width = 50;
    newTow->gunX = 23;
    newTow->gunY = 18;
    newTow->firingCoolDown = 0;
    assignCalculatedFiringType(newTow->towerID);
    
}

void assignCalculatedFiringType(int towerID) {

  tower t = getTowerGrp(NULL)->listOfTowers[towerID];
  
  if((t->damage * DAMAGE_MOD) > (t->speed * SPEED_MOD) && (t->damage * DAMAGE_MOD) > (t->range * RANGE_MOD) ) {
    t->firingType = bullet;
  } else {
    if ((t->speed * SPEED_MOD) > (t->damage * DAMAGE_MOD) && (t->speed * SPEED_MOD) > (t->range * RANGE_MOD) ) {
      t->firingType = laser;
    } else {
      t->firingType = missile;
    }
  }
}

void makePostUpgradeChanges(int TowerID) {
  
  tower t = getTowerGrp(NULL)->listOfTowers[TowerID];
  
  assignCalculatedFiringType(TowerID);
  
  t->upgradesCompleted++;
  if(t->upgradesCompleted % UPGRADES_PER_LEVEL == 0) {
    t->level++;
  }
}
      
int getUpgradesCompleted(int target)
{
    return getTowerGrp(NULL)->listOfTowers[target]->upgradesCompleted;
}
/*
 * Must be called and created before towers are created.
 */
TowerGroup getTowerGrp(TowerGroup Group)	{

	static TowerGroup newGroup;

	if(Group != NULL)	{
		newGroup = Group;
	}

	return newGroup;
}

int upgradeDmg(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->damage+=DAMAGE_UPGR_VAL;
    	makePostUpgradeChanges(target);
    	return upgradeT->damage;
	}
	return 0;
}
int upgradeRange(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->range+=RANGE_UPGR_VAL;
    	makePostUpgradeChanges(target);
    	return upgradeT->range;
	}
	return 0;
}
int upgradeSpeed(int target)
{
	
	  tower upgradeT;
	  if((upgradeT = getTowerID(target))!= NULL)	{
	      if(upgradeT->speed + SPEED_UPGR_VAL < MAX_COOLDOWN) { //make sure speed doesn't overflow cooldown value
		        upgradeT->speed+=SPEED_UPGR_VAL;
            makePostUpgradeChanges(target);
            return upgradeT->speed;
        }
	  }
	  return 0;
}
int upgradeAOEpower(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->AOEpower++;
    makePostUpgradeChanges(target);
    return upgradeT->AOEpower;
	}
	return 0;
}
int upgradeAOErange(int target)
{
	
	tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->AOErange++;
    makePostUpgradeChanges(target);
    return upgradeT->AOErange;
	}
	return 0;
}


unsigned int getNumberOfTowers()	{
	return ((getTowerGrp(NULL))->numOfTowers);
}

int checkCharType()	{
	int i = 1;
	while(i <= getTowerGrp(NULL)->numOfTowers)	{
		if(getTowerGrp(NULL)->listOfTowers[i]->towerType == CHAR_TYPE) {
			return 1;
		}
		i++;
	}
	return 0;
}
void freeAllTowers()	{

	int i = 1;
	makeAllTowPosAvailable();
	while(i <= getTowerGrp(NULL)->numOfTowers)	{
		free(getTowerGrp(NULL)->listOfTowers[i]);
		i++;
	}
	
	getTowerGrp(NULL)->numOfTowers = 0;
}

void testGetTower()	{

    freeAllTowers();
	  createTowerFromPositions(1);
	  sput_fail_unless(getNumberOfTowers() == 1, "Valid: Number of towers held in group is one.");
	  sput_fail_unless(getTowerID(1) != NULL,"Valid: Tower with ID 1 exists.");
	  createTowerFromPositions(2);
	  sput_fail_unless(getNumberOfTowers() == 2, "Valid: Number of towers held in group is two");
	  sput_fail_unless(getTowerID(2) != NULL,"Valid: Tower with ID 2 exists.");
	  freeAllTowers();
	  sput_fail_unless(getNumberOfTowers() == 0,"valid: All towers have been removed.");
	  sput_fail_unless(createTowerFromPositions(99) == 0, "Invalid: no tower position 99");
	  sput_fail_unless(createTowerFromPositions(0) == 0, "Invalid: no tower position 0");
	  sput_fail_unless(getNumberOfTowers() == 0,"valid: No Towers: previous attempts were invalid");
}

/*
 *Returns tower based on ID
 */
tower getTowerID(int target)	{
	int i;
	for( i = 1; i <= (getTowerGrp(NULL))->numOfTowers; i++)	{
		if((getTowerGrp(NULL))->listOfTowers[i]->towerID == target)	{
				return getTowerGrp(NULL)->listOfTowers[i];
		}
	}

	return NULL;
}


/*
* populates passed int pointers with specified tower stats. used in cat command (Information_Window.c)
*/
void getStats(int *towerType, int *range, int *damage, int *speed, int *AOEpower, int *AOErange, unsigned int towerID)
{
    TowerGroup towers = getTowerGrp(NULL);
     *range = towers->listOfTowers[towerID]->range;
     *damage = towers->listOfTowers[towerID]->damage;
     *speed = towers->listOfTowers[towerID]->speed;
     *AOEpower = towers->listOfTowers[towerID]->AOEpower;
     *AOErange = towers->listOfTowers[towerID]->AOErange;
     *towerType = towers->listOfTowers[towerID]->towerType;
}

int getTowerX(int towerID)
{
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->x;
}

int getTowerY(int towerID)
{
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->y;
}

int getTowerWidth(int towerID) {
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->width;
}


int setTowerY(int towerID, int newY)	{

	getTowerGrp(NULL)->listOfTowers[towerID]->y = newY;
	return newY;
	
}

int setTowerRange(int towerID, int newRange)	{

	getTowerGrp(NULL)->listOfTowers[towerID]->range = newRange;
	return newRange;

}

int setTowerDamage(int towerID, int newDamage)	{

	getTowerGrp(NULL)->listOfTowers[towerID]->damage = newDamage;
	return newDamage;

}

int setTowerX(int towerID,int newX)	{

	getTowerGrp(NULL)->listOfTowers[towerID]->x = newX;
	return newX;

}

void freeTower(tower t) {
  free(t);
}

void fire()
{
	  int towerID;

	  for(towerID = 1; towerID <= getNumberOfTowers(); towerID++)	{
	      tower currentTower = getTowerID(towerID);
	      currentTower->firing = 0;
	    
	      // check cooldown to see if tower is ready to fire
	      if(currentTower->firingCoolDown > 0) {
	          currentTower->firingCoolDown--;
	      } else {
	          findTarget(currentTower);
            if(currentTower->firing == 1) {
	              launchProjectile(currentTower);
            }   
        }
    }
}

void findTarget(tower currentTower)
{
    int enemyID;
    
    for(enemyID = 1; enemyID <= getNumberOfEnemies(); enemyID++)	{
        if (!isDead(enemyID) ) {
            if(inRange(currentTower->x + (currentTower->width/2), currentTower->y + (currentTower->height/2), currentTower->range, enemyID) == 1) {
                // if first enemy encountered, fire at it
                if(currentTower->firing == 0) {
                    currentTower->firing = 1;
                    currentTower->targetID = enemyID;
                }
                // else, compare with current target to choose closest to end of path
                else {
                    if(distanceToEndOfPath(enemyID) < distanceToEndOfPath(currentTower->targetID) ) {
                        currentTower->targetID = enemyID;
                    }
                }
            }
        }
    }
}

void launchProjectile(tower currentTower)
{
    currentTower->firingCoolDown = MAX_COOLDOWN - currentTower->speed;
    towerGetTargetPos(currentTower->targetPosition, currentTower->targetID);
    
    switch (currentTower->firingType) {
        case laser :
            fireLaser(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType);
            break;
        case missile :
            launchMissile(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType);
            break;
        case bullet :
            launchBullet(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType);
            break;
    }

}
  

void present_tower()
{
    TowerGroup TG = getTowerGrp(NULL);
    if(TG->numOfTowers>0)
    {
        for(int towerID=1; towerID<=TG->numOfTowers; ++towerID)
        {
            tower currentTower = getTowerID(towerID);
            drawTower(currentTower->x, currentTower->y, currentTower->width,currentTower->height, currentTower->towerType, currentTower->range,
                      8/*frames*/, 300/*anim_speed*/, 2080 /*pic_width*/, 258/*pic_height*/);
        }
    }
    // bullets added here temporarily
    moveProjectiles();
    drawProjectiles();
}

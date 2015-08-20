#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../includes/tower.h"
#include "../includes/sput.h"
#include "../includes/debug.h"

#define MAX_COOLDOWN 100 //  the longest number of ticks that a tower can take between shots
#define MAX_SPEED 80 // the highest speed of a tower

//#defines for starting tower stats
#define TOWER_STARTING_DAMAGE 20
#define TOWER_STARTING_RANGE 100
#define TOWER_STARTING_SPEED 50
#define TOWER_STARTING_AOE_POWER 10
#define TOWER_STARTING_AOE_RANGE 40
#define TOWER_STARTING_SLOW_POWER 0
#define TOWER_STARTING_SLOW_DURATION 40

//#defines for tower type weighting
#define DAMAGE_MOD 9
#define SPEED_MOD 8
#define RANGE_MOD 3

struct tower {
    int towerType;
    int towerID;
    int x, y;
    int damage;
    int range;
    int speed;
    int targetID;
    int firing;
    int targetPosition[2];
    
    int AOErange;
    int AOEpower;
    
    int slowPower;
    int slowDuration;
    
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

	Tower *listOfTowers;
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


////////////////////////////////////////////////////////////////////////////////////////////////////
// TOWER POSITION HANDLING FUNCTIONS
////////////////////////////////////

/** 
Whether a specified tower position is available for a tower to be placed there
@returns content of towerPosition[position]->empty, or 0 if tower position specified is higher than the number of positions
*/
int isTowerPositionAvailable(int position)	{
	TowerPos tPos = getTowerPos(NULL);
    if( position <= tPos->numberOfPositions ) {
        return tPos->towerPositions[position]->empty;
    }
    else {
        return 0;
    }
}

/**
Creates structure holding array of allowed tower positions
*/
void createTowerPos()	{

	TowerPos newPositions = (TowerPos) malloc(sizeof(*newPositions));
	newPositions->numberOfPositions = 0;
	newPositions->towerPositions = malloc(sizeof(TowerPosNode));
	getTowerPos(newPositions);
}

/**
Adjusts a tower's position to be of the correct scale in any window
@returns double - tower position after scaling
*/
double scaleTowerPos(int coord, int scaleAxis, int scaleMax)	{
	return ((double) coord * ((double) scaleAxis/ (double) scaleMax) );
}

/**
Returns the x coordinate of the specified tower position
*/
int getSpecifiedTowerPosX(int postion)	{
	TowerPos tPos = getTowerPos(NULL);
	return tPos->towerPositions[postion]->x;
}

/**
Returns the y coordinate of the specified tower position
*/
int getSpecifiedTowerPosY(int postion)	{
	TowerPos tPos = getTowerPos(NULL);
	return tPos->towerPositions[postion]->y;
}

/**
Returns integer value of the highest tower position
*/
int maxTowerPosition() {
    TowerPos tPos = getTowerPos(NULL);
    return  tPos->numberOfPositions;
}

/**
Returns char value of the highest tower position (1 = A, 3 = C etc.)
*/
char maxTowerPositionChar()
{
    TowerPos tPos = getTowerPos(NULL);
    char positionChar = tPos->numberOfPositions + 'a' - 1;
    return toupper(positionChar);
}

/**
Returns static pointer to structure holding allowed tower positions if passed NULL. If passed pointer, updates static pointer
*/
TowerPos getTowerPos(TowerPos tPos)	{

	static TowerPos currTPos;

	if(tPos != NULL)	{
		currTPos = tPos;
	}

	return currTPos;
}

/**
Sets all tower positions' empty statistic to be true
*/
void makeAllTowPosAvailable()	{
	TowerPos tPos = getTowerPos(NULL);
	int i;
	for(i = 1; i <= tPos->numberOfPositions; i++)	{
		tPos->towerPositions[i]->empty = TRUE;
	}
}

/**
Add a new tower position at specified x, y coordinates
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

/**
Returns int - the total number of tower positions that have been created
*/
int getNumOfTowerPositions() {

    return getTowerPos(NULL)->numberOfPositions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// TOWER POINTER FUNCTIONS
//////////////////////////

/**
Creates to towerGroup structure and assigns a static pointer to that group
*/
void createTowerGroup()	{

	TowerGroup Group = (TowerGroup) malloc(sizeof(*Group));
	getTowerGrp(Group);
	Group->listOfTowers=malloc(sizeof(Tower));
	Group->numOfTowers = 0;
}

/**
Returns static pointer to structure holding all towers if passed NULL. If passed pointer, updates static pointer
*/
TowerGroup getTowerGrp(TowerGroup Group)	{

	static TowerGroup newGroup;

	if(Group != NULL)	{
		newGroup = Group;
	}

	return newGroup;
}

/**
Returns pointer to specified tower
@param int - target tower ID
@return tower pointer
*/
Tower getTowerID(int target)	{
	int i;
	for( i = 1; i <= (getTowerGrp(NULL))->numOfTowers; i++)	{
		if((getTowerGrp(NULL))->listOfTowers[i]->towerID == target)	{
				return getTowerGrp(NULL)->listOfTowers[i];
		}
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// TOWER CREATION FUNCTIONS
///////////////////////////


/**
Called when create tower command input by player. Places a tower at the specified x y.
@return returns total number of towers if succesful, returns 0 if failled
*/
int userCreateTower(int inputTowerPositionX, int inputTowerPositionY)
{
    TowerGroup TG = getTowerGrp(NULL);

    TG->numOfTowers++; //!increased number of towers when one is created
    TG->listOfTowers = realloc(TG->listOfTowers, (TG->numOfTowers+1)*sizeof(Tower));
    if(TG->listOfTowers==NULL)
    {
        fprintf(stderr,"ERROR: createTower() \n towergroup array realloc failed\n");
        return 0;
    }
    Tower t = malloc(sizeof(*t));
    if(t==NULL)
    {
        fprintf(stderr,"ERROR: createTower()\n tower  malloc failed\n");
        return 0;
    }
    TG->listOfTowers[TG->numOfTowers] = t;
    initialiseNewTower(t, inputTowerPositionX, inputTowerPositionY);
    
    
    return TG->numOfTowers;
    
}

/**
Populates the specified tower structure with the default values and specified x and y coordinates
@param new tower pointer, int new tower x, int new tower y
*/
void initialiseNewTower(Tower newTow, int TowerPositionX, int TowerPositionY )
{
    TowerGroup TG = getTowerGrp(NULL);

    newTow->towerID = TG->numOfTowers;//new tower ID is the same as the number of towers in the group
    newTow->x = TowerPositionX;
    newTow->y = TowerPositionY;
    newTow->towerType = INT_TYPE;

    newTow->upgradesCompleted = 0;

    newTow->damage = TOWER_STARTING_DAMAGE;
    newTow->range = TOWER_STARTING_RANGE;
    newTow->firing = 0;
	  newTow->level = 1;
    newTow->speed = TOWER_STARTING_SPEED;
    newTow->AOEpower = TOWER_STARTING_AOE_POWER;
    newTow->AOErange = TOWER_STARTING_AOE_RANGE;
    newTow->slowPower = TOWER_STARTING_SLOW_POWER;
    newTow->slowDuration = TOWER_STARTING_SLOW_DURATION;
    newTow->height = 50;
    newTow->width = 50;
    newTow->gunX = 23;
    newTow->gunY = 18;
    newTow->firingCoolDown = 0;
    newTow->firingType = laser;
    
}

/**
Wrapper to pass in allowed tower positions to useCreateTower function
@param int - desired tower position
@return 1 if position present and available, 0 if not
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

/**
wrapper for createTowerFromPositions - adds tower type to creation process
@param int desired position, int new tower type
*/
void createTowerTypeFromPositions(int position, int tType)	{
	TowerGroup TG = getTowerGrp(NULL);
	createTowerFromPositions(position);
	TG->listOfTowers[TG->numOfTowers]->towerType = tType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// TOWER INFO RETURNING FUNCTIONS
/////////////////////////////////

/**
Returns specified tower's type
*/
int getTowerType(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->towerType;
	}
	return 0;
}
/**
Returns specified tower's range stat
*/
int getTowerRange(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->range;     
	}
	return 0;
}
/**
Returns specified tower's speed stat
*/
int getTowerSpeed(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->speed;   
	}	
	return 0;
}
/**
Returns specified tower's damage stat
*/
int getTowerDamage(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->damage;     
	}
	return 0;
}
/**
Returns specified tower's AOE range stat
*/
int getTowerAOErange(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->level;     
	}
	return 0;
}
/**
Returns specified tower's AOE power stat
*/
int getTowerAOEpower(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->AOEpower;     
	}
	return 0;
}
/**
Returns specified tower's slowing power stat
*/
int getTowerSlowPower(int towerID) {
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->slowPower;     
	}
	return 0;
}
/**
Returns specified tower's slowing duration stat
*/
int getTowerSlowDuration(int towerID) {
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->slowDuration;     
	}
	return 0;
}
/**
Returns specified tower's level
*/
int getTowerLevel(int towerID)	{
	if(getNumberOfTowers() >=towerID)	{
		return getTowerID(towerID)->level;     
	}
	return 0;
}

/**
Returns specified tower's x coordinate
*/
int getTowerX(int towerID)
{
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->x;
}

/**
Returns specified tower's y coordinate
*/
int getTowerY(int towerID)
{
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->y;
}

/**
Returns the total number of towers in the game
*/
unsigned int getNumberOfTowers()	{
	return ((getTowerGrp(NULL))->numOfTowers);
}

/**
Returns specified tower's width in pixels
*/
int getTowerWidth(int towerID) {
    TowerGroup TG = getTowerGrp(NULL);
    return TG->listOfTowers[towerID]->width;
}

/**
Returns the number of upgrades that have been completed on a specified tower
*/
int getUpgradesCompleted(int target)
{
    return getTowerGrp(NULL)->listOfTowers[target]->upgradesCompleted;
}

/**
Populates passed int pointers with specified tower stats. used in cat command (Information_Window.c)
@param int *towerType, int *range, int *damage, int *speed, int *AOEpower, int *AOErange, unsigned int towerID
*/
void getStats(int *towerType, int *range, int *damage, int *speed, int *AOEpower, int *AOErange, int *sp, int *sd, unsigned int towerID)
{
    TowerGroup towers = getTowerGrp(NULL);
    *range = towers->listOfTowers[towerID]->range;
    *damage = towers->listOfTowers[towerID]->damage;
    *speed = towers->listOfTowers[towerID]->speed;
    *AOEpower = towers->listOfTowers[towerID]->AOEpower;
    *AOErange = towers->listOfTowers[towerID]->AOErange;
    *towerType = towers->listOfTowers[towerID]->towerType;
    *sp = towers->listOfTowers[towerID]->slowPower;
    *sd = towers->listOfTowers[towerID]->slowDuration;
}


/**
Runs through all towers and checks to see if any one of them is of type CHAR. Used in tutorial
*/
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// UPGRADING TOWER FUNCTIONS
////////////////////////////

/**
Upgrades specified tower's damage by a hash defined amount
@return new value of upgraded statistic if specified tower existis. If not, returns 0
*/
int upgradeDmg(int target)
{
	
	Tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->damage+=DAMAGE_UPGR_VAL;
    	makePostUpgradeChanges(target);
    	return upgradeT->damage;
	}
	return 0;
}

/**
Upgrades specified tower's range by a hash defined amount
@return new value of upgraded statistic if specified tower existis. If not, returns 0
*/
int upgradeRange(int target)
{
	
	Tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->range+=RANGE_UPGR_VAL;
    	makePostUpgradeChanges(target);
    	return upgradeT->range;
	}
	return 0;
}

/**
Upgrades specified tower's firing speed by a hash defined amount
@return new value of upgraded statistic if specified tower existis. If not, returns 0
*/
int upgradeSpeed(int target)
{
	
	  Tower upgradeT;
	  if((upgradeT = getTowerID(target))!= NULL)	{
	      if(upgradeT->speed + SPEED_UPGR_VAL <= MAX_SPEED) { //make sure speed doesn't overflow cooldown value
		        upgradeT->speed+=SPEED_UPGR_VAL;
            makePostUpgradeChanges(target);
            return upgradeT->speed;
        }
	  }
	  return 0;
}

/**
Upgrades specified tower's AOE power by a hash defined amount
@return new value of upgraded statistic if specified tower existis. If not, returns 0
*/
int upgradeAOEpower(int target)
{
	
	Tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->AOEpower+=AOE_POWER_UPGR_VAL;
    makePostUpgradeChanges(target);
    return upgradeT->AOEpower;
	}
	return 0;
}

/**
Upgrades specified tower's AOE range by a hash defined amount
@return new value of upgraded statistic if specified tower existis. If not, returns 0
*/
int upgradeAOErange(int target)
{
	
	Tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->AOErange+=AOE_RANGE_UPGR_VAL;
    makePostUpgradeChanges(target);
    return upgradeT->AOErange;
	}
	return 0;
}

/**
Upgrades specified tower's slow effect power by a hash defined amount
@return new value of upgraded statistic if specified tower existis. If not, returns 0
*/
int upgradeSlowPower(int target)
{
	
	Tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->slowPower+=SLOW_POWER_UPGR_VAL;
    makePostUpgradeChanges(target);
    return upgradeT->slowPower;
	}
	return 0;
}

/**
Upgrades specified tower's slow effect duration by a hash defined amount
@return new value of upgraded statistic if specified tower existis. If not, returns 0
*/
int upgradeSlowDuration(int target)
{
	
	Tower upgradeT;
	if((upgradeT = getTowerID(target))!= NULL)	{
		upgradeT->slowDuration+=SLOW_DUR_UPGR_VAL;
    makePostUpgradeChanges(target);
    return upgradeT->slowDuration;
	}
	return 0;
}

/**
Changes the type of the specified tower to the specified type (int/char). Returns 1 if successful, 0 if tower ID doesn't exist.
*/
int setTowerType(int towerID, int newType)
{
  Tower t;
  if((t = getTowerID(towerID)) == NULL) {
    return 0;
  }
  else {
    t->towerType = newType;
    return 1;
  }
}

/**
Makes all changes that need to be done after upgrade for the specified tower (increase total number of upgrades, maybe increase level, recalculate firing type)
*/
void makePostUpgradeChanges(int TowerID) {
  
  Tower t = getTowerGrp(NULL)->listOfTowers[TowerID];
  
  assignCalculatedFiringType(TowerID);
  
  t->upgradesCompleted++;
  if(t->upgradesCompleted % UPGRADES_PER_LEVEL == 0) {
    t->level++;
  }
}

/**
Uses the statistics of the specified tower to calculate its firing type and assigns this to the tower
*/
void assignCalculatedFiringType(int towerID) {

  Tower t = getTowerGrp(NULL)->listOfTowers[towerID];
  int damageUpgrades = (t->damage - TOWER_STARTING_DAMAGE)/DAMAGE_UPGR_VAL;
  int speedUpgrades = (t->speed - TOWER_STARTING_SPEED)/SPEED_UPGR_VAL;
  int rangeUpgrades = (t->range - TOWER_STARTING_RANGE)/RANGE_UPGR_VAL;
  
  if(damageUpgrades > rangeUpgrades && damageUpgrades > speedUpgrades) {
    t->firingType = bullet;
  } else {
    if (speedUpgrades > rangeUpgrades && speedUpgrades > damageUpgrades) {
      t->firingType = laser;
    } else {
      if (rangeUpgrades > speedUpgrades && rangeUpgrades > damageUpgrades) {
        t->firingType = missile;
      }
    }
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// TOWER ACTIONS FUNCTIONS
//////////////////////////

/**
Runs through every tower that exists and either decreases its firing cooldown or launches a projectile at an enemy (if in range)
*/
void fire()
{
	  int towerID;

	  for(towerID = 1; towerID <= getNumberOfTowers(); towerID++)	{
	      Tower currentTower = getTowerID(towerID);
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

/**
For the specified tower, scans through all enemies and assigns one as the tower's target (if any are in range) 
*/
void findTarget(Tower currentTower)
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

/**
Launches the correct type of projectile at the specified tower's current target enemy from the specified tower's x and y coordinates
*/
void launchProjectile(Tower currentTower)
{
    currentTower->firingCoolDown = MAX_COOLDOWN - currentTower->speed;
    
    switch (currentTower->firingType) {
        case laser :
            fireLaser(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType, currentTower->AOEpower, currentTower->AOErange, currentTower->slowPower, currentTower->slowDuration);

            towerSound(laserSound);
            break;
        case missile :
            launchMissile(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType, currentTower->AOEpower, currentTower->AOErange, currentTower->slowPower, currentTower->slowDuration);
            towerSound(missileSound);
            break;
        case bullet :
            launchBullet(currentTower->x+currentTower->gunX, currentTower->y+currentTower->gunY, currentTower->damage, currentTower->targetID, currentTower->towerType, currentTower->AOEpower, currentTower->AOErange, currentTower->slowPower, currentTower->slowDuration);
            towerSound(bulletSound);
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// TOWER PRESENTATION FUNCTIONS
///////////////////////////////

/**
Calls the drawTower() function (Display.c) for each tower in the game
*/
void present_tower()
{
    TowerGroup TG = getTowerGrp(NULL);
    if(TG->numOfTowers>0)
    {
        for(int towerID=1; towerID<=TG->numOfTowers; ++towerID)
        {
            Tower currentTower = getTowerID(towerID);
            drawTower(currentTower->x, currentTower->y, currentTower->width,currentTower->height, currentTower->towerType, currentTower->range,
                      8/*frames*/, 300/*anim_speed*/, 2080 /*pic_width*/, 258/*pic_height*/);
        }
    }
}

/**
Calls the drawTowerPosition() function (Display.c) for all tower positions that are currently empty
*/
void drawAllTowerPositions()	{

	TowerPos tPos = getTowerPos(NULL);
	int t;
	for(t = 1; t <= tPos->numberOfPositions;t++)	{
		if(tPos->towerPositions[t]->empty == TRUE)	{
			drawTowerPosition(tPos->towerPositions[t]->x,tPos->towerPositions[t]->y,50,50,tPos->towerPositions[t]->tIcon);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// TOWER FREEING FUNCTIONS
//////////////////////////

/**
Frees the specified tower structure
*/
void freeTower(Tower t) {
    free(t);
}

/**
Frees all tower structures and sets the number of towers in the towerGroup structure to 0
*/
void freeAllTowers()
{
	  int i = 1;
	  makeAllTowPosAvailable();
	  while(i <= getTowerGrp(NULL)->numOfTowers)	{
		  free(getTowerGrp(NULL)->listOfTowers[i]);
		  i++;
	  }
	  getTowerGrp(NULL)->numOfTowers = 0;	
}

/**
Frees the towerGroup structure
*/
void freeTowerGroupStructure()
{
    free(getTowerGrp(NULL));
}

/**
Frees all tower position nodes and then frees the towerPositions structure
*/
void freeAllTowerPositions()	{
	
	TowerPos tPos = getTowerPos(NULL);
	int positions;
	for(positions = 1; positions <= tPos->numberOfPositions; positions++){
		free(tPos->towerPositions[positions]);
	}

	free(tPos);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// TOWER TESTING FUNCTIONS
//////////////////////////

/**
Manually set the specified tower's X position (used in testing)
*/
int setTowerX(int towerID,int newX)	{
	  getTowerGrp(NULL)->listOfTowers[towerID]->x = newX;
	  return newX;
}

/**
Manually set the specified tower's Y position (used in testing)
*/
int setTowerY(int towerID, int newY)	{
	  getTowerGrp(NULL)->listOfTowers[towerID]->y = newY;
	  return newY;
}

/**
Manually set the specified tower's x range statistic (used in testing)
*/
int setTowerRange(int towerID, int newRange)	{
	  getTowerGrp(NULL)->listOfTowers[towerID]->range = newRange;
	  return newRange;
}

/**
Manually set the specified tower's damage statistic (used in testing)
*/
int setTowerDamage(int towerID, int newDamage)	{
	  getTowerGrp(NULL)->listOfTowers[towerID]->damage = newDamage;
	  return newDamage;
}

/**
Wrapper for all of the tower tests
*/
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

/**
Test creating towers
*/
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

/**
Returns the Y coordinate of the most recently created tower (used in testing)
*/
int getLastTowerPositionY()	{
	return getSpecifiedTowerPosY(getTowerPos(NULL)->numberOfPositions);
}

/**
Returns the X coordinate of the most recently created tower (used in testing)
*/
int getLastTowerPositionX()	{
	return getSpecifiedTowerPosX(getTowerPos(NULL)->numberOfPositions);
}

/**
Testing the X and Y positions of created towers
*/
void testTowerCreation()	{
	
	addTowerPosNode(100,200);
	sput_fail_unless(getLastTowerPositionX() == (int) scaleTowerPos(100,SCREEN_WIDTH_GLOBAL,MAX_TOWER_X), "Newly Added Tower position x coord is correct");
	sput_fail_unless(getLastTowerPositionY() == (int) scaleTowerPos(200,SCREEN_HEIGHT_GLOBAL,MAX_TOWER_Y), "Newly Added Tower position y coord is correct");
}

/**
Returns the X coordinate of the most specified tower position (used in testing)
*/
int getTowerPositionX(int position)	{
	return getTowerPos(NULL)->towerPositions[position]->x;
}



int getTowerPositionY(int position)	{
	return getTowerPos(NULL)->towerPositions[position]->y;
}

/**
Wrapper for all external projectiles tests
*/
void testingProjectiles()
{
    sput_start_testing();
	  sput_set_output_stream(stderr);
	  sput_enter_suite("testProjectileHandling(): Checking projectiles are created, moved and deleted from linked list");
	  sput_run_test(testProjectileHandling);
	  sput_leave_suite();
	  
  	sput_finish_testing();
}

/**
Test projectile creation, movement and interaction with enemies from outside of the projectiles.c module
*/
void testProjectileHandling()
{
    
    freeAllEnemies();
    createSpecificEnemy(intBasic, 1, 1); //create 1 enemy
    int enemyOriginalHealth = getEnemyHealth(getNumberOfEnemies());
    
    int towerID = userCreateTower(5000,5000); //create standard tower at position 5000, 5000
    Tower t = getTowerID(towerID);
    
    t->towerType = INT_TYPE;
    t->firingType = bullet;
    t->targetID = 1;
    
    for(int i = 0; i < 1000; i++) { // fire 1000 projectiles
      launchProjectile(t);
    }
    
    sput_fail_unless(test_numOfProjectiles() == 1000, "1000 bullets created, all are in projectile list");
    
    int firstMoveX, firstMoveY;
    firstMoveX = test_startingProjectileXcoord();
    firstMoveY = test_startingProjectileYcoord();
    
    updateProjectiles();
    updateExplosions();
    sput_fail_unless(test_startingProjectileXcoord() != firstMoveX, "Bullets have changed X position on move");
    sput_fail_unless(test_startingProjectileYcoord() != firstMoveY, "Bullets have changed Y position on move");
    
    for(int i = 0; i < 10000; i++) {//move projectiles 10000 times
        updateProjectiles();
        updateExplosions();
    }
    
    sput_fail_unless(test_numOfProjectiles() == 0, "After 10,000 moves all bullets have disappeared");
    sput_fail_unless(getEnemyHealth(getNumberOfEnemies()) < enemyOriginalHealth, "After 10,000 projectile moves, targeted enemy's heath has been depleted");
    sput_fail_unless(isDead(getNumberOfEnemies()) == 1, "After being hit by 1000 bullets, enemy is good and dead");
    
    freeAllEnemies();
    freeAllTowers();
}

/**
Test tower targeting and firing
*/
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
    Tower t = TG->listOfTowers[TG->numOfTowers];
    t->range = 50;
    
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
    sput_fail_unless(test_checkStartingProjectileTarget() == 1, "Valid: created projectiles are targetting correct enemy");
}

/**
* manually reset a specified tower's firing cooldown. Used in testing
*/
void resetTowerCooldown(int towerID)
{
    getTowerGrp(NULL)->listOfTowers[towerID]->firingCoolDown = 0;
}











  


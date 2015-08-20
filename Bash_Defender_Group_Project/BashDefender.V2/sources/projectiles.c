
#include "../includes/projectiles.h"


//bullet #defines
#define BULLET_TO_TARGET 20 // the number of steps for a bullet to reach its target
#define BULLET_SIZE 10 // the height and width of the bullet image

//missile #defines
#define BUILDUP_DISTANCE 100 // the distance the missiles travel during their buildup stage
#define BUILDUP_STEPS 50 // the number of steps it takes for a missile to reach the end of its buildup stage
#define MISSILE_BUILDUP_DIVISION 20 // the fraction (e.g 1/20th, 1/30th) that the missile moves towards its buildup position every step
#define MISSILE_TO_TARGET 7 // the number of steps it takes for a missile to reach its target after buildup
#define MISSILE_STARTING_SIZE 3 // the starting height and width of missile projectiles
#define MISSILE_ENDING_SIZE 15 // the size of missiles once they have finished their buildup stage

//laser #defines
#define LASER_DRAW_COUNT 20 // the number of steps that laser beams stay on the screen

//aoe #defines
#define AOE_DISPLAY_COUNT 10 // the number of steps that AOE explosions stay on the screen
void drawExplosions();

struct projectileNode {
    int x, y;
    int h, w;
    int originX, originY;
    int buildUpX, buildUpY;
    
    int damageType;
    FiringMethod whatProjectile;
    
    int targetCoords[2];
    int targetID;
    
    int damage;
    int aoeDamage;
    int aoeRange;
    
    int slowPower;
    int slowDuration;
    
    int movesMade;
    int movesToTarget;
    int movesForBuildUp;
    int buildUpH, buildUpW;
    
    int drawLaserCount;
    int drawLaserMaxCount;
    
    ProjectileNode next;
};

struct projectileList {
    ProjectileNode start, current, last;
} ;

struct explosionNode {
    int dmgType;
  
    int x, y;
    int range;
    
    int totalDisplayCount;
    int currentDisplayCount;
    
    ExplosionNode next;
} ;

struct explosionList {
    ExplosionNode start, current, last;
} ;


//////////////////////////////////////////////////////////////////////////////////////////////////////
/* EXPLOSION LINKED LIST FUNCTIONS */

/**
creates the AOE explosion list structure
*/
void createExplosionList()
{
    ExplosionList newExplosionList = (ExplosionList)malloc(sizeof(struct explosionList) );
    if(newExplosionList == NULL) {
        fprintf(stderr,"****ERROR unable to malloc space for AOE explosion list ****\n");
        exit(1);
    }
    
    newExplosionList->start = NULL;
    newExplosionList->current = NULL;
    newExplosionList->last = NULL;
    
    getExplosionList(newExplosionList);
}

/**
if passed NULL, returns static pointer to AOE explosion list structure. Else, assigns the passed pointer as the static pointer
*/
ExplosionList getExplosionList(ExplosionList eL)
{
	  static ExplosionList newExplosionList;

	  if(eL != NULL)	{
		    newExplosionList = eL;
	  }

	  return newExplosionList;
}


/**
creates a new, blank Explosion node
*/
ExplosionNode newExplosionNode()
{
    ExplosionNode newNode = (ExplosionNode)malloc(sizeof(struct explosionNode) );
    if(newNode == NULL) {
        fprintf(stderr,"****ERROR unable to malloc space for AOE explosion node ****\n");
        exit(1);
    }
    
    newNode->next = NULL;
    
    return newNode;
}

/**
adds the passed AOE explosion node to the end of the explosion linked list
*/
void addToExplosionList(ExplosionNode newNode)
{
    ExplosionList eL = getExplosionList(NULL);
    
    if(eL->start == NULL) {
        eL->start = newNode;
        eL->current = eL->start;
        eL->last = eL->start;
    } else {
        eL->last->next = newNode;
        eL->last = newNode;
    }
}

/**
advances, draws and removes explosions each frame
*/
void updateExplosions()
{
    advanceExplosions();
    drawExplosions();
    removeExplosions();
}

/**
removes any AOE explosions that have reached the end of their draw count
*/
void removeExplosions()
{
    ExplosionList eL = getExplosionList(NULL);
    if(eL->start!=NULL) {
        eL->current = eL->start;
        ExplosionNode prv = eL->start;
        ExplosionNode tmp = eL->start;
        while(eL->current!=NULL) {
            if(explosionFinished(eL->current) ) {
                if(eL->current == eL->start) {
                    eL->start = eL->current->next;
                }
                if(eL->current == eL->last) {
                    eL->last = prv;
                }
                eL->current = eL->current->next;
                prv->next = eL->current;
                free(tmp);
            } else {
                prv = eL->current;
                eL->current = eL->current->next;
            }
            tmp = eL->current;
        }
    }
}

/**
returns 1 if an AOE explosion is at the end of its existence, else returns 0
*/  
int explosionFinished(ExplosionNode eNode)
{
    if(eNode->currentDisplayCount == eNode->totalDisplayCount) {
        return 1;
    } else {
        return 0;
    }
}

/**
makes a new AOE explosion and adds it to the explosion linked list
*/
void makeExplosion(int dmgType, int targetX, int targetY, int range)
{
    ExplosionNode newExplosion = newExplosionNode();
    
    newExplosion->dmgType = dmgType;
    
    newExplosion->x = targetX;
    newExplosion->y = targetY;
    newExplosion->range = range;
    
    newExplosion->totalDisplayCount = AOE_DISPLAY_COUNT;
    newExplosion->currentDisplayCount = 0;
    
    addToExplosionList(newExplosion);
}

/**
updates all explosions by increasing their current Display Count
*/
void advanceExplosions()
{
    ExplosionList eL = getExplosionList(NULL);
    eL->current = eL->start;
    while(eL->current != NULL) {
        eL->current->currentDisplayCount++;
        eL->current = eL->current->next;
    }
}

/**
draw the range of all AOE explosions
*/
void drawExplosions()
{
    ExplosionList eL = getExplosionList(NULL);
    eL->current = eL->start;
    while(eL->current != NULL) {
        drawAOE(eL->current->dmgType, eL->current->x, eL->current->y, eL->current->range, eL->current->currentDisplayCount, eL->current->totalDisplayCount);
        eL->current = eL->current->next;
    }
}

/**
damages enemies within AOE explosion range
*/
void doAOEDamage(int damageType, int damage, int targetID, int range, int x, int y) {
    for(int i = 1; i <= getNumberOfEnemies(); i++) {
        if(i != targetID && !isDead(i)) {
            if(inRange(x, y, range, i) ) {
                damageEnemy(damage, i, damageType);
            }
        }
    }
}

void freeAllExplosions()
{
    ExplosionList eL = getExplosionList(NULL);
    if(eL->start != NULL) { //if start is null, none to be freed
        int finished = 0;
        eL->current = eL->start;
        ExplosionNode tmp;
        while(!finished) {
            if(eL->current->next != NULL) {
                tmp = eL->current;
                eL->current = eL->current->next;
                free(tmp);
            } else {
                free(eL->current);
                finished = 1;
            }
        }
    }
    
    // once all freed, reset explosion list
    eL->start = NULL;
    eL->current = NULL;
    eL->last = NULL;
}
              
/**
frees all projectile nodes, then frees the projectile list structure itself
*/
void freeExplosionList()
{
    freeAllExplosions();
    ExplosionList eL = getExplosionList(NULL);
    free(eL);
}  



//////////////////////////////////////////////////////////////////////////////////////////////////////
/* PROJECTILE LINKED LIST FUNCTIONS */

/**
creates the projectile list structure
*/
void createProjectileList()
{
    ProjectileList newProjList = (ProjectileList)malloc(sizeof(struct projectileList) );
    if(newProjList == NULL) {
        fprintf(stderr,"****ERROR unable to malloc space for projectile list ****\n");
        exit(1);
    }
    
    newProjList->start = NULL;
    newProjList->current = NULL;
    newProjList->last = NULL;
    
    getProjectileList(newProjList);
}

/**
if passed NULL, returns static pointer to projectile list structure. Else, assigns the passed pointer as the static pointer
*/
ProjectileList getProjectileList(ProjectileList pL)
{
	  static ProjectileList newProjList;

	  if(pL != NULL)	{
		    newProjList = pL;
	  }

	  return newProjList;
}

/**
creates a new, blank projectile node
*/
ProjectileNode newProjectileNode()
{
    ProjectileNode newNode = (ProjectileNode)malloc(sizeof(struct projectileNode) );
    if(newNode == NULL) {
        fprintf(stderr,"****ERROR unable to malloc space for projectile node ****\n");
        exit(1);
    }
    
    newNode->next = NULL;
    
    return newNode;
}

/**
sets all the base stats for a newly created projectile
*/
void initialiseProjectile(ProjectileNode newNode, int gunX, int gunY, int damage, int targetID, int firingType, int aoeDamage, int aoeRange, int slowPower, int slowDuration)
{
    newNode->damage = damage;
    newNode->damageType = firingType;
    
    newNode->originX = gunX;
    newNode->originY = gunY;
    newNode->targetID = targetID;
    
    newNode->aoeDamage = aoeDamage;
    newNode->aoeRange = aoeRange;
    
    newNode->slowPower = slowPower;
    newNode->slowDuration = slowDuration;
}

/**
adds the passed projectile node to the end of the projectile linked list
*/
void addToProjectileList(ProjectileNode newNode)
{
    ProjectileList pL = getProjectileList(NULL);
    
    if(pL->start == NULL) {
        pL->start = newNode;
        pL->current = pL->start;
        pL->last = pL->start;
    } else {
        pL->last->next = newNode;
        pL->last = newNode;
    }
}    

/**
moves, draws and removes projectiles each frame
*/
void updateProjectiles()
{
    moveProjectiles();
    drawProjectiles();
    removeProjectiles();
}

/**
moves each projectile in the linked list depending on its type and attributes
*/
void moveProjectiles()
{
    int finished = 0;
    ProjectileList pL = getProjectileList(NULL);
    pL->current = pL->start;
    if(pL->current != NULL) {
        while(!finished) {
            switch(pL->current->whatProjectile) {
                case missile :
                  moveMissile(pL->current); 
                  break;
                case bullet :
                  moveBullet(pL->current);
                  break;
                case laser :
                  updateLaser(pL->current);
                  break;
            }
            if(pL->current->next == NULL) {
                finished = 1;
            } else {
                pL->current = pL->current->next;
            }
        }
    }
}

/**
draws each projectile in the linked list
*/
void drawProjectiles()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start!=NULL) {
        pL->current = pL->start;
        int finished = 0;
        Display d = getDisplayPointer(NULL);
        while(!finished) {
            ProjectileNode p = pL->current;
            switch(p->whatProjectile) {
                case missile :
                    drawBullet(p->x - (p->w/2), p->y - (p->h/2), p->w, p->h, p->damageType);
                    break;
                case bullet :
                    drawBullet(p->x - (p->w/2), p->y - (p->h/2), p->w, p->h, p->damageType);
                    break;
                case laser :
                    drawLaser(d, p->originX, p->originY, p->targetCoords[0], p->targetCoords[1], p->damageType, p->drawLaserCount, p->drawLaserMaxCount);
            }
            if(p->next == NULL) {
                finished = 1;
            } else {
                pL->current = p->next;
            }
        }
    }
}

/**
removes any projectiles that have reached the end of their moves
*/
void removeProjectiles()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start!=NULL) {
        pL->current = pL->start;
        ProjectileNode prv = pL->start;
        ProjectileNode tmp = pL->start;
        while(pL->current!=NULL) {
            if(projectileFinished(pL->current) ) {
                if(pL->current == pL->start) {
                    pL->start = pL->current->next;
                }
                if(pL->current == pL->last) {
                    pL->last = prv;
                }
                pL->current = pL->current->next;
                prv->next = pL->current;
                free(tmp);
            } else {
                prv = pL->current;
                pL->current = pL->current->next;
            }
            tmp = pL->current;
        }
    }
}

/**
returns 1 if a projectile is at the end of its existence, else returns 0
*/  
int projectileFinished(ProjectileNode pNode)
{
    switch(pNode->whatProjectile) {
        case missile :
            if(pNode->movesMade == pNode->movesToTarget+pNode->movesForBuildUp) {
                return 1;
            }
            break;
        case bullet :
            if(pNode->movesMade == pNode->movesToTarget) {
                return 1;
            }
            break;
        case laser :
            if(pNode->drawLaserCount == pNode->drawLaserMaxCount) {
                return 1;
            }
            break;
    }
    return 0;
}

/**
frees all projectiles in the linked list (if there are any) and resets internal list structure pointers to NULL
*/
void freeAllProjectiles()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start != NULL) { //if start is null, none to be freed
        int finished = 0;
        pL->current = pL->start;
        ProjectileNode tmp;
        while(!finished) {
            if(pL->current->next != NULL) {
                tmp = pL->current;
                pL->current = pL->current->next;
                free(tmp);
            } else {
                free(pL->current);
                finished = 1;
            }
        }
    }
    
    // once all freed, reset projectile list
    pL->start = NULL;
    pL->current = NULL;
    pL->last = NULL;
}
              
/**
frees all projectile nodes, then frees the projectile list structure itself
*/
void freeProjectileList()
{
    freeAllProjectiles();
    ProjectileList pL = getProjectileList(NULL);
    free(pL);
}  

//////////////////////////////////////////////////////////////////////////////////////////////////////
/* BULLET FUNCTIONS */

/**
creates a new bullet projectile and launches it at where the target will be
*/
void launchBullet(int firedX, int firedY, int damage, int targetID, int firingType, int aoeDamage, int aoeRange, int slowPower, int slowDuration)
{
    // make the bullet
    ProjectileNode newNode = newProjectileNode();
    newNode->whatProjectile = bullet;
    
    initialiseProjectile(newNode, firedX, firedY, damage, targetID, firingType, aoeDamage, aoeRange, slowPower, slowDuration);
    
    newNode->movesMade = 0;
    newNode->movesToTarget = BULLET_TO_TARGET;
    
    newNode->h = BULLET_SIZE;
    newNode->w = BULLET_SIZE;
    
    newNode->x = firedX;
    newNode->y = firedY;
    
    getProjectileTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget);
    
      // add it to the list
    addToProjectileList(newNode);
}

/**
moves the bullet in a straight line towards its target coords. If at end of moves, damages enemy
*/
void moveBullet(ProjectileNode bullet) {

    bullet->movesMade++;
    
    //recalculate target position (in case of slowed enemy)
    getProjectileTargetPos(bullet->targetID, bullet->targetCoords, bullet->movesToTarget - bullet->movesMade);
  
    if(bullet->movesMade == bullet->movesToTarget) {
        damageEnemy(bullet->damage, bullet->targetID, bullet->damageType);
        slowEnemy(bullet->targetID, bullet->slowPower, bullet->slowDuration);
        doAOEDamage(bullet->damageType, bullet->aoeDamage, bullet->targetID, bullet->aoeRange, bullet->targetCoords[0], bullet->targetCoords[1]);
        makeExplosion(bullet->damageType, bullet->targetCoords[0], bullet->targetCoords[1], bullet->aoeRange);
        
        bullet->x = bullet->targetCoords[0];
        bullet->y = bullet->targetCoords[1];
    } else {
      
        bullet->x = bullet->originX + (int) ( ((double)(bullet->targetCoords[0]-bullet->originX)/(double) bullet->movesToTarget) * bullet->movesMade);
        bullet->y = bullet->originY + (int) ( ((double)(bullet->targetCoords[1]-bullet->originY)/(double) bullet->movesToTarget) * bullet->movesMade);
        
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/* MISSILE FUNCTIONS */

/**
creates a new missile projectile and sets its target/target coords
*/
void launchMissile(int firedX, int firedY, int damage, int targetID, int firingType, int aoeDamage, int aoeRange, int slowPower, int slowDuration)
{
    // make the missile
    ProjectileNode newNode = newProjectileNode();
    newNode->whatProjectile = missile;
    
    initialiseProjectile(newNode, firedX, firedY, damage, targetID, firingType, aoeDamage, aoeRange, slowPower, slowDuration);
    
    newNode->movesMade = 0;
    newNode->movesForBuildUp = BUILDUP_STEPS;
    newNode->movesToTarget = MISSILE_TO_TARGET;
    
    newNode->h = MISSILE_STARTING_SIZE;
    newNode->w = MISSILE_STARTING_SIZE;
    
    newNode->x = firedX;
    newNode->y = firedY;
    
    getProjectileTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget+newNode->movesForBuildUp);
    getBuildUpCoords(newNode->originX, newNode->originY, &newNode->buildUpX, &newNode->buildUpY);
    
      // add it to the list
    addToProjectileList(newNode);
}

/**
sets the destination of the missile's build up phase (moving in a random direction of length BUILDUP_DISTANCE)
*/
void getBuildUpCoords(int firedX, int firedY, int *buildUpX, int *buildUpY)
{
  
    int randChecker = 0; //counter to check for loop in while statement below
    int x, y, xAdjust, yAdjust;
    
      // set x and y to be random value between -10 & 10
    while( (x = (rand()%21) - 10) == 0 || (y = (rand()%21) - 10) == 0) {
        randChecker++;
        if(randChecker > 1000) {
            fprintf(stderr,"****ERROR missile has looped more than 1000 times trying to find a build up coordinate (tower.c) ****\n");
            exit(1);
        }
    }
    
    double calcX, calcY;
    if(x < 0) {
        calcX = (double)(-x);
    } else {
        calcX = (double) x;
    }
    
    if(y < 0) {
        calcY = (double)(-y);
    } else {
        calcY = (double) y;
    }
    
      // get a build up target that is a set number of pixels away
    double angle = atan(calcY/calcX);
    
    calcY = BUILDUP_DISTANCE * sin(angle);
    calcX = BUILDUP_DISTANCE * cos(angle);
    
    // set x & y to adjusted values
    
    if(x < 0) {
        xAdjust = (int)(-calcX);
    } else {
        xAdjust = (int)calcX;
    }
    
    if(y < 0) {
        yAdjust = (int)(-calcY);
    } else {
        yAdjust = (int)calcY;
    }
    
    // set the build up coordinates based on the calculated values and starting coordinates
    *buildUpX = firedX+xAdjust;
    *buildUpY = firedY+yAdjust;
}

/**
moves the missile depending on its phase of movement. If at end of moves, damages enemy
*/
void moveMissile(ProjectileNode missile) {

    missile->movesMade++;
    
    //recalculate target position (in case of slowed enemy)
    getProjectileTargetPos(missile->targetID, missile->targetCoords, (missile->movesToTarget+missile->movesForBuildUp) - missile->movesMade);
    
    
    if(missile->movesMade == missile->movesToTarget+missile->movesForBuildUp) {
        damageEnemy(missile->damage, missile->targetID, missile->damageType);
        slowEnemy(missile->targetID, missile->slowPower, missile->slowDuration);
        doAOEDamage(missile->damageType, missile->aoeDamage, missile->targetID, missile->aoeRange, missile->targetCoords[0], missile->targetCoords[1]);
        makeExplosion(missile->damageType, missile->targetCoords[0], missile->targetCoords[1], missile->aoeRange);
        
        missile->x = missile->targetCoords[0];
        missile->y = missile->targetCoords[1];
    } else {
        if(missile->movesMade <= missile->movesForBuildUp) {
            missile->x = missile->originX + (int) ((double)(missile->buildUpX-missile->originX)/(double)MISSILE_BUILDUP_DIVISION);
            missile->y = missile->originY + (int) ((double)(missile->buildUpY-missile->originY)/(double)MISSILE_BUILDUP_DIVISION);
            missile->originX = missile->x;
            missile->originY = missile->y;
            
            missile->h = MISSILE_STARTING_SIZE + (int)( (double)(MISSILE_ENDING_SIZE-MISSILE_STARTING_SIZE) * ( (double)missile->movesMade/(double)missile->movesForBuildUp) );
            missile->w = MISSILE_STARTING_SIZE + (int)( (double)(MISSILE_ENDING_SIZE-MISSILE_STARTING_SIZE) * ( (double)missile->movesMade/(double)missile->movesForBuildUp) );
            
        } else {
            missile->x = missile->originX + (int) ( ((double)(missile->targetCoords[0]-missile->originX)/(double) (missile->movesToTarget)) * (missile->movesMade-missile->movesForBuildUp));
            missile->y = missile->originY + (int) ( ((double)(missile->targetCoords[1]-missile->originY)/(double) (missile->movesToTarget)) * (missile->movesMade-missile->movesForBuildUp));
        }

    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/* LASER FUNCTIONS */
    

/**
creates a laser projectile and sets its properties
*/
void fireLaser(int gunX, int gunY, int damage, int targetID, int firingType, int aoeDamage, int aoeRange, int slowPower, int slowDuration)
{
    ProjectileNode newNode = newProjectileNode();
    newNode->whatProjectile = laser;
    
    initialiseProjectile(newNode, gunX, gunY, damage, targetID, firingType, aoeDamage, aoeRange, slowPower, slowDuration);
    
    newNode->movesToTarget = 0;
    newNode->drawLaserCount = 0;
    newNode->drawLaserMaxCount = LASER_DRAW_COUNT;
    
    getProjectileTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget);
    
    
      // add it to the list
    addToProjectileList(newNode);
}
  
/**
updates the draw count for a laser
*/
void updateLaser(ProjectileNode laser)
{
    if(laser->drawLaserCount == 0) {
        damageEnemy(laser->damage, laser->targetID, laser->damageType);
        slowEnemy(laser->targetID, laser->slowPower, laser->slowDuration);
        doAOEDamage(laser->damageType, laser->aoeDamage, laser->targetID, laser->aoeRange, laser->targetCoords[0], laser->targetCoords[1]);
        makeExplosion(laser->damageType, laser->targetCoords[0], laser->targetCoords[1], laser->aoeRange);
    }
    
      // reset target position so that laser follows enemy
    getProjectileTargetPos(laser->targetID, laser->targetCoords, laser->movesToTarget);
    laser->drawLaserCount++;
}

///////////////////////////////////////////////////////////////////////////////////////
// TESTING FUNCTIONS

/**
returns the number of projectiles that currently exist. Used in testing
*/
int test_numOfProjectiles()
{
    
    ProjectileList pL = getProjectileList(NULL);
    
    if(pL->start == NULL) {
        return 0;
    } else {
        pL->current = pL->start;
        int projectiles = 1;
        while(pL->current->next != NULL) {
            projectiles++;
            pL->current = pL->current->next;
        }
        return projectiles;
    }
}

/**
returns the x coordinate of the first projectile in the projectile list
*/
int test_startingProjectileXcoord()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start == NULL) {
        fprintf(stderr,"tried to get test projectile X-coord from empty projectile list");
        exit(1);
    }
    return pL->start->x;
}

/**
returns the y coordinate of the first projectile in the projectile list
*/
int test_startingProjectileYcoord()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start == NULL) {
        fprintf(stderr,"tried to get test projectile Y-coord from empty projectile list");
        exit(1);
    }
    return pL->start->y;
}

/**
returns the target ID of the first projectile in the projectile list
*/
int test_checkStartingProjectileTarget()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start == NULL) {
        fprintf(stderr,"tried to get test projectile target from empty projectile list");
        exit(1);
    }
    return pL->start->targetID;
}

    
      
  


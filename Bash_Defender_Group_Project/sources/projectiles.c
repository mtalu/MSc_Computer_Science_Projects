
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


struct projectileNode {
    int x, y;
    int h, w;
    int originX, originY;
    int buildUpX, buildUpY;
    int centreX, centreY;
    
    int damageType;
    FiringMethod whatProjectile;
    
    int targetCoords[2];
    int targetID;
    
    int damage;
    int aoeDamage;
    int aoeRange;
    
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


//////////////////////////////////////////////////////////////////////////////////////////////////////
/* PROJECTILE LINKED LIST FUNCTIONS */

/*
* creates the projectile list structure
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

/*
* if passed NULL, returns static pointer to projectile list structure. Else, assigns the passed pointer as the static pointer
*/
ProjectileList getProjectileList(ProjectileList pL)
{
	  static ProjectileList newProjList;

	  if(pL != NULL)	{
		    newProjList = pL;
	  }

	  return newProjList;
}

/*
* creates a new, blank projectile node
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

/*
* adds the passed projectile node to the end of the projectile linked list
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

/*
* removes the passed projectile node from the projectile linked list
*/
void removeProjectileNode(ProjectileNode projNode)
{
    ProjectileList pL = getProjectileList(NULL);
    ProjectileNode prev;
    
    if(projNode == pL->start) {
        pL->start = projNode->next;
        free(projNode);
    } else {
        pL->current = pL->start->next;
        prev = pL->start;
        
        while(pL->current != projNode) {
            prev = pL->current;
            pL->current = pL->current->next;
        }
        
        if(pL->current == pL->last) {
            pL->last = prev;
            free(pL->current);
            pL->last->next = NULL;
        } else {
            prev->next = pL->current->next;
            free(pL->current);
        }
    }
}      

/*
* moves each projectile in the linked list depending on its type and attributes
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

/*
* draws each projectile in the linked list
*/
void drawProjectiles()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start!=NULL) {
        pL->current = pL->start;
        int finished = 0;
        Display d = getDisplayPointer(NULL);
        while(!finished) {
            switch(pL->current->whatProjectile) {
                case missile :
                    drawBullet(pL->current->x, pL->current->y, pL->current->w, pL->current->h, pL->current->damageType);
                    break;
                case bullet :
                    drawBullet(pL->current->x, pL->current->y, pL->current->w, pL->current->h, pL->current->damageType);
                    break;
                case laser :
                    drawLine(d, pL->current->originX, pL->current->originY, pL->current->targetCoords[0], pL->current->targetCoords[1], pL->current->damageType);
            }
            if(pL->current->next == NULL) {
                finished = 1;
            } else {
                pL->current = pL->current->next;
            }
        }
    }
}

/*
* frees all projectiles in the linked list (if there are any) and resets internal list structure pointers to NULL
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
              
/*
* frees all projectile nodes, then frees the projectile list structure itself
*/
void freeProjectileList()
{
    freeAllProjectiles();
    ProjectileList pL = getProjectileList(NULL);
    free(pL);
}  

//////////////////////////////////////////////////////////////////////////////////////////////////////
/* BULLET FUNCTIONS */

/*
* creates a new bullet projectile and launches it at where the target will be
*/
void launchBullet(int firedX, int firedY, int damage, int targetID, int firingType)
{
    // make the bullet
    ProjectileNode newNode = newProjectileNode();
    
    newNode->movesMade = 0;
    newNode->movesToTarget = BULLET_TO_TARGET;
    
    newNode->whatProjectile = bullet;
    newNode->damageType = firingType;
    newNode->damage = damage;
    newNode->h = BULLET_SIZE;
    newNode->w = BULLET_SIZE;
    
    newNode->x = firedX-(newNode->w/2);
    newNode->y = firedY-(newNode->h/2);
    newNode->originX = newNode->x;
    newNode->originY = newNode->y;
    
    newNode->aoeDamage = 0;
    newNode->aoeRange = 0;
    
    newNode->targetID = targetID;
    getBulletTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget);
    newNode->targetCoords[0] = newNode->targetCoords[0] - (newNode->w/2);
    newNode->targetCoords[1] = newNode->targetCoords[1] - (newNode->h/2);
    
      // add it to the list
    addToProjectileList(newNode);
}

/*
* moves the bullet in a straight line towards its target coords. If at end of moves, damages enemy
*/
void moveBullet(ProjectileNode bullet) {

    bullet->movesMade++;
  
    if(bullet->movesMade == bullet->movesToTarget) {
        damageEnemy(bullet->damage, bullet->targetID, bullet->damageType);
      
        removeProjectileNode(bullet);
    } else {
      
        bullet->x = bullet->originX + (int) ( ((double)(bullet->targetCoords[0]-bullet->originX)/(double) bullet->movesToTarget) * bullet->movesMade);
        bullet->y = bullet->originY + (int) ( ((double)(bullet->targetCoords[1]-bullet->originY)/(double) bullet->movesToTarget) * bullet->movesMade);
        
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/* MISSILE FUNCTIONS */

/*
* creates a new missile projectile and sets its target/target coords
*/
void launchMissile(int firedX, int firedY, int damage, int targetID, int firingType)
{
    // make the missile
    ProjectileNode newNode = newProjectileNode();
    newNode->whatProjectile = missile;
    newNode->damageType = firingType;
    
    newNode->movesMade = 0;
    newNode->movesForBuildUp = BUILDUP_STEPS;
    newNode->movesToTarget = MISSILE_TO_TARGET;
    
    newNode->h = MISSILE_STARTING_SIZE;
    newNode->w = MISSILE_STARTING_SIZE;
    
    newNode->centreX = firedX;
    newNode->centreY = firedY;
    newNode->x = firedX-(newNode->w/2);
    newNode->y = firedY-(newNode->h/2);
    newNode->originX = firedX;
    newNode->originY = firedY;
    
    newNode->damage = damage;
    newNode->aoeDamage = 0;
    newNode->aoeRange = 0;
    
    newNode->targetID = targetID;
    getBulletTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget+newNode->movesForBuildUp);
    
    
    getBuildUpCoords(newNode->originX, newNode->originY, &newNode->buildUpX, &newNode->buildUpY);
    
      // add it to the list
    addToProjectileList(newNode);
}

/*
*  sets the destination of the missile's build up phase (moving in a random direction of length BUILDUP_DISTANCE)
*/
void getBuildUpCoords(int firedX, int firedY, int *buildUpX, int *buildUpY)
{
  
    int randChecker = 0; //counter to check for loop in while statement below
    int x, y, xAdjust, yAdjust;
    
      // set x and y to be random value between -10 & 10
    while( (x = (rand()%21) - 10) == 0 || (y = (rand()%21) - 10) == 0) {
        randChecker++;
        if(randChecker > 1000) {
            fprintf(stderr,"****ERROR missile has looped more than 1000 times trying to find a missile coordinate (tower.c) ****\n");
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

/*
/ moves the missile depending on its phase of movement. If at end of moves, damages enemy
*/
void moveMissile(ProjectileNode missile) {

    missile->movesMade++;
    
    if(missile->movesMade == missile->movesToTarget+missile->movesForBuildUp) {
        damageEnemy(missile->damage, missile->targetID, missile->damageType);
        removeProjectileNode(missile);
    } else {
        if(missile->movesMade <= missile->movesForBuildUp) {
            missile->centreX = missile->originX + (int) ((double)(missile->buildUpX-missile->originX)/(double)MISSILE_BUILDUP_DIVISION);
            missile->centreY = missile->originY + (int) ((double)(missile->buildUpY-missile->originY)/(double)MISSILE_BUILDUP_DIVISION);
            missile->originX = missile->centreX;
            missile->originY = missile->centreY;
            
            missile->h = MISSILE_STARTING_SIZE + (int)( (double)(MISSILE_ENDING_SIZE-MISSILE_STARTING_SIZE) * ( (double)missile->movesMade/(double)missile->movesForBuildUp) );
            missile->w = MISSILE_STARTING_SIZE + (int)( (double)(MISSILE_ENDING_SIZE-MISSILE_STARTING_SIZE) * ( (double)missile->movesMade/(double)missile->movesForBuildUp) );
            
            missile->x = missile->centreX-(missile->w/2);
            missile->y = missile->centreY-(missile->h/2);
        } else {
            missile->x = missile->originX + (int) ( ((double)(missile->targetCoords[0]-missile->originX)/(double) (missile->movesToTarget)) * (missile->movesMade-missile->movesForBuildUp));
            missile->y = missile->originY + (int) ( ((double)(missile->targetCoords[1]-missile->originY)/(double) (missile->movesToTarget)) * (missile->movesMade-missile->movesForBuildUp));
        }

    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/* LASER FUNCTIONS */

void fireLaser(int gunX, int gunY, int damage, int targetID, int firingType)
{
    ProjectileNode newNode = newProjectileNode();
    newNode->whatProjectile = laser;
    
    newNode->damage = damage;
    newNode->damageType = firingType;
    
    newNode->movesToTarget = 5; // added some moves to target to draw line slightly in front of enemy. Visual effect, unused elsewhere
    newNode->drawLaserCount = 0;
    newNode->drawLaserMaxCount = LASER_DRAW_COUNT;
    
    newNode->originX = gunX;
    newNode->originY = gunY;
    
    newNode->targetID = targetID;
    getBulletTargetPos(targetID, newNode->targetCoords, newNode->movesToTarget);
    
    newNode->aoeDamage = 0;
    newNode->aoeRange = 0;
    
      // add it to the list
    addToProjectileList(newNode);
}
  

void updateLaser(ProjectileNode laser)
{
    if(laser->drawLaserCount == 0) {
        damageEnemy(laser->damage, laser->targetID, laser->damageType);
    }
    
    laser->drawLaserCount++;
    
    if (laser->drawLaserCount == laser->drawLaserMaxCount) {
        removeProjectileNode(laser);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
// TESTING FUNCTIONS

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

int test_startingProjectileXcoord()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start == NULL) {
        fprintf(stderr,"tried to get test projectile X-coord from empty projectile list");
        exit(1);
    }
    return pL->start->x;
}

int test_startingProjectileYcoord()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start == NULL) {
        fprintf(stderr,"tried to get test projectile Y-coord from empty projectile list");
        exit(1);
    }
    return pL->start->y;
}

int test_checkStartingProjectileTarget()
{
    ProjectileList pL = getProjectileList(NULL);
    if(pL->start == NULL) {
        fprintf(stderr,"tried to get test projectile target from empty projectile list");
        exit(1);
    }
    return pL->start->targetID;
}

    
      
  


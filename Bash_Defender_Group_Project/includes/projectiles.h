#ifndef TEST_Projectiles_h
#define TEST_Projectiles_h

#include "../includes/Display.h"
#include "../includes/enemy.h"

enum firingMethod	{

	laser = 1,
	bullet = 2,
	missile = 3
	
} ;

typedef enum firingMethod FiringMethod;

typedef struct projectileNode *ProjectileNode;
typedef struct projectileList *ProjectileList;


// functions for handling projectiles and the projectile linked list
void createProjectileList();
ProjectileList getProjectileList(ProjectileList pL);
ProjectileNode newProjectileNode();
void addToProjectileList(ProjectileNode newNode);
void removeProjectileNode(ProjectileNode projNode);
void moveProjectiles();
void drawProjectiles();
void freeAllProjectiles();
void freeProjectileList();

// bullet projectile functions
void launchBullet(int firedX, int firedY, int damage, int targetID, int firingType);
void moveBullet(ProjectileNode bullet);

// missile projectile functions
void launchMissile(int firedX, int firedY, int damage, int targetID, int firingType);
void getBuildUpCoords(int originX, int originY, int *buildUpX, int *buildUpY);
void moveMissile(ProjectileNode missile);

// laser projrctile functions
void fireLaser(int gunX, int gunY, int damage, int targetID, int firingType);
void updateLaser(ProjectileNode laser);

// testing functions
int test_numOfProjectiles();
int test_startingProjectileXcoord();
int test_startingProjectileYcoord();
int test_checkStartingProjectileTarget();

#endif

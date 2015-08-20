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

typedef struct explosionList *ExplosionList;
typedef struct explosionNode *ExplosionNode;


// functions for handling projectiles and the projectile linked list
void createProjectileList();
ProjectileList getProjectileList(ProjectileList pL);
ProjectileNode newProjectileNode();
void initialiseProjectile(ProjectileNode newNode, int gunX, int gunY, int damage, int targetID, int firingType, int aoeDamage, int aoeRange, int slowPower, int slowDuration);
void addToProjectileList(ProjectileNode newNode);
void updateProjectiles();
void moveProjectiles();
void drawProjectiles();
void removeProjectiles();
int projectileFinished(ProjectileNode pNode);
void freeAllProjectiles();
void freeProjectileList();

// bullet projectile functions
void launchBullet(int firedX, int firedY, int damage, int targetID, int firingType, int aoeDamage, int aoeRange, int slowPower, int slowDuration);
void moveBullet(ProjectileNode bullet);

// missile projectile functions
void launchMissile(int firedX, int firedY, int damage, int targetID, int firingType, int aoeDamage, int aoeRange, int slowPower, int slowDuration);
void getBuildUpCoords(int originX, int originY, int *buildUpX, int *buildUpY);
void moveMissile(ProjectileNode missile);

// laser projectile functions
void fireLaser(int gunX, int gunY, int damage, int targetID, int firingType, int aoeDamage, int aoeRange, int slowPower, int slowDuration);
void updateLaser(ProjectileNode laser);

// functions for handling AOE explosions and the explosion linked list
void createExplosionList();
ExplosionList getExplosionList(ExplosionList eL);
ExplosionNode newExplosionNode();
void addToExplosionList(ExplosionNode newNode);
void updateExplosions();
void removeExplosions();
int explosionFinished(ExplosionNode eNode);
void makeExplosion(int dmgType, int targetX, int targetY, int range);
void advanceExplosions();
void drawExplosions();
void doAOEDamage(int damageType, int damage, int targetID, int range, int x, int y);
void freeAllExplosions();
void freeExplosionList();

// testing functions
int test_numOfProjectiles();
int test_startingProjectileXcoord();
int test_startingProjectileYcoord();
int test_checkStartingProjectileTarget();

#endif

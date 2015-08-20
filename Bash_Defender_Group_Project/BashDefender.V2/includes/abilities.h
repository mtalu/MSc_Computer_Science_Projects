#ifndef _abilities_h
#define _abilities_h

#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"
#include "../includes/sput.h"

#define PSX_COST 0
#define KILL_COST 100
#define KILL_ALL_COST 300
#define KILL_UNLOCK_COST 500

typedef enum AbilityID 
{
	PSX,
	KILL
}AbilityID;

int kill_ability(int enemyID);

int is_ability_unlocked(AbilityID id);

int is_valid_unlock(AbilityID id);

int kill_all_ability();

void psx_ability();

int is_available_ability(AbilityID id);

int unlock_ability(AbilityID id);

void init_abilities();

int compare_health(const void*a, const void*b);

char *test_psx_string(char *psxlist);

void testAbilities();

void testpsx();

void testkillall();

int apt_get_query();

void testunlocks();

#endif

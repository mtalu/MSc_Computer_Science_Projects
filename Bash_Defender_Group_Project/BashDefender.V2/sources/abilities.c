#include "../includes/abilities.h"
#include "../includes/gameProperties.h"

typedef struct Enemyhealth
{
	int id;
	int health;
}Enemyhealth;


typedef struct Ability
{
	int unlocked;
	int cost;
	clockType cType;	
}Ability;

typedef struct Abilities
{
	Ability psx;
	Ability kill;
}Abilities;

Abilities* get_abilities()
{
	static Abilities a;
	return &a;
}

void init_abilities()
{	
	Abilities * abl = get_abilities();
	abl->psx.unlocked = 1;
	abl->psx.cost = PSX_COST;
	abl->kill.unlocked = 0;
	abl->kill.cost = KILL_COST;
}
	
int unlock_ability(AbilityID id)
{
	int success = 0;
	char unlockstring[17] = "Ability unlocked";
	Abilities *abl = get_abilities();
	switch(id)
	{
		case PSX:
		{
			abl->psx.unlocked = 1;
			success = 1;
			break;
		}
		case KILL:
		{
			abl->kill.unlocked = 1;
			useMemory(getGame(NULL), KILL_UNLOCK_COST);
			success = 1;
			break;
		}
		default: 
		{
			fprintf(stderr, "Ability Switch Error\n");
			exit(1);
		}
	}
	textToTowerMonitor(unlockstring);	
	if(success == 1)
	{
		return 1;
	}
	return 0;	
}

int get_ability_cost(AbilityID id)
{
	Ability *a;
	switch(id)
	{
		case PSX:
		{
			a = &(get_abilities()->psx);
			break;
		}
		case KILL:
		{
			a = &(get_abilities()->kill);
			break;
		}
		default:
		{
			fprintf(stderr, "Is valid unlock Switch Error\n");
			exit(1);
		}
	}
	return a->cost;
}


int is_valid_unlock(AbilityID id)
{
	Ability *a;
	switch(id)
	{
		case PSX:
		{
			a = &(get_abilities()->psx);
			break;
		}
		case KILL:
		{
			a = &(get_abilities()->kill);
			break;
		}
		default:
		{
			fprintf(stderr, "Is valid unlock Switch Error\n");
			exit(1);
		}
	}
	if(a->unlocked == 0)
	{
		return 1;
	}
	return 0;
}
int is_ability_unlocked(AbilityID id)
{
    
    Ability *a;
    switch(id)
    {
        case PSX:
        {
            a = &(get_abilities()->psx);
            break;
        }
        case KILL:
        {
            a = &(get_abilities()->kill);
            break;
        }
        default:
        {
            fprintf(stderr, "Is avail Ability Switch Error\n");
            exit(1);
        }
    }
    if(a->unlocked == 1 )
    {
        return 1;
    }
 
    return 0;
}
int is_available_ability(AbilityID id)
{

	Ability *a;
	switch(id)
	{
		case PSX:
		{
			a = &(get_abilities()->psx);
			break;
		}
		case KILL:
		{
			a = &(get_abilities()->kill);
			break;
		}
		default: 
		{
			fprintf(stderr, "Is avail Ability Switch Error\n");
			exit(1);
		}
	}
	if(a->unlocked == 1 && getAvailableMemory() >= a->cost)
	{
		return 1;
	}
	else if(a->unlocked == 0 && getAvailableMemory() < a->cost)
	{
		errorToTerminalWindow("Error: Ability is not yet unlocked\nError: Insufficient Memory available");
	}
	else if(a->unlocked == 0 && getAvailableMemory() >= a->cost)
	{
		errorToTerminalWindow("Error: Ability is not yet unlocked");
	}
	else if(a->unlocked == 1 && getAvailableMemory() < a->cost)
	{
		errorToTerminalWindow("Error: Insufficient Memory available");
	}
	return 0;
}

int apt_get_query()
{
	int success = 0;
	char *statuslist = (char*) calloc(200, sizeof(char));
	char template[100] = {"APT_GET_QUERY\n\n"};
	char psxlocked[50] = "PSX ability is locked\n";
	char killlocked[50] = "Kill ability is locked\n";
	char psxunlocked[50] = "PSX ability is unlocked\n";
	char killunlocked[50] = "Kill ability is unlocked\n";
	char killunlockcost[50] = {'\0'};
	Abilities *a;	
	Ability *b, *c;

	a = get_abilities();
	b = &a->psx;
	c = &a->kill;
	
	strcpy(statuslist, template);
	if(b->unlocked == 1)
	{
		strcat(statuslist, psxunlocked);
	}
	else
	{
		strcat(statuslist, psxlocked);
	}
	if(c->unlocked == 1)
	{
		strcat(statuslist, killunlocked);
	}
	else
	{
		strcat(statuslist, killlocked);
	}

	sprintf(killunlockcost, "Kill ability unlock cost = %d", KILL_UNLOCK_COST);
	strcat(statuslist, killunlockcost);
	textToTowerMonitor(statuslist);
	if(statuslist[0] != '\0')
	{
		success = 1;
	}
	statuslist[0] = '\0';
	if(success == 1)
	{
		return 1;
	}
	return 0;
} 
	


int compare_health(const void*a, const void*b)
{
	const Enemyhealth* e1 = (const Enemyhealth*) a;
	const Enemyhealth* e2 = (const Enemyhealth*) b;

	return (e2->health - e1->health);
}

void psx_ability()
{
	char *psxlist = (char*) calloc(500,sizeof(char));	
	char line[400];
	int enemy_number = getNumberOfEnemies();
	int health = 0, ID = 0, i, j, count = 0;
	printf("%d\n", enemy_number);


	Enemyhealth *e_health = (Enemyhealth*) calloc(enemy_number + 1, sizeof(Enemyhealth));
	{
		for(j = 1; j <= enemy_number; j++)
		{
			e_health[j].health = getEnemyHealth(j);
			e_health[j].id = j;
		}

		qsort(&e_health[1], enemy_number, sizeof(Enemyhealth), &compare_health);
		strcpy(psxlist, "EnemyID Health\n");
		for(i = 1; i <= enemy_number; i++)
		{
			if(count < 10)
			{
				if(isDead(e_health[i].id) == 0)
				{
					ID = e_health[i].id;
					health = e_health[i].health;
					sprintf(line, "%d                %d\n", ID, health);
					strcat(psxlist, line);
					count++;
				}
			}
		}
        textToTowerMonitor(psxlist);
		test_psx_string(psxlist);
	}
	free(e_health);
	free(psxlist);
}

int kill_ability(int enemyID)
{
	if(is_available_ability(KILL))
	{
		if(checkClock(killSingle,KILL_SINGLE_COOLDOWN))	{
			killEnemy(enemyID);
			useMemory(getGame(NULL), KILL_COST);
			return 1;
		} else	{
			errorToTerminalWindow("Cooldown not yet ready");			
		}
	}
	return 0;
}

int kill_all_ability()
{
	int i;
	int enemy_number = getNumberOfEnemies();

	if(is_available_ability(KILL) && getAvailableMemory() >= KILL_ALL_COST)
	{
		if(checkClock(killAll, KILL_ALL_COOLDOWN))
		{
			for(i = 1; i <= enemy_number; i++)
			{
				drawKillAll();
				killEnemy(i);
			}
			useMemory(getGame(NULL), KILL_ALL_COST);
			return 1;
		} 
		else 
		{
			errorToTerminalWindow("Cooldown not yet ready");			
			return 0;	
		}	
	}
	return 0;
}

char *test_psx_string(char *psxlist)
{
	static char list[2000];
	if(psxlist != NULL)
	{
		strcpy(list, psxlist);
	}
	return list;
}

void testAbilities()
{
	sput_start_testing();
	sput_set_output_stream(NULL);
	
	sput_enter_suite("Testing Unlock Functions");
	sput_run_test(testunlocks);
	sput_leave_suite();

	sput_enter_suite("psx_ability(): Testing PSX");
	sput_run_test(testpsx);
	sput_leave_suite();
	
	sput_enter_suite("kill_all_ability(): Testing Kill all");
	sput_run_test(testkillall);
	sput_leave_suite();

	sput_finish_testing();
}


void testpsx()
{
	getGame(NULL);
	createEnemy();
	setEnemyHealth(1,100);
	int enemy_number = getNumberOfEnemies();
	sput_fail_if(enemy_number == 0, "Enemies found should = 1");
	psx_ability();
	sput_fail_if(strlen(test_psx_string(NULL)) == 0, "String not created");
	freeAllEnemies();
} 

void testkillall()
{
	getGame(NULL);
	createEnemy();
	setEnemyHealth(1,100);
	int enemy_number = getNumberOfEnemies();

	addMemory(1000);

	delayGame(600);
	sput_fail_if(enemy_number != 1, "Enemies found should = 1");
	unlock_ability(KILL);
	sput_fail_unless(kill_all_ability() == 1, "Enemy should be killed");
	freeAllEnemies();
}

void testunlocks()
{
	init_abilities();
	sput_fail_unless(get_ability_cost(PSX) == 0, "PSX cost should be 0 after initialization");
	sput_fail_unless(get_ability_cost(KILL) > 0, "KILL cost should be greater than 0 after initialization");
	sput_fail_unless(is_valid_unlock(KILL) == 1, "Valid unlock should return 1 after kill initialization");
	sput_fail_unless(apt_get_query() == 1, "apt_get_query should return 1 on successful string creation");
	sput_fail_unless(unlock_ability(KILL) == 1, "On successful unlock, function should return 1");
}


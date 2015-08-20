//
// sputFunctions.c
// Created by bclarke on 2014/09/11
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Custom Headers	-----------*/
#include "../includes/sput.h"
#include "./../includes/gameProperties.h"

/*---------- Data Types -----------*/

struct gameClock	{

	clock_t start_t; //! time game started.  Must be ran at startup. 
	clock_t lastAction;
	ClockNode first;
	ClockNode last;
};

struct clockNode	{

	clockType type;
	clock_t time;
	ClockNode next;

};


struct gameProperties {

	int memoryUsed;
	int totalMemory;
	int currWaveNo;
	int totalWaveNo;
	int health;
    int costOfNewTower;
	GameClock clock;
	int deathCount;
	int createEnemyGroupDelay;
	int TotalEnemiesCurrentWave;	//!Holds the total number of enemies this wave
};

/*---------- Functions ----------*/

/*
 * Delays game by specified amount
 */
clock_t delayGame(int delayN)	{

	clock_t ticks1, ticks2,timeWaited;
    ticks1=clock();
    timeWaited = ticks2=ticks1;
    while((ticks2/(CLOCKS_PER_SEC/100)-ticks1/(CLOCKS_PER_SEC/100))<delayN){
        ticks2=clock();
		timeWaited = (ticks2/(CLOCKS_PER_SEC/100)-ticks1/(CLOCKS_PER_SEC/100));
	}
	return timeWaited;
}

int startNextWave()	{
	if(getTotalCurrentWaveEnemies() == getDeathCnt())	{
		if(getWave(getGame(NULL))  < getTotalWaveNo())	{
			resetEnemyCounts();
			setCurrWaveNum(getGame(NULL)->currWaveNo+1);
		} else {
            //printf("you have won the level\n");
		}
		return 1;
	}
	return 0;
}

void testStartNextWave()	{

		setCurrWaveNum(getGame(NULL)->currWaveNo+1);
		increaseEnemyNumbersThisWave(10);
		getGame(NULL)->deathCount = 0;
		sput_fail_unless(startNextWave() == 0, "Invalid: 10 enemies have not registered as dead yet");	
		getGame(NULL)->deathCount = 10;
		sput_fail_unless(startNextWave() == 1, "Valid: 10 enemies have registered as dead");	
}


void setCurrWaveNum(int newWave)	{

	getGame(NULL)->currWaveNo = newWave;

}

/*
 *Increases the number of enemies that are to be created this wave.
 */
void increaseEnemyNumbersThisWave(int numberOfEnemies)	{

	getGame(NULL)->TotalEnemiesCurrentWave +=numberOfEnemies;
}

void resetEnemyCounts()	{
	getGame(NULL)->TotalEnemiesCurrentWave = 0;
	getGame(NULL)->deathCount = 0;
}

/*
 *
 */
int getTotalCurrentWaveEnemies()	{

	return getGame(NULL)->TotalEnemiesCurrentWave;
}

/*
 *Sets lastAction member to current clock time
 */
int setlastAction(GameProperties Game)	{
	Game->clock->lastAction = clock()/CLOCKS_PER_SEC;
	return (int) Game->clock->lastAction;
}

void setClock(clockType clockToSet)	{

		//*clockToSet = clock()/CLOCKS_PER_SEC;
}

/*
 *Increases Death Count
 */
void increaseDeathCnt()	{

	GameProperties game = getGame(NULL);
	game->deathCount++;
}


/*
 *Returns Death Count
 */
int getDeathCnt()	{

	return getGame(NULL)->deathCount;
}

/*
 *Returns total waves for current level
 */
int getTotalWaveNo()	{

	return getGame(NULL)->totalWaveNo;
}

/*
 *Sets total Waves this level
 */
void setTotalWaveNo(int totalW)	{

		getGame(NULL)->totalWaveNo = totalW;
	
}

void testClocks()	{
	sput_fail_unless(addClock(testClock) == 1,"Valid: Adding Dummy Test Clock");
	sput_fail_unless(checkUniqueClockType(testClock) == 0,"Invalid: Unique clock checker.  Clocck should already exist");
	sput_fail_unless(addClock(testClock) == 0,"Invalid: Adding non unique Dummy clock");
	sput_fail_unless(checkClock(testClock,10) == 0,"Invalid: Cooldown of 10 has not passed");
	delayGame(10);
	sput_fail_unless(checkClock(testClock,10) == 1,"Valid: Cooldown of 10 has passed");
	sput_fail_unless(checkClock(testClock,10) == 0,"Invalid: Cooldown of 10 hasn't passed after reset of cooldown");
	delayGame(10);
	sput_fail_unless(checkClock(testClock,10) == 1,"Valid: Cooldown of 10 has passed after reset of cooldown");

}

void testSetLastAction()	{

	GameProperties newGame = getGame(NULL);
	clock_t currTime = clock()/CLOCKS_PER_SEC;
	delayGame(2);
	sput_fail_unless(setlastAction(newGame) == (currTime + 2),"Setting Last Action to current time");
	delayGame(2);
	sput_fail_unless(setlastAction(newGame) == (currTime + 4),"Setting Last Action to current time");
}

/*
 * returns true is last action time is more than global cooldown
 */
int lastAction(GameProperties Game)	{

	clock_t currTime = clock() / CLOCKS_PER_SEC;

	clock_t timeSinceLastAction = currTime - Game->clock->lastAction;
	if (timeSinceLastAction >= ACTIONCOOLDOWN)	{
		Game->clock->lastAction = currTime;
		return 1;	
	}
		return 0;
}



/*
 *Damages play health with specified amount of damage
 */
void damageHealth(int damage)	{

	getGame(NULL)->health -= damage;
}



void testlastAction()	{

	GameProperties newGame = getGame(NULL);
	delayGame(ACTIONCOOLDOWN);
	sput_fail_unless(lastAction(newGame) == 1,"Checking delay more than Cooldown returns true");
	delayGame(ACTIONCOOLDOWN-1);
	sput_fail_unless(lastAction(newGame) == 0,"Checking delay less than Cooldown returns false");

}

void testingGameStructure()	{
	sput_start_testing();
	
	sput_set_output_stream(NULL);

	sput_enter_suite("testStartNextWave(): Testing wave management");
	sput_run_test(testStartNextWave);
	sput_leave_suite();

	sput_enter_suite("testClocks(): Testing Clock Data Structure");
	sput_run_test(testClocks);
	sput_leave_suite();

	sput_enter_suite("CreateGameTest(): Creation & Initialization");
	sput_run_test(CreateGameTest);
	sput_leave_suite();

	sput_enter_suite("TestGetAvailableMemory(): Current Memory Available");
	sput_run_test(TestGetAvailableMemory);
	sput_leave_suite();

	sput_enter_suite("TestAddMemory(): Adding Memory");
	sput_run_test(TestAddMemory);
	sput_leave_suite();

	sput_enter_suite("TestUseMemory(): Using More Memory");
	sput_run_test(TestUseMemory);
	sput_leave_suite();

	sput_finish_testing();
}

void CreateGameTest()	{

	GameProperties testGame;
	testGame = getGame(NULL);
	sput_fail_unless(getAvailableMemory() == 1000,"Initializing Memory");
	//sput_fail_unless(getWave(testGame) == 3,"Initializing WaveNo");
	sput_fail_unless(getTotalWaveNo() == 3,"Total Wave Number set to 3 from level file");
	sput_fail_unless(getHealth(testGame) == 100,"Initializing Health");
}

/*
 *Returns or sets memory address of game structure`
 */
GameProperties getGame(GameProperties createdGame)	{

	static GameProperties game;

	if (createdGame != NULL)	{
		game = createdGame;
	}
	return game;

}

/*
 *Returns the current wave number
 */
int getWave(GameProperties game)	{

	return game->currWaveNo;

}

/*
 *Returns total number of waves
 */
int getTotalWaves(GameProperties game) {
    
    return game->totalWaveNo;
}

int getHealth(GameProperties game)	{

	return game->health;
}


/*
 * Checks if health is 0
 */
int checkIfPlayerDead()   {

    if(getHealth(getGame(NULL)) <= 0)   {
        return 1;
    }
    
    return 0;

}

/*
 *Returns cost of new tower
 */
int getCostOfNewTower() {

    return getGame(NULL)->costOfNewTower;
}
/*
 *Creating Game Clock linked list
 */
GameClock createClock()	{
	GameClock clock = (GameClock) malloc(sizeof(*clock));	
	clock->first = NULL;
	clock->last = NULL;
	getClock(clock);
	return clock;
}

GameClock getClock(GameClock clock)	{

		static GameClock currClock;
		if(clock != NULL)	{
			currClock = clock;
		}

		return currClock;

}

/*
 * Add Clock Node
 */
int addClock(clockType type)	{
	GameClock clock = getClock(NULL);	
	if(checkUniqueClockType(type))	{
		if(clock->first == NULL)	{
			clock->first = clock->last = createClockNode(type);
		} else {
			clock->last->next = createClockNode(type);
			clock->last = clock->last->next;
		}
	} else {
		fprintf(stderr,"Attempt to add non unique clock\n");
		return 0;
	}
	return 1;
}

ClockNode createClockNode(clockType type)	{
	ClockNode newNode;
	newNode = (ClockNode) malloc(sizeof(*newNode));
	newNode->next = NULL;
    newNode->time = clock() / (CLOCKS_PER_SEC/100);
	newNode->type = type;
	return newNode;
}

/*
 *Checks Clock Type is unique
 */

int checkUniqueClockType(clockType type)	{

	GameClock clock = getClock(NULL);
	ClockNode currNode;
	currNode = clock->first;
	while(currNode!= NULL)	{
		if (currNode->type == type)	{
			return 0;
		}
		currNode = currNode->next;
	}

	return 1;

}


void freeClocks()	{
	GameClock gClock = getClock(NULL);
	ClockNode currNode = gClock->first;
	ClockNode temp;
	while(currNode != NULL)	{
		temp = currNode->next;	
		free(currNode);
		currNode = temp;
	}

	free(gClock);
}

ClockNode findClock(clockType cType)	{
    GameClock gClock = getClock(NULL);
    ClockNode currNode;
    currNode = gClock->first;	
	while(currNode != NULL)	{
		if (currNode->type == cType)	{
			return currNode;
		}
	   currNode = currNode->next;	
	}
	return 0;
}

int checkClock(clockType cType,int coolDown)	{
	GameClock gClock = getClock(NULL);
	ClockNode currNode;
	currNode = gClock->first;
	clock_t currTime = (double) clock() / (CLOCKS_PER_SEC/100);
	while(currNode != NULL)	{
		if (currNode->type == cType)	{
			if((currTime - currNode->time) >= coolDown)	{
				setCurrTime(currNode);
				return 1;
			} else {
				return 0;
			}	
		}
		currNode = currNode->next;
	}

	fprintf(stderr,"clock does not exist\n");
	return 0;
}

void setCurrTime(ClockNode node)	{
	node->time = (double) clock() / (CLOCKS_PER_SEC/100);
}

/*
 *Creates game structure.  Needs to be run in level init
 */
GameProperties createGame()	{

	GameProperties newGame = (GameProperties) malloc(sizeof(*newGame));
	newGame->clock = createClock();

	newGame->totalMemory=1000;
	newGame->memoryUsed=0;
	newGame->currWaveNo=0;
	newGame->totalWaveNo = 0;
	newGame->health=100;
    newGame->costOfNewTower = 400;
	newGame->deathCount = 0;
	newGame->clock->start_t  = newGame->clock->lastAction = (double) clock()/CLOCKS_PER_SEC;
	newGame->createEnemyGroupDelay=0;
	newGame->TotalEnemiesCurrentWave = 0;
	getGame(newGame);
	return newGame;

}

int getEnemyGroupDelay()
{
	return(getGame(NULL)->createEnemyGroupDelay);
}

int setCreateEnemyGroupDelay(int delay)	{

	getGame(NULL)->createEnemyGroupDelay = delay;
	return delay;
}

/*
 *Returns amount of Memory available
 */
int getAvailableMemory()
{
	return getGame(NULL)->totalMemory - getGame(NULL)->memoryUsed;
}

int getTotalMemory()
{
	return getGame(NULL)->totalMemory;
}


void TestGetAvailableMemory()
{
	GameProperties testGame;
    testGame = createGame();
	testGame->totalMemory = 10;
	sput_fail_unless(getAvailableMemory() == 10,"Getting Memory");	
	free(testGame);
}

/*
 *Adds specified amount of Memory to resources
 */
int addMemory(int mem)          {
	GameProperties game = getGame(NULL);
	if(mem > 0)	{
		game->totalMemory+=mem;	
		return 1;
	} 

	return 0;
}
/*
 *Setting available memory to specified value
 */
void setMemory(int newMem)	{
	GameProperties game = getGame(NULL);
	if(newMem>= 0)	{
		game->totalMemory = newMem;
		game->memoryUsed = 0;
	}
}

void TestAddMemory()	{

	GameProperties testGame;
    testGame = createGame();
	addMemory(100);
	sput_fail_unless(getAvailableMemory() == 100,"Adding MEmory");
	sput_fail_unless(addMemory(-100) == 0,"Adding Negative Memory");
	free(testGame);
}

/*
 *Uses specified amount of memory
 */
int useMemory(GameProperties game,int mem)	{

	if (game->totalMemory-game->memoryUsed >= mem)	{
		game->memoryUsed+=mem;
		return 1;
	} else {
		return 0;
	}
}

void TestUseMemory()	{

	GameProperties testGame;
    testGame = createGame();
	testGame->totalMemory = 100;
	useMemory(testGame,50);
	sput_fail_unless(getAvailableMemory() == 50,"Subtracting Memory");
	sput_fail_unless(useMemory(testGame,100) == 0,"Subtracting too much Memory");
	free(testGame);
}

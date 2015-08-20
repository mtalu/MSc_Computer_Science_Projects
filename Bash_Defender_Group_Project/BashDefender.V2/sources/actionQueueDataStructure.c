//
// sputFunctions.c
// Created by bclarke on 2014/09/11
// tags: stack, abstraction, struct pointers
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*---------- Hash Defines -----------*/
#define ITEMS_IN_ACTION_QUEUE 5

/*---------- Custom Headers	-----------*/

#include "../includes/sput.h"
#include "../includes/actionQueueDataStructure.h"
#include "../includes/abilities.h"

/*---------- Data Types -----------*/

struct queueNode {
	
	cmdType command; //command for execution
	cmdOption option; //stat to change
	int target;	//target of command
    struct queueNode *nextNode;

};

struct actionQueueStructure  {
	
    struct queueNode *start;
    struct queueNode *current; //!back of queue
    int nItems;

};


/*---------- Functions ----------*/

void testingActionQueue()	{

	sput_start_testing();
	
	sput_set_output_stream(NULL);

	sput_enter_suite("testCheckMem(): memory check");
    sput_run_test(testCheckMem);
  	sput_leave_suite();
	

	sput_enter_suite("testPushToQueue(): Pushing to queue");
	sput_run_test(testPushToQueue);
	sput_leave_suite();

	sput_finish_testing();
}

/*
 *Creates queue.  Must be run at the start of every game.
 */
ActionQueueStructure createActionQueue()	{

	ActionQueueStructure newActionQueue = (ActionQueueStructure) malloc(sizeof(*newActionQueue));
	newActionQueue->nItems = 0;
	newActionQueue->start = newActionQueue->current = NULL;
	getQueue(newActionQueue);

	return newActionQueue;
}

void clearQueue()	{

	ActionQueueStructure q = getQueue(NULL);
	QueueNode currNode = q->start;
	QueueNode temp;
	while(currNode != NULL)	{
		temp = currNode->nextNode;
		free(currNode);
		currNode = temp;	
	}
	q->start = q->current = NULL;
}

void freeActionQueue()	{

		clearQueue();
		free(getQueue(NULL));

}

/*
 *puts node to back of queue
 */
int createNode(ActionQueueStructure queue)	{
	
	if(queue->start == NULL )	{
		queue->start = queue->current = (QueueNode) malloc(sizeof(*(queue->start)));
		queue->start->nextNode = NULL;
	} else {
		queue->current->nextNode = (QueueNode) malloc(sizeof(*(queue->current->nextNode)));
		queue->current = queue->current->nextNode;
		queue->current->nextNode = NULL;
	}

	return 1;

}

/*
 *Returns actionqueue address
 */
ActionQueueStructure getQueue(ActionQueueStructure queue)	{

	static ActionQueueStructure stQueue;

	if(queue != NULL)	{
		stQueue = queue;	
	}
	
	return stQueue;

}
/*
 * Pushes values to newly created node at back of queue
 */
int pushToQueue(ActionQueueStructure queue, cmdType command, cmdOption option, int target)	{
		createNode(queue);
		queue->current->command = command;
		queue->current->option = option;
		queue->current->target = target;
		(queue->nItems)++;
		return (queue->nItems);
}

void testPushToQueue()	{
	
    cmdType nCommand_1=cmd_upgrade;
    cmdOption nStat_1=upgrade_power;
    int tar_1 = 1;

    cmdType nCommand_2=cmd_upgrade;
    cmdOption nStat_2=upgrade_range;
    int tar_2 = 2;

    ActionQueueStructure newQueue = getQueue(NULL);

	sput_fail_unless(pushToQueue(newQueue,nCommand_1,nStat_1,tar_1) == 1,"Valid: 1 Queue Item");
	sput_fail_unless(pushToQueue(newQueue,nCommand_2,nStat_2,tar_2) == 2,"Valid: 2 Queue Items");
	sput_fail_unless(getFirstCommand(newQueue) == cmd_upgrade,"Valid: Top of Queue Upgrade Command");
	sput_fail_unless(getFirstOption(newQueue) == upgrade_power,"Valid: Top of Queue Power Option");
	sput_fail_unless(getLastCommand(newQueue) == cmd_upgrade,"Valid: Last in Queue upgrade Command");
	sput_fail_unless(getLastOption(newQueue) == upgrade_range,"Valid: Last of Queue range Option");
	pushToQueue(newQueue,cmd_mktwr,mktwr_int,2);
	sput_fail_unless(getLastCommand(newQueue) == cmd_mktwr,"Valid: Last in Queue make tower command");
	sput_fail_unless(getLastOption(newQueue) == mktwr_int,"Valid: Last option in Queue is int tower");
	clearQueue();
}

/*
 *Returns first command in queue
 */
cmdType getFirstCommand(ActionQueueStructure queue)	{

	return queue->start->command;
}

/*
 *Returns last command in queue
 */
cmdType getLastCommand(ActionQueueStructure queue)	{

	return queue->current->command;
}

/*
 *Returns first option in queue
 */
cmdOption getFirstOption(ActionQueueStructure queue)	{

	if(queue->start != NULL)	{
		return queue->start->option;	
	}

	return 0;
}

/*
 *Returns last option in queue
 */
cmdOption getLastOption(ActionQueueStructure queue)	{

	return queue->current->option;

}

/*
 *Returns first target in queue
 */
int getFirstTarget()	{

	return (getQueue(NULL)->start->target);

}

/*
 *Returns last target in queue
 */
int getLastTarget()	{

	return (getQueue(NULL)->current->target);

}
/*
 *Returns costs of command based on current tower stats
 */
int calculateCosts(cmdType cmd, cmdOption opt, int target)    {

    switch(cmd)
    {
        case cmd_upgrade:
        {
            if(opt == upgrade_slowPower) {
                return 300 * (getCurrentStat(opt,target)+1);
            } else {
                return ((getTowerLevel(target))*(2*getCurrentStat(opt,target)));
            }
        }
        case cmd_mktwr:
        {
            return getCostOfNewTower();
        }
        case cmd_aptget:
        {
            return getCostOfAptget(opt);
        }
        default:

        break;
    }

    return 0;
}



int costOfUpgradeFactoringInTheUpgradesOnTheQueue( int target, cmdOption stat)
{
    ActionQueueStructure q = getQueue(NULL);
    QueueNode currentNode = q->start;
    
    int upgradesToThisStatAndTowerInTheQueue=0;
    int towerUpgradesToBeCompleted = getUpgradesCompleted(target);
    
    int towerLevel = getTowerLevel(target);
    while(currentNode != NULL)
    {
        if(currentNode->command == cmd_upgrade &&
           currentNode->target == target )
        {
            ++towerUpgradesToBeCompleted;
            if(towerUpgradesToBeCompleted % UPGRADES_PER_LEVEL == 0)
            {
                ++towerLevel;

            }
            if(currentNode->option == stat)
            {
                
                switch(stat)
                {
                    case upgrade_power:
                    {
                        upgradesToThisStatAndTowerInTheQueue += DAMAGE_UPGR_VAL;
                        break;
                    }
                    case upgrade_speed:
                    {
                        upgradesToThisStatAndTowerInTheQueue += SPEED_UPGR_VAL;
                        break;
                    }
                    case upgrade_range:
                    {
                        upgradesToThisStatAndTowerInTheQueue += RANGE_UPGR_VAL;
                        break;
                    }
                    case upgrade_AOEpower:
                    {
                        upgradesToThisStatAndTowerInTheQueue += AOE_POWER_UPGR_VAL;
                        break;
                    }
                    case upgrade_AOErange:
                    {
                        upgradesToThisStatAndTowerInTheQueue += AOE_RANGE_UPGR_VAL;
                        break;
                    }
                    case upgrade_slowPower:
                    {
                        upgradesToThisStatAndTowerInTheQueue += SLOW_POWER_UPGR_VAL;
                        break;
                    }
                    case upgrade_slowDuration:
                    {
                        upgradesToThisStatAndTowerInTheQueue += SLOW_DUR_UPGR_VAL;
                        break;

                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
        currentNode=currentNode->nextNode;
    }

    int costOfUpgradeWillBe = towerLevel*2*( getCurrentStat(stat,target) +
                                            upgradesToThisStatAndTowerInTheQueue );
    return costOfUpgradeWillBe;
}


int getCostOfAptget (cmdOption option)
{
    if(option==aptget_kill)
    {
        return KILL_COST;
    }
    else return 0;
    
}
/*
 *Returns specified stat of specified tower
 */
int getCurrentStat(cmdOption stat,int target)	{

	switch(stat)	{
		case upgrade_power:
				return getTowerDamage(target);
				break;
		case upgrade_range:
				return getTowerRange(target)/2;
				break;	
		case upgrade_speed:
				return getTowerSpeed(target);	
				break;	
		case upgrade_AOErange:
				return getTowerAOErange(target);
				break;	
		case upgrade_AOEpower:
				return getTowerAOEpower(target);
				break;
        case upgrade_slowPower:
            return getTowerSlowPower(target);
            break;
        case upgrade_slowDuration:
            return getTowerSlowDuration(target);
            break;
		case upgrade_level:
				return getTowerLevel(target);
				break;	
		default:
				return 0;
				break;
	}
}

/*
 *Upgrades tower stat
 */
cmdOption upgradeTowerStat(cmdOption stat, int target)  {

    switch(stat)    {
        case upgrade_power:
        {
            if(upgradeDmg(target))  {
                return upgrade_power;
            }
			break;
        }
        case upgrade_range:
        {
            if(upgradeRange(target))    {
                return upgrade_range;
            }
			break;
        }
        case upgrade_speed:
        {
            if(upgradeSpeed(target))    {
                return upgrade_speed;
            }
			break;
        }
        case upgrade_AOErange:
        {
            if(upgradeAOErange(target)) {
                return upgrade_AOErange;
            }
			break;
        }
        case upgrade_AOEpower:
        {
            if(upgradeAOEpower(target)) {
                return upgrade_AOEpower;
            }
			break;
        }
        case upgrade_slowPower:
        {
            if(upgradeSlowPower(target)) {
                return upgrade_slowPower;
            }
            break;
        }
        case upgrade_slowDuration:
        {
            if(upgradeSlowDuration(target)) {
                return upgrade_slowDuration;
            }
            break;
        }
        default:
            fprintf(stderr,"upgradeTowerStat tower.c: unrecognised stat\n");
            return optionError;
    }
	return 0;
}

int startOfQueueCalc()	{
	ActionQueueStructure queue = getQueue(NULL);
	if(queue->start != NULL)	{
		return calculateCosts(queue->start->command,queue->start->option,queue->start->target);
	}
	return 0;
}
/*
 *Checks start of action Queue for command, and actions it if all criteria are met
 */
int popToTower()	{
	ActionQueueStructure queue = getQueue(NULL);
	GameProperties Game = getGame(NULL);
	int needed;
	if(queue->start != NULL) {
		needed = calculateCosts(queue->start->command,queue->start->option,queue->start->target);
		switch(queue->start->command)	{
			case cmd_upgrade:
				if (checkQueue(queue, Game,needed)) {
					upgradeTowerStat(queue->start->option,queue->start->target);
					useMemory(Game, needed);
					removeQueueItem();
				}
				break;
			case cmd_mktwr:
				if (checkQueue(queue,Game,needed)) {
					switch(queue->start->option)	{
						case mktwr_int:
							createTowerTypeFromPositions(queue->start->target,INT_TYPE);	
							break;
						case mktwr_char:
							createTowerTypeFromPositions(queue->start->target,CHAR_TYPE);	
							break;
						default:
							fprintf(stderr,"Unrecognised tower type\n");
					        break;
					}
					useMemory(Game, needed);
					removeQueueItem();
				}
				break;
			case cmd_aptget:
				if(checkQueue(queue,Game,needed)) {
					unlock_ability(KILL);
					useMemory(Game, needed);					
					removeQueueItem();
				}
			default:

				break;
		}
	} else {
		return 0;
	}
	return 1;
}

void removeQueueItem()	{
    ActionQueueStructure queue = getQueue(NULL);
    GameProperties Game = getGame(NULL);
	QueueNode tempStart = queue->start;
    queue->start = queue->start->nextNode;
	free(tempStart);
	setlastAction(Game);
	--(queue->nItems);
}

int getNumOfQueueItems()	{
	return getQueue(NULL)->nItems;
}

/*
 * Checks Top of queue to ensure target and time is correct
 */
int checkQueue(ActionQueueStructure queue, GameProperties Game, int needed)	{
	if((checkMem(needed, Game)) && (checkClock(lastCmdAction,ACTIONCOOLDOWN)))	{
			return 1;		
	}
	return 0;	
}

/*
 *Checks required Memory is less than the Memory available to use
 */

int checkMem(int needed, GameProperties Game)	{
	if(needed > getAvailableMemory(Game)) {
		return 0;
	}
		return 1;
}

void testCheckMem()	{

	GameProperties testGame;
    testGame = getGame(NULL);
	setMemory(0);
	addMemory(10);
	sput_fail_unless(checkMem(10,testGame) == 1,"boundary Testing enough memory");
	useMemory(testGame,10);
	sput_fail_unless(checkMem(50,testGame) == 0,"Testing not enough memory");
	addMemory(100);
	sput_fail_unless(checkMem(100,testGame) == 1,"Testing enough memory");
	setMemory(0);
	test_KillAnEnemy();
	sput_fail_unless(getAvailableMemory() > 0, "Valid: More memory available after killing an enemy");
	freeAllEnemies();
}

/*
 *Extract display string from first N nodes in action queue and return output string
 */

char *getActionQueueString(void) {
    
    QueueNode p = getQueue(NULL)->start;
    char *outputString = calloc(500,sizeof(char));
    char *targetString = calloc(10,sizeof(char));
    for(int count = 0; p!= NULL && count < ITEMS_IN_ACTION_QUEUE; p = p->nextNode, count++) {
        cmdType command = p->command;
        cmdOption option = p->option;
        int target = p->target;
        switch(command) {
            case cmd_upgrade:
                strcat(outputString, "upgrade ");
                switch(option) {
                    case upgrade_power:
                        strcat(outputString, "p ");
                        break;
                    case upgrade_range:
                        strcat(outputString, "r ");
                        break;
                    case upgrade_speed:
                        strcat(outputString, "s ");
                        break;
                    case upgrade_AOErange:
                        strcat(outputString, "AOEr ");
                        break;
                    case upgrade_AOEpower:
                        strcat(outputString, "AOEp ");
                        break;
                    case upgrade_slowPower:
                        strcat(outputString, "slow power ");
                    case upgrade_slowDuration:
                        strcat(outputString, "slow duration ");
                    case upgrade_level:
                        strcat(outputString, "level ");
					default:
						break;
                }
                
                if(target) {
                    sprintf(targetString, " t%d", target);
                    strcat(outputString, targetString);
                }
                break;
            case cmd_mktwr:
                strcat(outputString, "mktwr ");
                switch(option) {
                    case mktwr_int:
                        strcat(outputString, "INT ");
                        break;
                    case mktwr_char:
                        strcat(outputString, "CHAR ");
                        break;
					default:
						break;
                }
                
                if(target) {
                    sprintf(targetString, "%c", 'A' + (target - 1));
                    strcat(outputString, targetString);
                }

                break;
            case cmd_aptget:
                strcat(outputString, "aptget");
                break;
            default:
                continue;
        }
        
        strcat(outputString, "\n");
    }
    
    return outputString;
}

#ifndef actionQueueDataStructure_h
#define actionQueueDataStructure_h

#include "../includes/debug.h"
#include "../includes/gameProperties.h"
#include "../includes/tower.h"

/*----------Enumerated Types-----------*/

/*----------Symbolic Constants-----------*/

/*----------TypeDefs-----------*/



typedef struct queueNode *QueueNode;
typedef struct actionQueueStructure *ActionQueueStructure;

/*----------Function Prototypes-----------*/
int checkTime(GameProperties Game);
int checkQueue(ActionQueueStructure queue, GameProperties Game, int needed);
ActionQueueStructure createActionQueue();
int createNode(ActionQueueStructure queue);
int pushToQueue(ActionQueueStructure queue, cmdType command, cmdOption option, int target);
int popFromQueue(ActionQueueStructure queue, cmdType *cmd, cmdOption *stat, int *target);
int checkMem(int needed, GameProperties Game);
cmdType getFirstCommand(ActionQueueStructure queue);
cmdType getLastCommand(ActionQueueStructure queue);
cmdOption getFirstOption(ActionQueueStructure queue);
cmdOption getLastOption(ActionQueueStructure queue);
ActionQueueStructure getQueue(ActionQueueStructure queue);
int getLastTarget();
int getFirstTarget();
void clearQueue();
void removeQueueItem();
int calculateCosts(cmdType cmd, cmdOption stat, int target);
int costOfUpgradeFactoringInTheUpgradesOnTheQueue( int target, cmdOption stat);
int getCurrentStat(cmdOption stat,int target);
int popToTower();
char *getActionQueueString(void);
int getCostOfAptget (cmdOption option);
void freeActionQueue();
int startOfQueueCalc();
int getNumOfQueueItems();
cmdOption upgradeTowerStat(cmdOption stat, int target);

/*----------Testing Function Prototypes-----------*/
void testCheckMem();
void testingActionQueue();
void testPushToQueue();
#endif

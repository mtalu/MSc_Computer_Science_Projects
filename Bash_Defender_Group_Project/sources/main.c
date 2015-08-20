#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../includes/sput.h"
#include "../includes/levelController.h"
#include "../includes/abilities.h"
#include "../includes/tower.h"
#include "./../includes/actionQueueDataStructure.h"
#include "../includes/parser.h"
#include "../includes/main.h"
#include "../includes/enemy.h"
#include "../includes/Sound.h"
#include "../includes/abilities.h"
#include "../includes/Information_Window.h"

#define TESTING 1


int main(int argc, char ** argv)
{
    
	Display d = init_SDL();
    if(TESTING) {
        testing();
        exit(EXIT_SUCCESS);
    }
	int restart = 0;
    int started = 0;
    
    srand(time(NULL));
	gameState state = menu;
	do{
		switch(state)	{
			case menu:
			    while(state == menu){
			        menu_screen(d, &state);
			    }
				break;
			case tutorial:
					initLevel(0);
					tutorialLevel(d,&restart);
					endLevel(&restart);
					state = menu;
				break;
			case level1:
					initLevel(1);
					startLevel(d,&restart);
					endLevel(&restart);
					state = menu;
				break;
			default:
				exit(1);	
		}
	}	while(1);

    /*while(started == 0){
    	menu_screen(d, &state);
    }*/
/*
	do	{
		restart = 0;
    	initLevel(1); //For tutorial level, change to 0, uncomment tutorial level, comment startlevel for tutorial
		//tutorialLevel(d,&restart);
		startLevel(d,&restart);
		endLevel();
	} while (restart);
*/
    
    shutSDL(d);
    quitGame();
    return 0;
}

void startLevel(Display d, int *restart)	{

    char text[128] = {'>', '>'};
    char empty[128] = {'>', '>'};
    char *pass, *clear, *inputCommand=NULL;
    pass = text;
    clear = empty;
    int ended = 0;
   	int pause = 0; 
    int steps=0;

    //init_sound();
    //playBackgroundSound();
    do{
        startFrame(d);
		while(pause)	{
			pause_screen(d,&pause,restart);
		}
        ++steps;
        drawBackground();

    	startNextWave();
        levelQueueReader();
        terminal_window(d, pass, clear,&pause, *restart);
    	popToTower();
        if(inputCommand)
        {
            parse(inputCommand);
        }
        present_enemy(d);
        present_tower();

    	fire();
        for(int i=1; i<=getNumberOfEnemies(); ++i)
        {
            moveEnemy(i);
        }
        presentAnimation();
    	drawAllTowerPositions();
        updateAllInfoWindow();
        endFrame(d);
        
        //ended = checkIfPlayerDead();
        while (ended) {
            //final screen returns 1 if restart button was pressed...
            if (final_screen()){
                ended = 0;
            }
        }
        
    } while(!terminal_window(d, pass, clear,&pause, *restart));
		printf("finished\n");    
}


void tutorialLevel(Display d,int *restart)	{

    tutPhase tPhase = phaseOne;
    char text[128] = {'>', '>'};
    char empty[128] = {'>', '>'};
    char *pass, *clear, *inputCommand=NULL;
    pass = text;
    clear = empty;
    int ended = 0;
   	int pause = 0;
    int steps=0;
    
    //init_sound();
    //playBackgroundSound();
    addClock(tutorialClock);
    int damage, range, speed, resetTime = 0, flag = 1;
    int currMemory;
    do{
        startFrame(d);
        while(pause)	{
            pause_screen(d,&pause,restart);
        }
        ++steps;
        drawBackground();
        
        switch(tPhase)	{
                
            case phaseOne:
                if(flag)	{
                    setCurrTime(findClock(tutorialClock));
                    flag = 0;
                }
                tutorial_one();
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
                    tPhase++;
                }
                break;
            case phaseTwo:
                tutorial_two();
                if(getNumberOfTowers() > 0)	{
                    damage = getTowerDamage(1);
                    tPhase++;
                }
                break;
            case phaseThree:
                if(!flag)	{
                    setCurrTime(findClock(tutorialClock));
                    flag = 1;
                }
                tutorial_three();
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
                    tPhase++;
                }
                break;
            case phaseFour:
                startNextWave();
                tutorial_four();
                if(flag)	{
                    setCurrTime(findClock(tutorialClock));
                    flag = 0;
                }
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN))  {
                    tPhase++;
                }
                break;
            case phaseFive:
                tutorial_five();
                if(!flag)	{
                    setCurrTime(findClock(tutorialClock));
                    flag = 1;
                }
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN))  {
                    tPhase++;
                    currMemory = getAvailableMemory();
                }
                break;
            case phaseSix:
                if(getTowerDamage(1) > damage)	{
                    tutorial_six();
                    tPhase++;
                } else if(getAvailableMemory() < currMemory) {
                    tutorial_five_error();
                }
                break;
            case phaseSeven:
                startNextWave();
                if(getWave(getGame(NULL)) == 2)  {
                    if(flag)	{
                        setCurrTime(findClock(tutorialClock));
                        flag = 0;
                    }
                    if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
                        tPhase++;
                    }
                }
                break;
            case phaseEight:
                tutorial_seven();
                if(!flag)	{
                    setCurrTime(findClock(tutorialClock));
                    flag = 1;
                }
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
                    tPhase++;
                }
                break;
            case phaseNine:
                tutorial_eight();
                if(checkCharType())  {
                    tPhase++;
                }
                break;
            case phaseTen:
                startNextWave();
                if(getDeathCnt() > 0)	{
                    tPhase++;
                }
                break;
            case phaseEleven:
                if(flag)	{
                    setCurrTime(findClock(tutorialClock));
                    flag = 0;
                }
                tutorial_nine();
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN))  {
                    tPhase++;
                    speed = getTowerSpeed(1);
                }
                break;
            case phaseTwelve:
                tutorial_ten();
                if(getTowerSpeed(1) > speed)	{
                    tPhase++;
                    range = getTowerRange(1);
                }
                break;
            case phaseThirteen:
                tutorial_eleven();
				if(getFirstOption(getQueue(NULL)) == upgrade_range)	{
                    tPhase++;
					setMemory(0);
				}
                break;
            case phaseFourteen:
                if(!flag)	{
                    setCurrTime(findClock(tutorialClock));
                    flag = 1;
                }
                if(startOfQueueCalc() > getAvailableMemory(getGame(NULL))){
                    tutorial_fourteen();
                } else {
                    if(getTowerRange(1) > range)	{
                        tPhase++;
                    } else {
                        tutorial_sixteen();
                    }
                }
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN_LONG))  {
                    addMemory(1000);
                }
                break;
            case phaseFifteen:
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
                    tutorial_fifteen();	
                    tPhase++;
                }
                break;
            case phaseSixteen:
                if(flag)	{
                    setCurrTime(findClock(tutorialClock));
                    flag = 0;
                }
                tutorial_twelve();
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN))	{
                    tPhase++;
                }
				break;
            case phaseSeventeen:
                    if(!flag)   {
                        setCurrTime(findClock(tutorialClock));
                        flag = 1;
                    }
                    if(checkClock(tutorialClock,TUTORIALCOOLDOWN))  {
                    	tutorial_thirteen();
                        tPhase++;
                    }
					break;
            case phaseEighteen:
                    if(flag)    {
                        setCurrTime(findClock(tutorialClock));
                        flag = 0;
                    }
                    updateAllInfoWindow();
                    if(checkClock(tutorialClock,TUTORIALCOOLDOWN_LONG))  {
                        tPhase++;
                    }
                    break;  
		 	case phaseNineteen:
				if(!flag)	{
                	setCurrTime(findClock(tutorialClock));
                    flag = 1;
				}	
				tutorial_seventeen();
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN_LONG))  {
                	tPhase++;
                }
				break;
			case phaseTwenty:
				if(flag)	{
                	setCurrTime(findClock(tutorialClock));
                    flag = 1;
				}	
				tutorial_seventeen();
                if(checkClock(tutorialClock,TUTORIALCOOLDOWN_LONG))  {
                	tPhase++;
                }
				break;
			case phaseTwentyOne:
				if(flag)	{
                	setCurrTime(findClock(tutorialClock));
                    flag = 0;
				}
				tutorial_eighteen();
				if(getTowerType(1) == CHAR_TYPE && getTowerType(2) == INT_TYPE)	{
					tPhase++;
				}
				break;
			case phaseTwentyTwo:
				if(!flag)	{
                	setCurrTime(findClock(tutorialClock));
                    flag = 1;
				}
				exit(1);	
			default:
                break;
        }
        
        if(resetTime)	{
            setCurrTime(findClock(tutorialClock));
            resetTime = 0;
            
        }
        
        levelQueueReader();
        terminal_window(d, pass, clear,&pause, *restart);
        popToTower();
        if(inputCommand)
        {
            parse(inputCommand);
        }
        present_enemy(d);
        present_tower();

    	fire();
        for(int i=1; i<=getNumberOfEnemies(); ++i)
        {
            moveEnemy(i);
        }
        presentAnimation();
        drawAllTowerPositions();
        tutorialUpdateAllInfoWindow();
        endFrame(d);
        
        //ended = checkIfPlayerDead();
        while (ended) {
            //final screen returns 1 if restart button was pressed...
            if (final_screen()){
                ended = 0;
            }
        }
        
    } while(!terminal_window(d, pass, clear,&pause, *restart));
}


/*void startLevel(Display d, int *restart)	{

    char text[128] = {'>', '>'};
    char empty[128] = {'>', '>'};
    char *pass, *clear, *inputCommand=NULL;
    pass = text;
    clear = empty;
    int ended = 0;
   	int pause = 0; 
    int steps=0;

    //init_sound();
    //playBackgroundSound();
    do{
        startFrame(d);
		while(pause)	{
			pause_screen(d,&pause,restart);
		}
        ++steps;
        drawBackground();
        
    	startNextWave();
        levelQueueReader();
        terminal_window(d, pass, clear,&pause, *restart);
    	popToTower();
        if(inputCommand)
        {
            parse(inputCommand);
        }
        present_enemy(d);
        present_tower(d);

    	fire();
        for(int i=1; i<=getNumberOfEnemies(); ++i)
        {
            moveEnemy(i);
        }
        presentAnimation();
    	drawAllTowerPositions();
        updateAllInfoWindow();
        endFrame(d);
        
        //ended = checkIfPlayerDead();
        while (ended) {
            //final screen returns 1 if restart button was pressed...
            if (final_screen())	{
                ended = 0;
            }
        }
    } while(!terminal_window(d, pass, clear,&pause, *restart));
}
*/

void quitGame()
{
    freeParseLists();
    destroyEnvVarList();
    //shutSound();
//    freeEnemyGroup();
//    freeLevelPaths();

}

void testing()	{

	setUpTesting();
	//!Unit Tests	
	testLevelController(); //! Working

	testingProjectiles(); //! Working
    //testingTowerPositions(); //!Workingr
    //testingGameStructure(); //!Memory Tests Failing
    testingActionQueue(); //! Working
    testEnemy(); // ! Working.
    testParser();
    testingTowerModule(); //! working
    //testingInformationWindowModule();

   	//! System Tests 
	//queueToTowerTesting();
    //parseToQueueTesting(); //!Working
	//parseToTowerTesting(); //!Working
    //towerToEnemyTesting(); //! Doesnt work.  Firing and range dont seem to be working
	//enemyToGamePropertiesTesting();
    //testParserToInfoWindow();

}

void queueToTowerTesting(){

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testPopFromQueue(): Queue items getting popped and actioned in correct order");	
	sput_run_test(testPopFromQueue);
	sput_leave_suite();

	sput_finish_testing();	
}

void testPopFromQueue() {

    int tDmg, tRng, towerN;
    clearQueue();
	freeAllTowers();
    ActionQueueStructure newQueue = getQueue(NULL);
    addMemory(1000);
    createTowerFromPositions(1);
    tDmg = getTowerDamage(1);
    tRng = getTowerRange(1);
    pushToQueue(newQueue,cmd_upgrade,upgrade_power,1);
    pushToQueue(newQueue,cmd_upgrade,upgrade_range,1);
    popToTower();
    delayGame(ACTIONCOOLDOWN);
    sput_fail_unless(getTowerDamage(1) > tDmg,"Valid: Damage upgrade for tower 1 successfully popped");
    delayGame(ACTIONCOOLDOWN);
    popToTower();
    sput_fail_unless(getTowerRange(1) > tRng,"Valid: Range upgrade for tower 1 successfully popped");
    tRng = getTowerRange(1);
    pushToQueue(newQueue,cmd_upgrade,upgrade_range,1);
    popToTower();
    sput_fail_unless(getTowerRange(1) == tRng,"Invalid: Range upgrade for tower 1 is not ready: range state unchanged");
    setMemory(0);
    delayGame(ACTIONCOOLDOWN);
    popToTower();
    sput_fail_unless(getTowerRange(1) == tRng,"Invalid: Not enough memory to upgrade: range state unchanged");
    addMemory(1000);
    popToTower();
    sput_fail_unless(getTowerRange(1) > tRng,"Valid: Range upgrade for tower 1 successfully popped");
    towerN = getNumberOfTowers();
    pushToQueue(newQueue,cmd_mktwr,mktwr_int,2);
    delayGame(ACTIONCOOLDOWN);
    popToTower();
    sput_fail_unless(getNumberOfTowers() > towerN,"Valid: Number of towers has increased");
	freeAllTowers();
	clearQueue();
}

void enemyToGamePropertiesTesting()	{

	sput_start_testing();
    sput_set_output_stream(NULL);

	sput_enter_suite("testEnemyDeath(): Game Properties capturing enemy deaths corectly");
	sput_run_test(testEnemyDeath);
	sput_leave_suite();

	sput_finish_testing();
}

void testEnemyDeath()	{
	int enemyID = createSpecificEnemy(1,1,1), 
	currDeathCnt = getDeathCnt(),
	currMemory = getAvailableMemory();
	damageEnemy(50,enemyID,1);
	sput_fail_unless(getDeathCnt() > currDeathCnt, "Valid: One Enemy has died");
	sput_fail_unless(getAvailableMemory() > currMemory,"Valid: Enemy has died and added to available memory");
	resetEnemyCounts();
}

void towerToEnemyTesting()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testEnemyInRange(): tower detecting single enemy in range");
	sput_run_test(testEnemyInRange);
	sput_leave_suite();

	sput_finish_testing();
}

void testEnemyInRange()	{

	createEnemy();
	setEnemyHealth(1,100);
	setEnemyArmour(1,0);
	setEnemyX(1,50);
	setEnemyY(1,50);
	setTowerY(1,400);
	setTowerX(1,400);
	setTowerRange(1,15);
	setTowerDamage(1,10);
	sput_fail_unless(inRange(400,400,10,1)== 0, "Enemy 1 is out of range of tower 1");
	fire();
	sput_fail_unless(getEnemyHealth(1) == 100, "Out of range enemy 1 has full health after tower has fired");
	setEnemyX(1,400);
	setEnemyY(1,400);
	sput_fail_unless(inRange(400,400,39,1)== 1, "Enemy 1 is in range of tower 1");
	sput_fail_unless(getEnemyHealth(1) == 100, "Enemy 1 has full health");
	fire();
	//sput_fail_unless(getEnemyHealth(1) == 100 - getTowerDamage(1),"In range enemy has reduced health from tower damage");
	int i;
	for(i = 0; i < 9; i++)	{
	fire();
	}
	sput_fail_unless(isDead(1) == 1, "Enemy dead after being fired on 10 times");	
	
}

void parseToTowerTesting()	{

	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testParseToTower():Tower popping parsed commands");
	sput_run_test(testParseToTower);
	sput_leave_suite();
	
	sput_finish_testing();

}

void parseToQueueTesting()	{
	
	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("testValidParses():Testing Valid parsed commands being placed on queue");
	sput_run_test(testValidParses);
	sput_leave_suite();
	
	sput_finish_testing();
}

void testParseToTower()
{
	freeAllTowers();
	clearQueue();	
	createTowerFromPositions(1);
	createTowerFromPositions(2);
	iprint(getNumberOfTowers());
	int originalValue = 10;
	setTowerRange(1,originalValue); //Setting tower range to 10 for tests.
	setTowerDamage(2,originalValue); //Setting tower damage to 10 for tests.
	addMemory(10000);
	printf("#####before Parse\n");
	parse("upgrade r t1");
	parse("upgrade p t2");
	printf("#####here\n");
	sput_fail_unless(getFirstTarget() == 1, "First target is 1");
	sput_fail_unless(getLastTarget() == 2, "Last target is 2");
	delayGame(ACTIONCOOLDOWN);
	popToTower();
	sput_fail_unless(getTowerRange(1) > originalValue, "Upgraded range is greater than original value");
	delayGame(ACTIONCOOLDOWN);
	popToTower();
	sput_fail_unless(getTowerDamage(2) > originalValue, "Upgraded Damage is greater than original Value");
	freeAllTowers();
	clearQueue();
}

void testValidParses()
{
	
	userCreateTower(0, 0); // create tower at x: 0, y: 0. Position is irrelevant for this test
    sput_fail_unless(parse("upgrade r t1")== 1, "upgrade r t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == cmd_upgrade, "First command in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == upgrade_range, "First option in queue: range");
    sput_fail_unless(parse("upgrade p t1")== 1, "upgrade p t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == cmd_upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == upgrade_power, "Last option in queue: power");

    sput_fail_unless(parse("upgrade s t1")== 1, "upgrade s t1 is valid command");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == cmd_upgrade, "Last comand in queue: upgrade");
	sput_fail_unless(getLastOption(getQueue(NULL)) == upgrade_speed, "Last option in queue: speed");
	sput_fail_unless(getLastCommand(getQueue(NULL)) == cmd_upgrade, "First command in queue: upgrade");
    sput_fail_unless(parse("  ??D--") == 0, "  ??D-- is invalid command");
    sput_fail_unless(parse("upgrade r r1") == 0, "upgrade r r1 is invalid command");
    //sput_fail_unless(parse("upgrade r t") == 0, "upgrade r t is invalid command");
    //sput_fail_unless(parse("upgrade t") == 0, "upgrade t is invalid command");
    //sput_fail_unless(parse("cat t") == 0, "cat t is invalid command");
	freeAllTowers();
	clearQueue();
}

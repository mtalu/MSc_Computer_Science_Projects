//
//  Information_Window.c
//  Group_Project
//
//  Functions relating to game windows and displaying information on screen
//
//  Created by Michael on 10/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

/*---------- Standard Headers -----------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------- Custom Headers	-----------*/
#include "../includes/Information_Window.h"
#include "../includes/tower.h"
#include "../includes/gameProperties.h"
#include "../includes/actionQueueDataStructure.h"
#include "../includes/Display.h"
#include "../includes/sput.h"
#include "../includes/parser.h"
#include "../includes/levelController.h"

/*---------- Data Types -----------*/
typedef enum towerMonitorString {TOWER_DEFAULT, TOWER_INFO, OTHER_INFO} TowerMonitorString;
typedef enum terminalWindowString {TERMINAL_DEFAULT, ERROR_MESSAGE} TerminalWindowString;

struct towerMonitor {
    char *string;
    TowerMonitorString stringType;
    int timeSet;
    int targetTower;
};

typedef struct commandNode {
    char *commandString;
    struct commandNode *next;
} CommandNode;

struct terminalWindow {
    CommandNode *start;
    TerminalWindowString stringType;
    int commands;
    char *outputString;
    char *errorString;
    int timeSet;
};

/*---------- Hash Defines -----------*/

#define MAX_OUTPUT_STRING 2000
#define TOTAL_COMMANDS_DISPLAYED 5
#define DEFAULT_TOWER_MONITOR_TIME 10000
#define TERMINAL_ERROR_TIME 5000

/*----------Function Prototypes (Internal)-----------*/
char *towerMonitor(void);
char *terminalWindow(void);
void statsBar(void);
void towerInformation(void);
void actionQueueMonitor(void);
TowerMonitor *getTowerMonitor(void);
char *getDefaultTowerString(TowerMonitor *tm);
char *getTowerString(unsigned int targetTower, TowerMonitor *tm);
TerminalWindow *getTerminalWindow(void);
CommandNode *createCommandNode(void);
void destroyCommandNode(CommandNode **start);

/*----------Function Prototypes (Testing)-----------*/
void testTowerMonitor(void);
void testTerminalWindow(void);
void testParserErrorMessages(void);
void testParserInfoMessages(void);

/**
 Updates everything in information window
 @param void
 @returns void
 */
void updateAllInfoWindow(void) {
    statsBar();
    towerMonitor();
    actionQueueMonitor();
    towerInformation();
    terminalWindow();
}

void tutorialUpdateAllInfoWindow(void) {
    statsBar();
	tutorialTowerMonitor();
    actionQueueMonitor();
    towerInformation();
    terminalWindow();
}

void tutorialTowerMonitor()	{
	TowerMonitor *tm = getTowerMonitor();
	updateTowerMonitor(tm->string);
}

/**
 update tower monitor according to information in tower monitor object
 @param void
 @returns pointer to display string currently held in tower monitor
 */
char* towerMonitor(void) {
    TowerMonitor *tm = getTowerMonitor();
    int time = SDL_GetTicks();
    
    //Set output string accordingly
    switch (tm->stringType) {
        case TOWER_DEFAULT:
            getDefaultTowerString(tm);
            break;
        case TOWER_INFO:
            getTowerString(tm->targetTower, tm);
            break;
        case OTHER_INFO:
            break;

    }
    
    //If another string has been set and a period of time has elapsed, reset to default
    if(tm->stringType != TOWER_DEFAULT && time - tm->timeSet > DEFAULT_TOWER_MONITOR_TIME) {
        tm->stringType = TOWER_DEFAULT;
    }
    
    updateTowerMonitor(tm->string);
    
    return tm->string;
}


/**
send string to tower monitor and display for set period of time
@param string to be displayed
@returns pointer to string to be displayed in tower monitor object
*/
char *textToTowerMonitor(char *string) {
    TowerMonitor *tm = getTowerMonitor();
    
    strcpy(tm->string, string);
    tm->stringType = OTHER_INFO;
    tm->timeSet = SDL_GetTicks();
    
    return tm->string;
}

/**
 alerts tower monitor that tower information has been requested, information for that
 tower will be displayed for set period of time
 @param the tower for which information is to be displayed
 @returns void
 */
void displayTowerInfo(unsigned int targetTower) {
    TowerMonitor *tm = getTowerMonitor();
    
    tm->targetTower = targetTower;
    tm->stringType = TOWER_INFO;
    tm->timeSet = SDL_GetTicks();
}


/**
 initialize tower monitor object when first called, return pointer to object each subsequent call
 @param void
 @returns the Tower Monitor object
 */
TowerMonitor *getTowerMonitor(void) {
    static TowerMonitor *tm;
    static bool initialized = false;
    
    if(!initialized) {
        tm = (TowerMonitor *) malloc(sizeof(TowerMonitor));
        
        tm->string = (char *) calloc(MAX_OUTPUT_STRING,sizeof(char));
        tm->timeSet = 0;
        tm->stringType = TOWER_DEFAULT;
        
        initialized = true;
    }
    
    return tm;
}

/**
 update terminal window according to information in terminal window object
 @param void
 @returns the display string currently stored in terminal window object
 */
char *terminalWindow(void) {
    TerminalWindow *tw = getTerminalWindow();
    int time = SDL_GetTicks();
    
    strcpy(tw->outputString, "");
    
    switch(tw->stringType) {
        case TERMINAL_DEFAULT:
            for(CommandNode *start = tw->start; start != NULL; start = start->next) {
                strcat(tw->outputString, "$ ");
                strcat(tw->outputString, start->commandString);
                strcat(tw->outputString, "\n");
            }
            break;
        case ERROR_MESSAGE:
            strcpy(tw->outputString, tw->errorString);
            
            //Reset to default terminal string after period of time has elapsed
            if(time - tw->timeSet > TERMINAL_ERROR_TIME) {
                tw->stringType = TERMINAL_DEFAULT;
            }
            break;
    }

    
    //Send string to display if not empty
    if(strlen(tw->outputString) > 0) {
        updateTerminalWindow(tw->outputString);
    }
    
    return tw->outputString;
}

/**
 initialize terminal window object when first called, return pointer to object each subsequent call
 @param void
 @returns pointer to the terminal window object
 */
TerminalWindow *getTerminalWindow(void) {
    static TerminalWindow *tw;
    static bool initialized = false;
    
    if(!initialized) {
        tw = (TerminalWindow *) malloc(sizeof(TerminalWindow));
        tw->start = NULL;
        tw->commands = 0;
        tw->outputString = (char *) malloc(sizeof(char) * MAX_OUTPUT_STRING);
        tw->errorString = (char *) malloc(sizeof(char) * MAX_OUTPUT_STRING);
        tw->stringType = TERMINAL_DEFAULT;
        tw->timeSet = 0;
        
        initialized = true;
    }
    
    return tw;
}

/**
 send error to terminal window object
 @param string to be sent to terminal window after formatting
 @returns void
 */
char *errorToTerminalWindow(char *string) {
    TerminalWindow *tw = getTerminalWindow();
    
    static char errorString[MAX_OUTPUT_STRING];
    sprintf(errorString, "******************************\n%s\n******************************", string);
    
    strcpy(tw->errorString, errorString);
    tw->stringType = ERROR_MESSAGE;
    tw->timeSet = SDL_GetTicks();
    
    return errorString;
}

/**
 send command to terminal window object
 @param the command to be added to terminal window
 @returns a pointer to the display string with last N commands stored in terminal window
 */
char *commandToTerminalWindow(char *string) {
    TerminalWindow *tw = getTerminalWindow();
    
    //Create command node and add command string to it
    CommandNode *newNode = createCommandNode();
    strcpy(newNode->commandString, string);
    newNode->next = NULL;
    
    //Find last node in list and add new command node
    if(tw->start == NULL) {
        tw->start = newNode;
    }
    else {
        CommandNode *temp = tw->start;
        
        while(temp->next != NULL) {
            temp = temp->next;
        }
        
        temp->next = newNode;
    }
    
    tw->commands++;
    
    //Destroy first command and relink list if too many commands
    if(tw->commands > TOTAL_COMMANDS_DISPLAYED) {
        destroyCommandNode(&tw->start);
    }
    
    return tw->outputString;
}

/**
 Creates command node that will store command string
 @param void
 @returns pointer to the command node
 */
CommandNode *createCommandNode(void) {
    
    CommandNode *commandNode = (CommandNode*) malloc(sizeof(CommandNode));
    if(commandNode == NULL) {
        fprintf(stderr, "malloc failed in createCommandNode()");
    }
    commandNode->commandString = (char *) malloc(sizeof(char) * MAX_OUTPUT_STRING);
    if(commandNode->commandString == NULL) {
        fprintf(stderr, "malloc failed in createCommandNode()");
    }
    
    return commandNode;
    
}

/**
 Deallocate memory for previously created command node and command string and relink list
 @param pointer to the command node pointer
 @returns void
 */
void destroyCommandNode(CommandNode **start) {
    CommandNode *temp = *start;
    *start = (*start)->next;
    free(temp->commandString);
    free(temp);

}

void destroyCommandList(void) {
    TerminalWindow *tw = getTerminalWindow();
    
    for(CommandNode *start = tw->start; start != NULL;) {
        destroyCommandNode(&start);
    }
    
    tw->start = createCommandNode();
    tw->commands = 0;
    tw->start->commandString = "";
}

/**
 Creates output string for stats bar and updates it
 @param void
 @returns void
 */
void statsBar() {
    
    GameProperties properties = getGame(NULL);
    
    int mem = getAvailableMemory(properties);
    int waveNumber = getWave(properties);
    int totalWaves = getTotalWaves(properties);
    int health = getHealth(properties);
    
    char *outputString = malloc(MAX_OUTPUT_STRING);
    
    sprintf(outputString, "Available Memory: %dbytes                                                 Wave: %d / %d                                                                     Health: %d", mem, waveNumber, totalWaves, health);
    
    updateStatsBar(outputString);
}

/**
 Creates output string for action queue monitor and updates it
 @param void
 @returns void
 */
void actionQueueMonitor() {
    
    char *outputString = getActionQueueString();
    
    updateActionQueueMonitor(outputString);
}

/**
 Creates tower string for every drawn tower and displays it
 @param void
 @returns void
 */
void towerInformation() {
    
    int numOfTowers = getNumberOfTowers();
    
    if(numOfTowers > 0) {
        for(int towerID = 1; towerID <= numOfTowers; ++towerID) {
            char towerString[10];
            sprintf(towerString, "Tower %d", towerID);
            
            int towerX = getTowerX(towerID);
            int towerY = getTowerY(towerID);
            
            updateTowerInformation(towerX, towerY, towerString, towerID);
        }
    }
    
}

/**
 Creates default tower monitor string and stores in tower monitor object
 @param Tower monitor object
 @returns Pointer to the created display string
 */
char *getDefaultTowerString(TowerMonitor *tm) {
    
    static char defaultTowerString[MAX_OUTPUT_STRING];
    
    sprintf(defaultTowerString, "TOWER MONITOR\n\nActive Towers: %d", getNumberOfTowers());
    strcpy(tm->string, defaultTowerString);
    
    return defaultTowerString;
}

/**
 Creates display string for specific tower and stores in tower monitor object
 @param Tower for which to create display string, tower monitor object
 @returns Pointer to the created display string
 */
char *getTowerString(unsigned int targetTower, TowerMonitor *tm) {
    
    static char towerString[MAX_OUTPUT_STRING];
    char type[10];
    
    int towerType, range, damage, speed, AOEpower, AOErange;

    getStats(&towerType, &range, &damage, &speed, &AOEpower, &AOErange, targetTower);
    
    switch(towerType) {
        case INT_TYPE:
            strcpy(type, "INT");
            break;
        case CHAR_TYPE:
            strcpy(type, "CHAR");
            break;
    }


    sprintf(towerString, "TOWER %d\n\nRange: %d Cost to Upgrade: %d \nDamage: %d Cost to Upgrade: %d\nSpeed: %d Cost to Upgrade: %d \nAOE Power: %d\nAOE Range: %d", targetTower, 
					range, calculateCosts(cmd_upgrade,upgrade_range,targetTower), 
					damage, calculateCosts(cmd_upgrade,upgrade_power,targetTower), 
					speed, calculateCosts(cmd_upgrade,upgrade_speed,targetTower),
					AOEpower, AOErange);

    strcpy(tm->string, towerString);
    
    return towerString;
}

void tutorial_one()	{

	textToTowerMonitor("Hi!  Welcome to your first day as a Bash Defender.  We expect you to defend our precious systems against all kinds of computing nasties.\n Lets get started!\n");

}

void tutorial_two()	{

	textToTowerMonitor("lets try making a tower!\n please type:\n mktwr int a\n to make an integer tower in position a.  You can try another position if you like!\n\n This command looks like a lot like linux command 'mkdir' for making directories - but thats not too important right now.\n");

}

void tutorial_three()	{

		textToTowerMonitor("Well done!  You have made a tower of type integer.\n You should use this tower type against integer enemies - those are the ones with numbers.\n\n  In Computer Science, an integer is data type for storing whole numbers.\n");

}
void tutorial_four()	{
		textToTowerMonitor("I know what you are thinking, why would you need to build towers when its so calm and peaceful round here?\n Well, its not always like this.  In your job as a linux administrator, you will be under constant attack from virus's.\n  In fact, it looks like there is one on its way right now!\n");

}

void tutorial_five()	{

	textToTowerMonitor("Oh no!  It looks like that enemy got away.  We need to prepare before the next enemy comes.\n  Upgrade your tower's damage by typing\n upgrade p t1");

}

void tutorial_six()	{

	textToTowerMonitor("Well Done!  Now your tower will do more damage against those pesky virus'\n.  Check it out, here comes another one!\n");

}

void tutorial_five_error()	{

	textToTowerMonitor("Thats not the correct stat to upgrade.\n  Please upgrade power by typing \n upgrade p t1\n");
}

void tutorial_seven()	{

	textToTowerMonitor("Well That was weird.\n Why didn't your upgraded tower kill that enemy?  Its because your tower was the wrong type!  That enemy was a char enemy - You can tell because it was red.  \n\nYou should also note that your health is going down with each enemy to break through your defenses (top right corner!).  Let this get to zero and your lose!");

}

void tutorial_eight()	{

		textToTowerMonitor("Right!  No more losing.  Lets create a char tower with mktwr char c\n  Char is data type in computer science for storing characters");

}

void tutorial_nine()	{

		textToTowerMonitor("Bam!  that char virus didn't stand a chance.  Lets look at some other upgrade commands before we finish up.  \n");

}

void tutorial_ten()	{
	textToTowerMonitor("Try upgrading your tower's speed.  This will increase the speed that you tower fires at enemies!\n  Type upgrade s t1");
}

void tutorial_eleven()	{
	textToTowerMonitor("Now lets upgrade your tower's range.  This allows you towers to hit farther aware enemies.\n type \n upgrade r t1\n You'll be becoming very familiar with this style of command - they are closely modeled on real linux command line utilities.  You have command, an option and a target!  \n");
}

void tutorial_twelve()	{
	textToTowerMonitor("Hey, you are getting pretty good at this!  \n Lets hope you perform this well when faced with a REAL threat...\n  So, we have upgraded your tower quite a bit now, do you want to see what its stats are in some more detail?");
}

void tutorial_thirteen()	{

	textToTowerMonitor("Lets try using the cat command: type cat t1\n This command tells you the current stats of your tower, and how much it will cost you to upgrade each.\n You can use the cat command on a linux machine to check the contents of files too!");
}

void tutorial_fourteen()	{

		textToTowerMonitor("That command doesn't seem to have worked...  Its because you have been using all your memory on upgrades and towers!\n  Check your memory (top left) and check the amount needed to do the upgrade you are attempting by typing\n cat t1\n  In the field, you'd need to kill more enemies to get more memory.  Since this is training, we will give you some for free.\n");

}

void tutorial_fifteen()	{
		textToTowerMonitor("You should think carefully about what commands you try to use.  If you place a very expensive command in your action queue, you will not be able to do anything else until this command gets actioned!\n  There is also a limit on how quickly you can execute command: commands that aren't ready yet will also be placed in the queue.\n");

}

void tutorial_sixteen()	{
	textToTowerMonitor("No, you have to upgrade the tower's range!\n Please type\n upgrade r t1 \n");

}

void tutorial_seventeen()	{
	textToTowerMonitor("Right - listen up because this next bit advice will be key to you keeping your job! \n What are you going to do if you have only int towers and lots of char virus' attack?  You could build another char tower, but that gets expensive...  Why dont you just change the type of your current towers? \n");
}

void tutorial_eighteen()	{
	textToTowerMonitor("Type:\n chmod int t2 to change tower 2 to an int type.  \nType: \n chmod char t1\n to change tower 2 to a char type.\n  On a proper linux system, you can use the chmod command to change properties of files, not towers!: it will change the file permisions.\n");
}

void tutorial_nineteen()	{
	textToTowerMonitor("There is one more thing you need to know about.  You can install new abilities with the apt-get command.  This command installs a new specified ability from a package repository do give you some more fire-power to fight off virus' - Just like in a Linux Environment when you need a new utility!.\n type apt-get install");
}

/*Test functions*/

/**
 test important functions in information window (unit testing)
 @param void
 @returns void
 */
void testingInformationWindowModule()	{
    
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    sput_enter_suite("testTowerMonitor");
    sput_run_test(testTowerMonitor);
    sput_leave_suite();
    
    sput_enter_suite("testTerminalWindow");
    sput_run_test(testTerminalWindow);
    sput_leave_suite();
    
    sput_finish_testing();
}

/**
 test information window functionality called from parser (integration testing)
 @param void
 @returns void
 */
void testParserToInfoWindow(void) {
    
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    sput_enter_suite("testParserErrorMessages");
    sput_run_test(testParserErrorMessages);
    sput_leave_suite();
    
    sput_enter_suite("testParserInfoMessages");
    sput_run_test(testParserInfoMessages);
    sput_leave_suite();
    
    sput_finish_testing();
    
}


/**
 test if strings in tower monitor are being stored correctly
 @param void
 @returns void
 */
void testTowerMonitor(void) {
    TowerMonitor *tm= getTowerMonitor(); //initialize tower monitor
    
    createTowerGroup(); //Create tower group to test retrieving default tower string
    sput_fail_if(strcmp(getDefaultTowerString(tm), tm->string) != 0, "Testing default string");
    
    textToTowerMonitor("This is a test string");
    sput_fail_if(strcmp(tm->string, "This is a test string") != 0, "Testing random string");
    
    userCreateTower(200, 200); //Create random tower to test retrieving specific tower string.
    sput_fail_if(strcmp(getTowerString(1, tm), tm->string) != 0, "Testing specific tower string");
}

/**
 test if strings in terminal window are being stored correctly.
 @param void
 @returns void
 */
void testTerminalWindow(void) {
    TerminalWindow *tw = getTerminalWindow();
    
    sput_fail_if(strcmp(errorToTerminalWindow("This is a test string"), tw->errorString) != 0, "Testing error string");
    
    commandToTerminalWindow("A random command");
    sput_fail_if(strcmp(tw->start->commandString, "A random command") != 0, "Testing sending a command");
    commandToTerminalWindow("Another random command");
    sput_fail_if(strcmp(tw->start->next->commandString, "Another random command") != 0, "Testing sending another command");
}

void testParserErrorMessages(void) {
    TerminalWindow *tw = getTerminalWindow();
    
    //Test parsing empty string and unknown commands
    
    parse("");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse empty string, should send appropriate error message to terminal window");
    strcpy(tw->errorString, ""); //Reset string
    
    parse("unrecognized");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse unrecognized command, should send appropriate error message to terminal window");
     strcpy(tw->errorString, "");
    
    parse("unrecognized unrecognized");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse 2 unrecognized commands, should send appropriate error message to terminal window"); //CURRENTLY FAILS, NEEDS FIXING IN GAME
     strcpy(tw->errorString, "");
    
    parse("unrecognized unrecognized unrecognized");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse 3 unrecognized commands, should send appropriate error message to terminal window"); ////CURRENTLY FAILS, NEEDS FIXING IN GAME
     strcpy(tw->errorString, "");
    
    //Test parsing upgrade command with all combinations
    
    parse("upgrade");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse upgrade with no stats or target, should send appropriate error message to terminal window");
    
     strcpy(tw->errorString, ""); //Reset string
    
    parse("upgrade r");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse upgrade with no target, should send appropriate error message to terminal window");
    
     strcpy(tw->errorString, ""); //Reset string
    
    parse("upgrade r t25");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse upgrade with non-existing target, should send appropriate error message to terminal window");
    
     strcpy(tw->errorString, ""); //Reset string
    
    /*createTowerFromPositions(1);
    parse("upgrade r t1");
    sput_fail_if(strcmp(tw->errorString, "") != 0, "Testing parse upgrade with existing target, should not send any message to terminal window");
    strcpy(tw->errorString, "");*/
    //CAUSES MALLOC ERROR, POSSIBLY BECAUSE TOWER HASN'T BEEN CREATED PROPERLY
    
    //Test parsing mktwr command with all combinations
    
    parse("mktwr");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse mktwr without tower type or targer, should send appropriate error message to terminal window");
    strcpy(tw->errorString, "");
    
    parse("mktwr unrecognized");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse mktwr with unrecognized type and no target, should send appropriate error message to terminal window");
    strcpy(tw->errorString, "");
    
    parse("mktwr INT");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse mktwr with recognized type and no target, should send appropriate error message to terminal window");
    strcpy(tw->errorString, "");
    
    parse("mktwr INT unrecognized");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse mktwr with recognized type and unrecognized target, should send appropriate error message to terminal window");
    strcpy(tw->errorString, "");
    
    /*parse("mktwr INT A");
    printf("\n\nMichael Testing: .%s.\n\n", tw->errorString);
    sput_fail_if(strcmp(tw->errorString, "") != 0, "Testing parse mktwr with recognized type and recognized target, should not send error message to terminal window");
    strcpy(tw->errorString, "");*/
    //DOES SEND ERROR MESSAGE, POSSIBLY BECAUSE TOWER POSITIONS NOT CREATED PROPERLY
    
    //Test parsing man command with all combinations
    
    parse("man");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse man without target, should send appropriate error message to terminal window");
    strcpy(tw->errorString, "");
    
    /*parse("man unrecognized");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse man with unrecognized target, should send appropriate error message to terminal window");
    strcpy(tw->errorString, "");*/
    //NOT WORKING CURRENTLY, NEEDS FIXING IN GAME
    
    parse("man cat");
    sput_fail_if(strcmp(tw->errorString, "") != 0, "Testing parse man with valid target, should not send error message to terminal window");
    strcpy(tw->errorString, "");
    
    //Test parsing cat command with all combinations
    
    parse("cat");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse cat without target, should send appropriate error message to terminal window");
    strcpy(tw->errorString, "");
    
    parse("cat unrecognized");
    sput_fail_if(strcmp(tw->errorString, "******************************\nERROR: Could not execute command. Type man [COMMAND] for help\n******************************") != 0, "Testing parse cat with unrecognized target, should send appropriate error message to terminal window"); //CURRENTLY FAILS, NEEDS FIXING IN GAME
    strcpy(tw->errorString, "");
    
    parse("cat t1");
    sput_fail_if(strcmp(tw->errorString, "") != 0, "Testing parse cat with recognized target, should not send error message to terminal window");
    strcpy(tw->errorString, "");
}

void testParserInfoMessages(void) {
    TowerMonitor *tm = getTowerMonitor();
    
    //Test parsing man command with valid and invalid combinations
    
    parse("man cat");
    sput_fail_if(strcmp(tm->string, "GENERAL COMMANDS MANUAL: \n\ncat \n\ntype ""cat"" followed by a target, e.g. t1, t2, t3..., to display the stats of that target\n") != 0, "Testing parse man with recognized target, should send appropriate info message to tower monitor");
    strcpy(tm->string, ""); //Reset string
    
    /*parse("man unrecognized");
    sput_fail_if(strcmp(tm->string, "") != 0, "Testing parse man with unrecognized target, should not send info message to tower monitor");//CURRENTLY CAUSES SEGFAULT, NEEDS FIXING IN GAME
    strcpy(tm->string, ""); //Reset string*/
    
    //Test parsing cat command with valid and invalid combinations
    
    /*parse("cat t2");
    char towerMonitorString[MAX_OUTPUT_STRING];
    strcpy(towerMonitorString, tm->string);
    sput_fail_if(strcmp(towerMonitorString, getTowerString(1, tm)) != 0, "Testing parse cat with recognized target, should send appropriate info message to tower monitor");
    strcpy(tm->string, "");*/
    //PROBLEMS CREATING TOWERS
    
}

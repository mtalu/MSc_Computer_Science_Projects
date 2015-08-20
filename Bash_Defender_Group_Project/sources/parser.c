//
//  paser.c
//  The parser needs to accept a string from the text input
//
//  Created by ben on 07/11/2014.
//
//



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "../includes/tower.h"
#include "../includes/actionQueueDataStructure.h"
#include "./../includes/parser.h"
#include "../includes/Information_Window.h"
#include "../includes/abilities.h"
#include "../includes/enemy.h"
#include "../includes/sput.h"
#include "../includes/gameProperties.h"
#include "../includes/abilities.h"

#pragma mark ProtoTypes
//top level command parser:
int parseCommands(char ** commandArray, int numberOfTokens);
//specific command parsers:
int parseCat(char * inputStringTargeting);
int parseMan(char * inputStringCommandMan);
int parseAptget(char * aptToGetString);
int parsePs(char * optionString);
int parseAptget(char * aptToGetString);
int parseKill(char ** commandArray,int numberOfChunks);
int parseMktwr(char ** commandArray, int numberOfTokens);
int parseChmod(char ** commandArray,int numberOfTokens);
int parseUpgrade(char ** commandArray, int numberOfChunks);

void cleanUpParseUpgrade(cmdOption * statsToUpgradeArray,int * targetArray);

//generic functions:
cmdType getCommandType(char * firstToken);
cmdOption getCommandOption(char * input);
unsigned int getTargetTower(const char * inputStringTargeting, bool needsIdentifier);
unsigned int getTargetEnemy(const char * inputStringTargeting);
unsigned long int stringToInt(const char * string);
char ** breakUpString(const char * inputString, int *numberOfChunksPtr, const char * delimiter);
char * strdup(const char * s);
void freeCommandArray(char **commandArray,int numberOfChunks);
//default error messaging functions:
void optionUsageError();
void actionUsageError();

typedef struct targetArray {
    int * array;
    int numberOfElements;
} targetArrayStruct;

typedef struct statArray {
    cmdOption * array;
    int numberOfElements;
} statArrayStruct;

typedef struct upgradeArrays {
    statArrayStruct * statArray;
    targetArrayStruct * tarArray;
} upgradeArraysStruct;

typedef enum operator {
    error = -1,
    none = 0,
    not = '!',
    greaterThan = '>',
    lessThan = '<',
    equalTo = '=',
    greaterThanOrEqualTo = '>'+'=',
    lessThanOrEqualTo = '<'+'=',
    sameAs = '='+'=',
    notEqualTo = '!'+'='
} operator;

//While parsing funtions:
int parseWhile(char *inputString);

operator matchesOperator(char isThisAnOperator);
operator combineOperators(operator firstOp, operator secondOp);
operator getOperatorFromString(char * conditionString);
void makeStringForOperator(operator op, char * string);

envVar * returnEnvVar(char * stringToMatch);
int getCommandMemCost(cmdType command, envVar * mem);
int numberOfMktwrLastPushed (int mktwrsPushed);
upgradeArraysStruct * getStatsToUpgradeArrayAndTargetArray(upgradeArraysStruct * upgradeStruct);
int getCostOfMktwr();
int getCostOfUpgrade(upgradeArraysStruct * upgradeStuct);
int isThisInfiniteLoop(envVar * variable, operator op, char ** commandArray);
int updateTowsValue(cmdType command);
int updateMemValue(cmdType command);

//initialiser functions:
stringList * intialiseCommandList();
stringList * intialiseOptionList();
stringList * getCommandList(stringList * commandList);
stringList * getOptionList(stringList * optionList);
envVarList * intialiseEnvVarsList();
envVarList * getEnvsList(envVarList * envsList);

//developement testing functions:
void testStringLists();
void testCommandArray(char ** commandArray, int numberOfChunks);


#pragma mark MainFuntion

/*
 * Parse called with string of user input from terminal window.
 * reads the first token and calls the relevant command function 
 * returns zero if syntax error.
 */
int parse(char *inputString)
{
    commandToTerminalWindow(inputString); //Display input string in terminal window, whether recognized or not
    size_t len = 1+strlen(inputString);//gets the size of inputString
    if( len < 3*sizeof(char) )
    {
        fprintf(stderr,"ERROR: valid commands must be longer than that\n");
        errorToTerminalWindow("ERROR: valid commands must be longer than that");
        return 0;
    }
    if(tolower(inputString[0])=='w' && tolower(inputString[1])=='h' &&
       tolower(inputString[2])=='i' && tolower(inputString[3])=='l' &&
       tolower(inputString[4])=='e')
    {
        return parseWhile(inputString);
    }


    int numberOfTokens;
    char **commandArray = breakUpString(inputString, &numberOfTokens, " ,");
    //testCommandArray(commandArray, numberOfTokens);
    //array of strings, each elem holds a token string from the input command
    int minNumberOfChunks = 2;//as of cat man and upgrade
    if( numberOfTokens < minNumberOfChunks )
    {
        fprintf(stderr,"ERROR: commands must be space seperated, with atleast two words\n");
        errorToTerminalWindow("ERROR: commands must be space seperated, with atleast two words");
        freeCommandArray(commandArray, numberOfTokens);
        return 0;//no valid commands with less than 2 strings or more than 3
    }
    
    int specificReturns = parseCommands(commandArray,numberOfTokens);
    
    freeCommandArray(commandArray, numberOfTokens);
    return specificReturns;//0 for error
}
/*
 *
 */
int parseCommands(char ** commandArray, int numberOfTokens)
{
    //enumerated type cmdType can describe each of the possible commands(see actionQueue.h)
    cmdType command = getCommandType(commandArray[0]);//the first string in the command should contain the action
    
    if(command==cmd_commandError)//if getAction returns commandError then the input is invalid
    {                //Error messaging handled in getCommandType function
        return 0;
    }
    int specificReturns=0;//stores return values of the different functions that execute the commands
    /**** Now we deal with each possible command separately as they all have different syntax ****/
    switch (command)
    {
        case cmd_upgrade:
        {
            if(numberOfTokens<3)
            {
                fprintf(stderr,"ERROR: Upgrade command expected 2 or more arguments\n");
                errorToTerminalWindow("ERROR: Upgrade command expected 2 or more arguments");
                specificReturns = 0;
            }
            else
            {
                specificReturns = parseUpgrade(commandArray, numberOfTokens);
            }
            break;
        }
        case cmd_cat:
        {
            if(numberOfTokens!=2)
            {
                fprintf(stderr,"ERROR: cat command expected 1 argument only \n");
                errorToTerminalWindow("ERROR: cat command expected 1 argument only");
                specificReturns = 0;
            }
            else
            {
                specificReturns = parseCat(commandArray[1]);
            }
            break;
        }
        case cmd_chmod:
        {
            if(numberOfTokens<3)
            {
                fprintf(stderr,"ERROR: chmod command expected 2 or more arguments\n");
                errorToTerminalWindow("ERROR: chmod command expected 2 or more arguments");
                specificReturns = 0;
            }
            specificReturns = parseChmod(commandArray, numberOfTokens);
            break;
        }
        case cmd_man:
        {
            if(numberOfTokens!=2)
            {
                fprintf(stderr,"ERROR: man command expected 1 argument only \n");
                errorToTerminalWindow("ERROR: man command expected 1 argument only");
                specificReturns = 0;
            }
            else
            {
                specificReturns = parseMan(commandArray[1]);
            }
            break;
        }
        case cmd_mktwr:
        {
            if(numberOfTokens<3)
            {
                fprintf(stderr,"ERROR: mktwr command expected 2 or more arguments\n");
                errorToTerminalWindow("ERROR: mktwr command expected 2 or more arguments");
                specificReturns = 0;
            }
            else
            {
                specificReturns = parseMktwr(commandArray,numberOfTokens);
            }
            break;
        }
        case cmd_aptget:
        {
            if(numberOfTokens!=2)
            {
                fprintf(stderr,"ERROR: apt-get command expected 1 argument only \n");
                errorToTerminalWindow("ERROR: apt-get command expected 1 argument only");
                specificReturns = 0;
            }
            else
            {
                specificReturns = parseAptget(commandArray[1]);
            }
            break;
        }
        case cmd_ps:
        {
            if(numberOfTokens!=2)
            {
                fprintf(stderr,"ERROR: ps command expected 1 argument only \n");
                errorToTerminalWindow("ERROR: ps command expected 1 argument only");
                specificReturns = 0;
            }
            else
            {
                specificReturns = parsePs(commandArray[1]);
            }
            break;
        }
        case cmd_kill:
        {
            if(numberOfTokens<2)
            {
                fprintf(stderr,"ERROR: kill command expected atleast 1 argument \n");
                errorToTerminalWindow("ERROR: kill command expected atleast 1 argument ");
                specificReturns = 0;
            }
            specificReturns = parseKill(commandArray, numberOfTokens);
            break;
        }
        case cmd_commandError:
        {
            printf("command was not read\n");
            specificReturns = 0;
            break;
        }
        case cmd_execute:
        default:
            fprintf(stderr,"\n***parsing not implemented yet returning***\n");
    }
    return specificReturns;
}

#pragma mark IndividualCommandParsers

int parseChmod(char ** commandArray,int numberOfTokens)
{
    cmdOption twrType = getCommandOption(commandArray[1]);
    if( !(twrType==mktwr_int || twrType==mktwr_char) )
    {
        optionUsageError();
        fprintf(stderr,"ERROR: chmod expected a type (int, or char) as its last argument\n");
        errorToTerminalWindow("ERROR: chmod expected a type (int, or char) as its last argument");
        return 0;
    }
    
    // get targets
    int atToken = 2;
    int * targetArray = NULL;
    int numberOfTargets = 0;
    while( atToken < numberOfTokens )
    {
        for(int i = 0; commandArray[atToken][i]; i++)
        {
            commandArray[atToken][i] = tolower(commandArray[atToken][i]);
        }

        int target = getTargetTower(commandArray[atToken], false);
        if(target==0)
        {
            fprintf(stderr,"ERROR: chmod expected target tower as first argument \n");
            errorToTerminalWindow("ERROR: chmod expected target tower as first argument");
            cleanUpParseUpgrade(NULL, targetArray);
            return 0;
        }
        else
        {
            ++numberOfTargets;
            int * tmp = realloc(targetArray, numberOfTargets*sizeof(int));
            if(tmp==NULL)
            {
                fprintf(stderr,"realloc error parser.c parseUpgrade() 2\n");
                exit(1);
            }
            targetArray=tmp;
            targetArray[numberOfTargets-1] = target;
        }
        ++atToken;
    }
    if(!numberOfTargets)
    {
        fprintf(stderr,"ERROR: chmod needs atleast 1 target tower \n");
        errorToTerminalWindow("ERROR: chmod needs atleast 1 target tower ");
        cleanUpParseUpgrade(NULL, targetArray);
        return 0;
    }
    

    for(int tarIter=0; tarIter<numberOfTargets; ++tarIter)
    {
        if(twrType==mktwr_int)
        {
            setTowerType(targetArray[tarIter], INT_TYPE);
            char str[100];
            sprintf(str,">>> changed t%d to int <<<",targetArray[tarIter]);
            printf("\n>>> changed t%d to int <<< \n",targetArray[tarIter]);
            textToTowerMonitor(str);
        }
        if(twrType==mktwr_char)
        {
            setTowerType(targetArray[tarIter], CHAR_TYPE);
            char str[100];
            sprintf(str,">>> changed t%d to char <<<",targetArray[tarIter]);
            printf("\n>>> changed t%d to char <<< \n",targetArray[tarIter]);
            textToTowerMonitor(str);
        }
    }
    cleanUpParseUpgrade(NULL,targetArray);

    return 1;
}
/*
 *
 */
int parseKill(char ** commandArray,int numberOfTokens)
{
    cmdOption option = getCommandOption(commandArray[1]);
    
    if(option!=kill_minus9 && option!=kill_all)
    {
        char str[100];
        sprintf(str,"ERROR: Could not read first kill argument ""%s"" expected all or -9 ",
                commandArray[1]);
        errorToTerminalWindow(str);
        return 0;
    }
    if(option==kill_minus9)
    {
        if(numberOfTokens!=3)
        {
            
            errorToTerminalWindow("ERROR: Expected 3rd argument to kill -9 containing a target enemy ");
            return 0;
        }

        else
        {
            int targetEnemyID = getTargetEnemy(commandArray[2]);//pass 3rd token
            kill_ability(targetEnemyID);
            return 1;
        }
    }
    else if(option==kill_all)
    {
        kill_all_ability();
        return 2;
    }
    else
    {
        errorToTerminalWindow("ERROR: invalid argument to kill command, expected all or -9");
    }
 
    return 0;
}
/*
 *
 */
int parsePs(char * optionString)
{
    cmdOption option = getCommandOption(optionString);
    if(option != ps_x)
    {
        optionUsageError();
        return 0;
    }
    else
    {
        psx_ability();
        return 1;
    }
}

/*
 *
 */
int parseAptget(char * aptToGetString)
{
    cmdOption aptToGet = getCommandOption(aptToGetString);
    if(aptToGet!=aptget_kill)
    {
        fprintf(stderr,"\n***app not recognised***\n");
        fprintf(stderr,"type man aptget to see availible apps\n");
        return 0;
    }
    // if(is_valid_unlock(aptToGet))
    {
        
    }
    if(pushToQueue(getQueue(NULL),cmd_aptget,aptToGet,0)>=1)
    {
        printf("pushing tower to queue\n");
        return 1;
    }
    else return 0;
}

int numberOfMktwrLastPushed (int mktwrsPushed)
{
    static int numberOfMktwrsPushedLastPushed = 0;
    if(mktwrsPushed)
    {
        numberOfMktwrsPushedLastPushed = mktwrsPushed;
    }
    return numberOfMktwrsPushedLastPushed;
}
/*
 *  Called when we read mktwr cmd.
 *  gets tower position and pushes to queue
 *  returns 1 if cmd was probably successfully pushed to queue
 *  returns 0 if definately not succesful or if target or stat call failed
 */
int parseMktwr(char ** commandArray, int numberOfTokens)
{
    cmdOption twrType = getCommandOption(commandArray[1]);
    if( !(twrType==mktwr_int || twrType==mktwr_char) )
    {
        optionUsageError();
        errorToTerminalWindow("ERROR: mktwr expected a type (int, or char) as its first argument");
        return 0;
    }
    static int numberOfCommandsPushed = 0;
    
    int token = 2;
    while(token < numberOfTokens)
    {
        if(strlen(commandArray[token])>1)
        {
            char str[100];
            sprintf(str,"ERROR: mktwr expected a target positon A - %c it read %s",maxTowerPositionChar(),commandArray[token]);
            errorToTerminalWindow(str);
            return 0;
        }
        int towerPosition = (int)tolower(commandArray[token][0]) - 'a' + 1;
        if( towerPosition < 1 || towerPosition > maxTowerPosition() )
        {
            char str[100];
            sprintf(str,"ERROR: mktwr expected a target positon A - %c it read %c",maxTowerPositionChar(),commandArray[token][0]);
            errorToTerminalWindow(str);
            return 0;
        }
        if( isTowerPositionAvailable(towerPosition) )
        {
            if(pushToQueue(getQueue(NULL),cmd_mktwr,twrType,towerPosition)>=1)
            {
                ++numberOfCommandsPushed;
                printf("pushing tower %d to queue\n",towerPosition);
            }
        }
        else
        {
            char str[100];
            sprintf(str,"ERROR: postion %c already has a tower built there",commandArray[token][0]);
            errorToTerminalWindow(str);
            return 0;
        }
        ++token;
    }
    return numberOfCommandsPushed;
}

/*  calls man printing functions
 *  returns 1 if ok
 returns 0 if error and prints message
 */
int parseMan(char * inputStringCommandMan)
{
    cmdType commandToMan = getCommandType(inputStringCommandMan);
    switch (commandToMan)
    {
        case cmd_upgrade:
        {
            textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\nupgrade\n\nType ""upgrade"" followed by a stat\n( p, r, s, AOEp, AOEr)\nfollowed by a target tower\ne.g. t1, t2, t3...\nExamples:\nupgrade r t2\nupgrade p t3");
            return 1;
        }
        case cmd_cat:
        {
            textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\ncat \n\ntype ""cat"" followed by a target, e.g. t1, t2, t3..., to display the stats of that target\n");
            return 1;
        }
        case cmd_man:
        {
            textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\nman \n\ntype ""man"" followed by a command, e.g. upgrade or cat, to view the manual\nentry for that command\n");
            return 1;//0 for error
        }
        case cmd_ps:
        {
            textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\nps\n\ntype ""ps"" followed by a command\n ( -x\n ) to discover information about one or more enemies\nExamples:\nps -x\n");
            return 1;//0 for error
        }
        case cmd_kill:
        {
            textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\nps\n\ntype ""kill -9"" followed by a target enemyID (eg 6) or *all*\n to kill one or more enemies\nExamples:\nkill -9 7\n kill -9 all");
            return 1;//0 for error
        }
        case cmd_execute:
        {
            //manExecute();
            return 1;
        }
        case cmd_chmod:
        {
            textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\n chmod \n\n changes the type of a tower to INT or CHAR. \n It accepts a list of target towers as arguments following the command and must have a type INT or CHAR as the final argument. eg chmod t1 2 3 INT, chmod t1,2,3,4,5 char etc..");
            return 1;
        }
        case cmd_mktwr:
        {
            textToTowerMonitor("GENERAL COMMANDS MANUAL: \n\n mktwr\n\n Builds a new tower. Accepts a tower type either int or char as the second argument and one or more position to build on. e.g. mktwr int a b c d    or    mktwr char a,b,d,e,g etc..");
            return 1;
        }
        default:
        {
            char str[100];
            sprintf(str,"ERROR: command to man not recognised. You entered: %s",inputStringCommandMan);
            errorToTerminalWindow(str);
            return 0;
        }
    }
}



/*
 *  Called when we read cat cmd.
 *  gets target and pushes to info window.
 *  returns 1 if cmd was probably successfully pushed.
 *  returns 0 if definately not succesful or if target call failed.
 */
int parseCat(char * inputStringTargeting)
{
    //looks for tower type target:
    if( (inputStringTargeting[0]=='t' || inputStringTargeting[0]=='T') && strlen(inputStringTargeting)>=2)
    {
        unsigned int targetTower = getTargetTower(inputStringTargeting, true);
        if(targetTower)
        {
            displayTowerInfo(targetTower);//function in Information_Window.c
            return 1;
        }
        else
        {
            char str[100];
            sprintf(str,"ERROR: cat expected a target tower as its 2nd argument");
            errorToTerminalWindow(str);
            return 0;
        }
    }
    //can we also cat other things eg enemies?
    //for now
    else
    {
        char str[100];
        sprintf(str,"ERROR: cat expected a target tower as its second argument. Enter t1 tower target tower 1.");
        errorToTerminalWindow(str);
        return 0;
    }
}


                               
                               
                               
                               
                               
#pragma mark parseUpgrade

/*
 *  Called when we read upgrade cmd.
 *  gets stat and target and pushes to queue
 *  returns 1 if cmd was probably successfully pushed to queue
 *  returns 0 if definately not succesful or if target or stat call failed
 */
int parseUpgrade(char ** commandArray, int numberOfChunks)
{
    static cmdOption * statsToUpgradeArray = NULL;
    int numberOfStatsBeingUpgraded = 0;
    int atToken = 1;
    while(atToken < numberOfChunks)
    {
        cmdOption statToUpgrade = getCommandOption(commandArray[atToken]);
        if(statToUpgrade==-1) break;
        iprint(statToUpgrade);
        if(statToUpgrade<=0 || statToUpgrade>6)
        {
            if(tolower(commandArray[atToken][0])=='t' || tolower(commandArray[atToken][0])=='-')
            {
                break;//read a target tower
            }
            else
            {
                //unrecognised stat error
                optionUsageError();
                return 0;
            }
        }

        ++numberOfStatsBeingUpgraded;
        cmdOption * tmp = realloc(statsToUpgradeArray, numberOfStatsBeingUpgraded*sizeof(cmdOption));
        if(tmp==NULL)
        {
            fprintf(stderr,"realloc error parser.c parseUpgrade() 1 \n");
            exit(1);
        }
        statsToUpgradeArray=tmp;
        statsToUpgradeArray[numberOfStatsBeingUpgraded-1] = statToUpgrade;
        
        ++atToken;
    }
           
    if(!numberOfStatsBeingUpgraded)
    {
        //no stats being upgraded error
        cleanUpParseUpgrade(statsToUpgradeArray,NULL);
        return 0;
    }

    //now get targets
    int * targetArray = NULL;
    int numberOfTargets = 0;
    while( atToken < numberOfChunks)
    {
        int target = getTargetTower(commandArray[atToken], false);
        iprint(target);
        if(target==0)
        {
            //bogus target error
            optionUsageError();
            cleanUpParseUpgrade(statsToUpgradeArray, targetArray);
            return 0;
        }
        
        ++numberOfTargets;
        int * tmp = realloc(targetArray, numberOfTargets*sizeof(int));
        if(tmp==NULL) {
            fprintf(stderr,"realloc error parser.c parseUpgrade() 2\n");
            exit(1);
        }
        targetArray=tmp;
        targetArray[numberOfTargets-1] = target;
 
        
        ++atToken;
    }
    if(!numberOfTargets)
    {
        //no targets error
        optionUsageError();
        cleanUpParseUpgrade(statsToUpgradeArray,targetArray);
        return 0;
    }

    for(int statIter=0; statIter<numberOfStatsBeingUpgraded; ++statIter)
    {
        for(int tarIter=0; tarIter<numberOfTargets; ++tarIter)
        {
            if(pushToQueue(getQueue(NULL),cmd_upgrade, statsToUpgradeArray[statIter],
                           targetArray[tarIter])>=1)
            {
                 printf("\n>>> pushed stat = %d tar = %d <<< \n",statsToUpgradeArray[statIter],
                        targetArray[tarIter]);
            }
        }
    }
    
    targetArrayStruct * pushedTargetArray = malloc(sizeof(targetArrayStruct));
    pushedTargetArray->array = targetArray;
    pushedTargetArray->numberOfElements = numberOfTargets;
    
    statArrayStruct * pushedStatArray = malloc(sizeof(statArrayStruct));
    pushedStatArray->array = statsToUpgradeArray;
    pushedStatArray->numberOfElements = numberOfStatsBeingUpgraded;
    
    upgradeArraysStruct * upgradeStruct = malloc(sizeof(upgradeArraysStruct));
    upgradeStruct->statArray = pushedStatArray;
    upgradeStruct->tarArray = pushedTargetArray;

    getStatsToUpgradeArrayAndTargetArray(upgradeStruct);//storeStatsToUpgradeArray and targetArray
    return 1;
}
                               

void cleanUpParseUpgrade(cmdOption * statsToUpgradeArray,int * targetArray)
{
    if(statsToUpgradeArray)
    {
        free(statsToUpgradeArray);
    }
    if(targetArray)
    {
        free(targetArray);
    }
}

upgradeArraysStruct * getStatsToUpgradeArrayAndTargetArray(upgradeArraysStruct * upgradeStruct)
{
    static upgradeArraysStruct * storedUpgradeStuct = NULL;
    if(upgradeStruct!=NULL)
    {
        if(storedUpgradeStuct)
        {
            
            free(storedUpgradeStuct->tarArray);
            free(storedUpgradeStuct->statArray);
            free(storedUpgradeStuct);
        }
        storedUpgradeStuct = upgradeStruct;
    }
    return storedUpgradeStuct;
}
 




#pragma mark parseWhile
/*
 *  while(mem>0){ command }
 */
int parseWhile(char *inputString)
{
    int numberOfBracketsTokens;
    char ** bracketTokenArray = breakUpString(inputString, &numberOfBracketsTokens, "(){}");
    if(numberOfBracketsTokens<3)
    {
        fprintf(stderr,"ERROR: was expecting condition and command e.g. ""while ( condition ) { command }"" \n");
        errorToTerminalWindow("ERROR: was expecting condition and command e.g. ""while ( condition ) { command }"" ");
        return 0;
    }
    else
    {
        int numberOfTokensInCommandArray;
        char ** commandArray = breakUpString( bracketTokenArray[2], &numberOfTokensInCommandArray,
                                             " ," );
        cmdType command = getCommandType(commandArray[0]);
        if(command != cmd_upgrade && command != cmd_mktwr)
        {
            fprintf(stderr,"ERROR: You can only use while loops with an upgrade or mktwr command \n");
            errorToTerminalWindow("ERROR: You can only use while loops with an upgrade or mktwr command ");
            return 0;
        }
        operator op = getOperatorFromString( bracketTokenArray[1] );
        int numberOfOperands=0;
        //envVarList * envsListStruct = getEnvsList(NULL);
        envVar * variable;
        if(op==none || op==not)
        {
            char ** conditionArray = breakUpString(bracketTokenArray[1], &numberOfOperands,
                                                   " ,");
            if(numberOfOperands>1)
            {
                fprintf(stderr,"ERROR: was expecting a single argument for condition with no operator or ! operator \n");
                errorToTerminalWindow("ERROR: was expecting a single argument for condition with no operator or ! operator");
                free(conditionArray);
                
                return 0;
            }
            free(conditionArray);
            variable = returnEnvVar(bracketTokenArray[1]);
            if(!variable)
            {
                char termErrString[100];
                sprintf(termErrString,"ERROR: use of undeclared variable %s in condition statement",
                        bracketTokenArray[1]);
                fprintf(stderr,"%s \n",termErrString);
                errorToTerminalWindow(termErrString);
                return 0;
            }
            variable->value = variable->getValueFunc();
            if(isThisInfiniteLoop(variable,none,commandArray))
            {
                char termErrString[100];
                sprintf(termErrString,"ERROR: there is no way to break this loop");
                fprintf(stderr,"%s \n",termErrString);
                errorToTerminalWindow(termErrString);
                return 1;
            }
            //error testing done
            //now execute
            if(op==none)
            {
//                while(variable->value>0)
//                {
//                    if( parseCommands(commandArray,numberOfTokensInCommandArray) )
//                    {
//                        variable->updateValueFunc(command);
//                        if(command==cmd_mktwr)
//                        {
//                            ++commandArray[2][0];//increments the tower position
//                        }
//
//                    }
//                    else
//                    {
//                        return 0;
//                    }
//                }
                return 0;
            }
            if(op==not)
            {
                return 0;
            }
            
        }
        else
        {
            char stringForOperator[5];
            makeStringForOperator(op, stringForOperator);
            char ** conditionArray = breakUpString(bracketTokenArray[1], &numberOfOperands,
                                                   stringForOperator);
            //testCommandArray(conditionArray,numberOfOperands);
            if( numberOfOperands!=2 )
            {
                char termErrString[100];
                sprintf(termErrString,"ERROR: was expecting two operands to %s in conditional \n",
                        stringForOperator);
                fprintf(stderr,"%s \n",termErrString);
                errorToTerminalWindow(termErrString);
                return 0;
            }
            envVar * variable = returnEnvVar(conditionArray[0]);
            int conditionTokenIs=1;
            if(!variable)
            {
                variable = returnEnvVar(conditionArray[1]);
                if(!variable)
                {
                    char termErrString[100];
                    sprintf(termErrString,"ERROR: use of undeclared variable in condition statement\n");
                    fprintf(stderr,"%s \n",termErrString);
                    errorToTerminalWindow(termErrString);
                    return 0;
                }
                conditionTokenIs=0;
            }
            int condition = stringToInt(conditionArray[conditionTokenIs]);
            if(conditionTokenIs==0)//for now must have var on LHS
            {
                char termErrString[100];
                sprintf(termErrString,"ERROR: please place the variable on the left hand side of the operator.\n");
                fprintf(stderr,"%s \n",termErrString);
                errorToTerminalWindow(termErrString);
                return 0;
            }
            if(isThisInfiniteLoop(variable,op,commandArray))
            {
                char termErrString[100];
                sprintf(termErrString,"ERROR: there is no way to break this loop");
                fprintf(stderr,"%s \n",termErrString);
                errorToTerminalWindow(termErrString);
                return 1;
            }
            if(op==greaterThan)
            {
          
                while(variable->value>condition)
                {
                    if( parseCommands(commandArray,numberOfTokensInCommandArray) )
                    {
                        variable->updateValueFunc(command);
                        if(command==cmd_mktwr)
                        {
                            ++commandArray[2][0];//increments the tower position
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            if(op==greaterThanOrEqualTo)
            {
                while(variable->value>=condition)
                {
                    if( parseCommands(commandArray,numberOfTokensInCommandArray) )
                    {
                        variable->updateValueFunc(command);
                        if(command==cmd_mktwr)
                        {
                            ++commandArray[2][0];//increments the tower position
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            if(op==lessThan)
            {
                while(variable->value<condition)
                {
                    if( parseCommands(commandArray,numberOfTokensInCommandArray) )
                    {
                        variable->updateValueFunc(command);
                        if(command==cmd_mktwr)
                        {
                            ++commandArray[2][0];//increments the tower position
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            if(op==lessThanOrEqualTo)
            {
                while(variable->value<=condition)
                {
                    if( parseCommands(commandArray,numberOfTokensInCommandArray) )
                    {
                        variable->updateValueFunc(command);
                        if(command==cmd_mktwr)
                        {
                            ++commandArray[2][0];//increments the tower position
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
    freeCommandArray(bracketTokenArray,numberOfBracketsTokens);
    return 0;
}

envVar * returnEnvVar(char * stringToMatch)
{
    envVarList * envsListStruct = getEnvsList(NULL);
    for(int i=0; i<envsListStruct->numberOfElements; ++i)
    {
        if(strcmp(stringToMatch,envsListStruct->array[i]->name) ||
           strcmp(stringToMatch,envsListStruct->array[i]->name2))
        {
            return envsListStruct->array[i];
        }
    }
    return 0;
    
}
int updateTowsValue(cmdType command)
{
    envVar * tows = returnEnvVar("tows");
    if(command == cmd_mktwr)
    {
        tows->value += 1;
        return tows->value;
    }
    if(command == cmd_upgrade)
    {
        return tows->value;
    }
    return tows->value;//stops warnings
}

int updateMemValue(cmdType command)
{
    envVar * mem = returnEnvVar("mem");
    if(command == cmd_mktwr)
    {
        mem->value -= getCostOfMktwr();
        iprint(getCostOfMktwr());
        return mem->value;
    }
    if(command == cmd_upgrade)
    {
        iprint(getCostOfUpgrade( getStatsToUpgradeArrayAndTargetArray(NULL) ));
        mem->value -= getCostOfUpgrade( getStatsToUpgradeArrayAndTargetArray(NULL) );
        return mem->value;
    }
    return mem->value;//stops warnings

}
int getCostOfMktwr()
{
    /*int costs=0;
    int numberOfMktwrsPushed = numberOfMktwrLastPushed(0);
    iprint(numberOfMktwrsPushed);
    for(int i = 0; i < numberOfMktwrsPushed; ++i)
    {
        costs += calculateCosts(cmd_mktwr,0,0);
    }*/
    
    return calculateCosts(cmd_mktwr,0,0);
}

int getCostOfUpgrade(upgradeArraysStruct * upgradeStuct)
{
    int costs=0;
    if( upgradeStuct )
    {
        for(int statIter=0; statIter < upgradeStuct->statArray->numberOfElements; ++statIter)
        {
            for(int tarIter=0; tarIter < upgradeStuct->tarArray->numberOfElements; ++tarIter)
            {
                costs += costOfUpgradeFactoringInTheUpgradesOnTheQueue(upgradeStuct->tarArray->array[tarIter],
                                                                       upgradeStuct->statArray->array[statIter]);
            }
        }
    }
    else
    {
        fprintf(stderr," parse while getCommandsCost error exiting\n");
        exit(0);
    }
    return costs;
}



int isThisInfiniteLoop(envVar * variable, operator op, char ** commandArray)
{
    cmdType command = getCommandType(commandArray[0]);
    if(command != cmd_upgrade && command != cmd_mktwr)
    {
        return 1;
    }
    else if( strcmp(variable->name,"mem")==0 && !(op==greaterThanOrEqualTo || op==greaterThan)  )
    {
        return 1;
    }
    else if( strcmp(variable->name,"tows")==0 && (command==cmd_upgrade || !(op==lessThan || op==lessThanOrEqualTo) ) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void makeStringForOperator(operator op, char * string)
{
    switch (op)
    {
        case not:
            string[0] = '!';
            string[1] = '\0';
            
            return;
        case greaterThan:
            string[0] = '>';
            string[1] = '\0';
            return;
        case lessThan:
            string[0] = '<';
            string[1] = '\0';
            return;
        case equalTo:
            string[0] = '=';
            string[1] = '\0';
            return;
        case greaterThanOrEqualTo:
            string[0] = '>';
            string[1] = '=';
            string[2] = '\0';
            return;
        case lessThanOrEqualTo:
            string[0] = '<';
            string[1] = '=';
            string[2] = '\0';
            return;
        case sameAs:
            string[0] = '=';
            string[1] = '=';
            string[2] = '\0';
            return;
        case notEqualTo:
            string[0] = '!';
            string[1] = '=';
            string[2] = '\0';
            return;
        case none:
            string[0] = 'n';//no op just a while var is none zero
            string[1] = 'o';
            string[2] = 'n';
            string[3] = 'e';
            string[4] = '\0';
            return;
        default:
            string[0] = 'e';
            string[1] = 'r';
            string[2] = 'r';
            string[3] = '\0';
            return;
    }
}

operator getOperatorFromString(char * conditionString)
{
    int i=0;
    operator firstOp, secondOp, totalOp;
    while(conditionString[i]!='\0')
    {
        firstOp =  matchesOperator(conditionString[i]);
        if(firstOp)
        {
            secondOp = matchesOperator(conditionString[i+1]);
            if(secondOp=='=')
            {
                totalOp = combineOperators(firstOp,secondOp);
                return totalOp;
            }
            else
            {
                return firstOp;
            }
        }
        else
        {
            ++i;
        }
    }
    return 0;
}


operator combineOperators(operator firstOp, operator secondOp)
{
    int combinedOpInt;
    operator combinedOp;
    if(firstOp && secondOp == '=')
    {
        combinedOpInt = (int)firstOp + (int)secondOp;
        combinedOp = (operator)combinedOpInt;
        return combinedOp;
    }
    else return firstOp;
}



operator matchesOperator(char isThisAnOperator)
{
    switch (isThisAnOperator)
    {
        case not:                           return not;
        case greaterThan:                   return greaterThan;
        case lessThan:                      return lessThan;
        case equalTo:                       return equalTo;
        default:                            return 0;
    }
}




                               
#pragma mark GenericFunctions

                               
/* 
 *  Called on cat and upgrade commands with the target specifying token.
    looks at the 2nd char in the string to find an int 1-9 to be the target.
    Note, wont work for anything > 9, would just see 1.
    Will print its own error message.
    Returns TargetTowerID if sucessful
    Returns 0 if error
 */
unsigned int getTargetTower(const char * inputStringTargeting, bool needsIdentifier)
{
    unsigned int numberOfTowers = getNumberOfTowers();//getNumberOfTowers(); this is func in tower.c
    
    size_t len = strlen(inputStringTargeting);//gets the size of string
    if( len<1  || ( needsIdentifier &&  len<2 ) )
    {
        char str[100];
        sprintf(str,"ERROR: You must target a towers with this command\nTo target a tower enter t followed by a number or list of numbers 1 - %d",numberOfTowers);
        errorToTerminalWindow(str);
        fprintf(stderr,"*** SYNTAX ERROR: You must target a tower with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number 1 - %d \n",numberOfTowers);
        return 0;
    }
    if ( needsIdentifier && !(inputStringTargeting[0]=='t' || inputStringTargeting[0]=='T') )
    {
        errorToTerminalWindow("ERROR: You must target a towers with this command");
        char str[100];
        sprintf(str,"ERROR: You must target a towers with this command\nTo target a tower enter t followed by a number or list of numbers 1 - %d",numberOfTowers);
        errorToTerminalWindow(str);
        fprintf(stderr,"*** ERROR: You must target a towers with this command ***\n");
        fprintf(stderr,"to target a tower enter t followed by a number or list of numbers 1 - %d \n",numberOfTowers);
        return 0;
    }
    unsigned int targetTower = 0;
    if( inputStringTargeting[0]=='t' || inputStringTargeting[0]=='T' )
    {
        targetTower = stringToInt(inputStringTargeting+1);
    }
    else
    {
        targetTower = stringToInt(inputStringTargeting);
    }
    if(targetTower > numberOfTowers || targetTower < 1 )
    {
        char str[100];
        sprintf(str,"ERROR: target tower does not existYou have only %d towers you entered t%d",numberOfTowers,
                targetTower);
        errorToTerminalWindow(str);
        
        fprintf(stderr,"*** ERROR: target tower does not exist ***\n");
        fprintf(stderr,"You have only %d towers you entered t%d\n",
                numberOfTowers,targetTower);
        return 0;
    }
    return targetTower;
}

/*
 *  Called on cat and upgrade commands with the target specifying token.
 */
unsigned int getTargetEnemy(const char * inputStringTargeting)
{
    unsigned int numberOfEnemies = getNumberOfEnemies();// this is func in enemy.c
    
    size_t len = strlen(inputStringTargeting);//gets the size of string
    if( len<(2*sizeof(char)) )
    {
        fprintf(stderr,"ERROR: You must target a enemy with this command to target a tower enter t followed by a number 1 - %d \n",numberOfEnemies);
        char str[100];
        sprintf(str,"ERROR: You must target a enemy with this command to target a tower enter t followed by a number 1 - %d \n",numberOfEnemies);
        errorToTerminalWindow(str);
        return 0;
    }
    if (inputStringTargeting[0]!='e' && inputStringTargeting[0]!='E')
    {
        fprintf(stderr,"ERROR: You must target a enemy with this command to target a tower enter t followed by a number 1 - %d \n",numberOfEnemies);
        char str[100];
        sprintf(str,"ERROR: You must target a enemy with this command to target a tower enter t followed by a number 1 - %d \n",numberOfEnemies);
        errorToTerminalWindow(str);
        return 0;
    }
    
    unsigned int targetEnemyID = (unsigned int) stringToInt(inputStringTargeting+1);
    
    if(targetEnemyID > numberOfEnemies || targetEnemyID<1)
    {
        char str[100];
        sprintf(str,"ERROR: target enemy does not exist there are only %d enemies you entered e%d\n",
                numberOfEnemies,targetEnemyID);
        errorToTerminalWindow(str);
        return 0;
    }
    return targetEnemyID;
}




unsigned long int stringToInt(const char * string)
{
    unsigned long int converted=0;
    size_t length = strlen(string);
    for(int i=0; i<length; ++i)
    {
        converted += (unsigned int)(string[i]-'0') * pow( 10, (length-i-1));
    }
    return converted;
}
/*
 *  Takes the first string of the input command and tests it against the correct
 syntax to find which command they want to execute then returns that command
 as a enum cmdType variable. Returns cmdType correspodning to the
 validated command or a commandError cmdType
 */
cmdType getCommandType(char * firstToken)
{
    for(int i = 0; firstToken[i]; i++) {
        firstToken[i] = tolower(firstToken[i]);
    }
    stringList * commandList = getCommandList(NULL);
    //now test the input string against all valid actions
    cmdType command = cmd_commandError;
    for(int i=1; i<=commandList->numberOfStrings; ++i)
    {
        if(strcmp(firstToken,commandList->stringArray[i])==0)//if the string is identical to one of the commands
        {                                        //then action is set to that command
            switch (i)
            {
                case 1:
                    command = cmd_upgrade;
                    break;
                case 2:
                    //command = cmd_execute; NOT IMPLEMENTED
                    break;
                case 3:
                    command = cmd_chmod;
                    break;
                case 4:
                    command = cmd_man;
                    break;
                case 5:
                    command = cmd_cat;
                    break;
                case 6:
                    command = cmd_mktwr;
                    break;
                case 7:
                    command = cmd_ps;
                    break;
                case 8:
                    command = cmd_aptget;
                    break;
                    
                case 9:
                    command = cmd_kill;
                    break;
            }
            break;
        }
    }
    
    if(command==cmd_commandError)//if it is still set to ERROR then the user made a mistake
    {
        actionUsageError(firstToken);
    }
    return command;
}

/*  Called when after we read a command, tests the next token against the
 *  possible options returns the corresponding cmdOption Or
    returns optionError  and calls the optUsageError function
 */
cmdOption getCommandOption(char * secondToken)
{
    for(int i = 0; secondToken[i]; i++)
    {
        secondToken[i] = tolower(secondToken[i]);
    }
    /*first lets get the array of strings that hold all the possible action commands*/
    stringList * optionList = getOptionList(NULL);
    int numberOfOptions=optionList->numberOfStrings;

    //now test the input string against all valid stats
    cmdOption option = optionError;
    for(int i=1; i<=numberOfOptions; ++i)
    {
        if(strcmp(secondToken,optionList->stringArray[i])==0)//if the string is identical to one of the commands
        {                                        //then action is set to that command
            switch (i)
            {
                case 1:
                    option = upgrade_power;
                    break;
                case 2:
                    option = upgrade_range;
                    break;
                case 3:
                    option = upgrade_speed;
                    break;
                case 4:
                    option = upgrade_AOErange;
                    break;
                case 5:
                    option = upgrade_AOEpower;
                    break;
                case 6:
                    option = upgrade_level;
                    break;
                case 7:
                    option = mktwr_int;
                    break;
                case 8:
                    option = mktwr_char;
                    break;
                case 9:
                    option = ps_x;
                    break;
                case 10:
                    option = kill_minus9;
                    break;
                case 11:
                    option = kill_all;
                    break;
                case 12:
                    option = aptget_kill;
                    break;
            }
            break;
        }
    }
    
    /*if(option==optionError)//if it is still set to ERROR then the user made a mistake
    {
        optionUsageError();
    }*/
    return option;
}




/* 
 *  If there was a syntax error in the users command call this function which
    will print usage advice to the terminal window
 */
void optionUsageError()
{
    errorToTerminalWindow("ERROR: Could not execute command. Type man [COMMAND] for help");
    fprintf(stderr,"*** Syntax error: Could not execute command.***\n");
    fprintf(stderr,"\nType man [COMMAND] for usage\n");//we advise them on usage
    //error messages will need to be passed back to the terminal to be printed. hopefully can do this by setting up a custom stream. For now will print to stderr.
}







/*
 *   If there was a syntax error in the users command call this function which
     will print usage advice to the terminal window.
 */
void actionUsageError(const char * firstToken)
{
    stringList * commandList = getCommandList(NULL);
    fprintf(stderr,"*** ""%s"" command not recognised ***\n",firstToken);
    fprintf(stderr,"installed commands: \n");
    char str[200];
    sprintf(str," ""%s"" command not recognised",firstToken);
    
    int numberOfCommands=commandList->numberOfStrings;

    for(int i=1; i<=numberOfCommands; ++i)
    {
        fprintf(stderr,"%s\n",commandList->stringArray[i]);
    }
    fprintf(stderr,"\nType man [COMMAND] for usage\n");//we advise them on usage
    //error messages will need to be passed back to the terminal to be printed.
    //hopefully can do this by setting up a custom stream. For now will print to stderr.

}

/*
 *  Takes the input string and breaks into separate words (where there is a 
    space and new string starts) each of these words is stored in the 
    commandArray which is an array of strings
 */
char **breakUpString(const char * inputString, int *numberOfChunksPtr, const char * delimiter)
{
    char    *stringChunk,                       //holds the chunks on the input string as we break it up
            *inputStringDuplicate = strdup(inputString),//duplicate input string for editting
            **commandArray = NULL;              //this will be an array to hold each of the chunk strings
    int     numberOfChunks=0;
    
    //using http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    stringChunk = strtok(inputStringDuplicate, delimiter); // gets the first chunk (up to the first space)
    
    // walk through rest of string
    while( stringChunk != NULL )
    {
        ++numberOfChunks;
        commandArray=(char **)realloc(commandArray,numberOfChunks*sizeof(char*));//array of strings
        commandArray[numberOfChunks-1]=(char *)malloc((size_t)(strlen(stringChunk)*sizeof(char)+1));
        strcpy(commandArray[numberOfChunks-1],stringChunk);
        stringChunk = strtok(NULL, delimiter);
    }
    free(inputStringDuplicate);//frees the malloc made in strdup()
                               //$(numberOfChunks) strings now stored in the commandArray
    *numberOfChunksPtr=numberOfChunks;
    return commandArray;
}

/*
 *  Duplicates a string
 */
char *strdup(const char * s)
{
    size_t len = 1+strlen(s);//gets the size of s
    char *p = malloc(len);//allocates a block big enough to hold s
    
    return p ? memcpy(p, s, len) : NULL;//if p is non 0 ie malloc worked, then copy everything in s into p and return p. if p is NULL malloc didnt work so return NULL
}

/*
 *  frees the memory allocated in breakup string funct
 */
void freeCommandArray(char **commandArray,int numberOfChunks)
{
    for(int i=0; i<numberOfChunks; ++i)
    {
        free(commandArray[i]);
    }
    //free(commandArray);
}


#pragma mark CommandLists

void initialiseParser()
{
    stringList * commandList = intialiseCommandList();
    getCommandList(commandList);
    stringList * optionList = intialiseOptionList();
    getOptionList(optionList);
    envVarList * envsListStruct = intialiseEnvVarsList();
    getEnvsList(envsListStruct);

}



stringList * intialiseCommandList()
{
    /* make an array of strings to hold all the possible action commands*/
    char **validActions;
    int numberOfActions=9;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validActions=malloc((numberOfActions)*sizeof(char*));//array of $[numberOfActions] strings
    validActions-=1;
    validActions[1]=strdup("upgrade");
    validActions[2]=strdup("execute");
    validActions[3]=strdup("chmod");
    validActions[4]=strdup("man");
    validActions[5]=strdup("cat");
    validActions[6]=strdup("mktwr");
    validActions[7]=strdup("ps");
    validActions[8]=strdup("apt-get");
    validActions[9]=strdup("kill");


    stringList * commandList = malloc(sizeof(stringList));
    commandList->stringArray=validActions;
    commandList->numberOfStrings=numberOfActions;
    
    return commandList;
}
stringList * getCommandList(stringList * commandList)
{
    static stringList * storedCommandList = NULL;
    if(commandList != NULL && storedCommandList == NULL ) {
        storedCommandList = commandList;
    }
    return storedCommandList;
}
stringList * intialiseOptionList()
{
    /*first lets make an array of strings to hold all the possible action commands*/
    char **validOptions;
    int numberOfOptions=12;//have 5 action commands at this time: upgrade, execute, set, man, cat
    validOptions=malloc((numberOfOptions)*sizeof(char*));    //upgrade opts
    validOptions-=1;
    validOptions[1]=strdup("p");
    validOptions[2]=strdup("r");
    validOptions[3]=strdup("s");
    validOptions[4]=strdup("aoer");
    validOptions[5]=strdup("aoep");
    validOptions[6]=strdup("lvl");
    //mktwr opts:
    validOptions[7]=strdup("int");
    validOptions[8]=strdup("char");
    //ps opts:
    validOptions[9]=strdup("x");
    //kill opts:
    validOptions[10]=strdup("-9");
    validOptions[11]=strdup("all");
    //aptget opts:
    validOptions[12]=strdup("kill");
    
    stringList * optionsList = malloc(sizeof(stringList));
    optionsList->stringArray=validOptions;
    optionsList->numberOfStrings=numberOfOptions;
    
    return optionsList;
}

stringList *  getOptionList(stringList * optionList)
{
    static stringList * storedOptionList = NULL;
    if(optionList != NULL && storedOptionList == NULL ) {
        storedOptionList = optionList;
    }
    return storedOptionList;
}
void freeParseLists()
{
    stringList * commandList = getCommandList(NULL);
    if(commandList)
    {
        for(int i=1; i<=commandList->numberOfStrings; ++i)
        {
            free(commandList->stringArray[i]);
        }
        free(commandList->stringArray+1);
        free(commandList);
    }
    stringList * optsList = getOptionList(NULL);
    if(optsList)
    {
        for(int i=1; i<=optsList->numberOfStrings; ++i)
        {
            free(optsList->stringArray[i]);
        }
        free(optsList->stringArray+1);
        free(optsList);
    }
}

#pragma mark EnvironmentVariables
envVarList * intialiseEnvVarsList()
{
    envVarList * envsListStruct = malloc(sizeof(envVarList));
    envsListStruct->numberOfElements = 2;
    envsListStruct->array = malloc(envsListStruct->numberOfElements*sizeof(envVar *));
    
    envsListStruct->array[0] = malloc(sizeof(envVar));
    envsListStruct->array[0]->name = strdup("memory");
    envsListStruct->array[0]->name2 = strdup("mem");
    envsListStruct->array[0]->getValueFunc = &getAvailableMemory;
    envsListStruct->array[0]->updateValueFunc = &updateMemValue;
    envsListStruct->array[0]->value = getAvailableMemory();
    
    envsListStruct->array[1] =  malloc(sizeof(envVar));
    envsListStruct->array[1]->name = strdup("tows");
    envsListStruct->array[1]->name2 = strdup("towers");
    envsListStruct->array[1]->getValueFunc = (int (*)())&getNumberOfTowers;
    envsListStruct->array[1]->updateValueFunc = &updateTowsValue;
    envsListStruct->array[1]->value = getNumberOfTowers();
    
    return envsListStruct;
}


void destroyEnvVarList()
{
    envVarList * envsListStruct = getEnvsList(NULL);
    while(envsListStruct->numberOfElements > 0)
    {
        free(envsListStruct->array[envsListStruct->numberOfElements-1]->name);
        free(envsListStruct->array[envsListStruct->numberOfElements-1]->name2);
        free(envsListStruct->array[envsListStruct->numberOfElements-1]);
        envsListStruct->numberOfElements -= 1;
    }
    free(envsListStruct->array);
    free(envsListStruct);
}

envVarList * getEnvsList(envVarList * envsList)
{
    static envVarList * storedEnvsList = NULL;
    if(envsList != NULL && storedEnvsList == NULL )
    {
        storedEnvsList = envsList;
    }
    return storedEnvsList;
}

#pragma mark DevelopementTests

void testStringLists()
{
    stringList * cmdList = getCommandList(NULL);
    testCommandArray(cmdList->stringArray,cmdList->numberOfStrings);
    stringList * optList = getOptionList(NULL);
    testCommandArray(optList->stringArray,optList->numberOfStrings);
}

/*
 *  Test function for developement. Prints contents of a commandArray
 */
void testCommandArray(char ** commandArray, int numberOfChunks)
{
    for(int i=0; i<numberOfChunks; ++i)
    {
        printf("%s",commandArray[i]);
    }
}


#pragma mark UnitTests

void testParser()
{
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    sput_enter_suite("testReadLevelSettingsFile(): Testing reading and processing level keywords");
    //sput_run_test(test);
    sput_leave_suite();
    
    
    sput_finish_testing();
}

void testInitialiseParseLists()
{
    
}



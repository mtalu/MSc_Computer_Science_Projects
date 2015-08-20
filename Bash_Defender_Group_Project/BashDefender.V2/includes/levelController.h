#ifndef levelController_h
#define levelController_h
/*---------- Custom Headers -----------*/
#include "../includes/parser.h"
#include "../includes/tower.h"
#include "../includes/gameProperties.h"
#include "../includes/enemy.h"
#include "../includes/actionQueueDataStructure.h"
#include "../includes/abilities.h"

#define ENDOFSTRING '\0'
#define READCHAR	'/'
#define TESTLEVEL	"../data/testingLevel.txt" //!holds test data for level settings
#define TUTORIAL_LEVEL	"../data/tutorial.txt" //!Tutorial Level Settings
#define EASY_LEVEL	"../data/easyLevel.txt"	//!Easy Level Settings
#define HARD_LEVEL	"../data/hardLevel.txt" //!Hard Level Settings

typedef enum levelCommand { makeTowerP= 0, 
							totalWaves = 1, 
							wave = 2,
							makeEnemy = 3,
							delay = 4,
							path = 5
} levelCommand;

typedef enum property {	x = 0, 
						y = 1, 
						total = 2, 
						waveID = 3, 
						enemyType = 4,
					    numberOfEnemies = 5,	
						dTime = 6, 
						entrance = 7,
						enemyLevel = 8,
					  	pathLevel = 9,
						numberOfPaths = 10	
} property;

typedef struct keyword *Keyword;
typedef struct keywordProp *KeywordProp;
typedef struct keywordQueue *KeywordQueue;
typedef struct parseLineArray *ParseLineArray;

/*---------- Functions ----------*/
void createKeywordQueue();
KeywordQueue getKWQueue(KeywordQueue kwQueue);
void initLevel(int level);
void endLevel(int *restart);
void addKeyWordToken(char *token);
void readLevelSettingsFile(char *file);
int checkProperty(char *token);
int checkForSemiColon(char *token);
void addKWtoQueue(Keyword nwKW);
char* expandCBuffer(char *toExpand, int currSize);
char* getToken(char *line);
int validateLine(char *Line, int nWords);
int createEnemyCommand(Keyword makeEnemy);
void addProperty(property p);
void pathCommand(Keyword pathCommand);
void addValue(char *token);
void createLevel();
void printQueue();
Keyword createKeyword();
int breakDownWaveCommand(KeywordProp *propertiesList, int nProps);
void makeTowerCommand(Keyword setTower);
int returnPropertyValue(Keyword current, property reqProperty);
void createLevelClocks();
void setWaveTotalCommand(Keyword setWaveTotal);
int waveCreatorCommand(Keyword waveKeyWord);
Keyword removeLink(Keyword current);
void initialQueueReader();
void levelQueueReader();
int addGroupCreationDelay(Keyword waveKW);
int countKeywords();
int returnPropertyValueFromQueue(int place,property reqProperty);
Keyword getKeywordFromQueue(int place);
KeywordProp* returnPropertiesListFromQueue(int place);
int getNumberOfPropertiesFromQueue(int place);
levelCommand getKeywordTypeFromQueue(int place);
void addRawDelay(int delay);
/*---------- Test Functions ----------*/
void setUpTesting();
void testLevelController();
void testReadLevelSettingsFile();

#endif

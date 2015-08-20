#define ETYPE_MOD	4	//!Used to generate random enemy type and level
#define RND_MAX_LEVEL 2	//! Max level for a random virus

typedef enum tutPhase {

	phaseOne = 1,
	phaseTwo = 2,
	phaseThree = 3,
	phaseFour = 4,
	phaseFive =5,
	phaseSix = 6,
	phaseSeven = 7,
	phaseEight = 8,
	phaseNine = 9,
	phaseTen = 10,
	phaseEleven = 11,
	phaseTwelve = 12,
	phaseThirteen = 13,
	phaseFourteen = 14,
	phaseFifteen = 15,
	phaseSixteen = 16,
	phaseSeventeen = 17,
	phaseEighteen = 18,
	phaseNineteen = 19,
	phaseTwenty = 20,
	phaseTwentyOne = 21,
	phaseTwentyTwo = 22,
	phaseTwentyThree = 23,
	phaseTwentyFour = 24,
	phaseTwentyFive = 25,
	phaseTwentySix = 26,
	phaseTwentySeven = 27,
	phaseTwentyEight = 28,
	phaseTwentyNine = 29
} tutPhase;

void quitGame();
void startLevel(Display d, int *restart);
void testing();
void tutorialLevel(Display d,int *restart, tutPhase sPhase);
/*----------System Test Functions-----------*/

void queueToTowerTesting();
void parseToQueueTesting();
void parseToTowerTesting();
void towerToEnemyTesting();
void enemyToGamePropertiesTesting();

/*----------Testing Suites-----------*/

void testPopFromQueue();
void testEnemyDeath();
void testParseToTower();
void testValidParses();
void testEnemyInRange();
void testkillall();


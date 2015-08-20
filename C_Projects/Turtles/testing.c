//
//  testing.c
//  turtles
//
//  Created by Michael on 19/01/2015.
//  Copyright (c) 2015 Michael. All rights reserved.
//

/*---------- Custom Headers	-----------*/
#include "testing.h"
#include "turtles.h"
#include "sput.h"

/*---------- Function Prototypes (Internal) -----------*/
void testParseFD(void);
void testParseRT(void);
void testParseLT(void);
void testExecuteRT(void);
void testExecuteLT(void);
void testParserIllegalCommands(void);
void testParserLegalCommands(void);
int testTurtleScript(char *turtleScript);

/**
 Start testing all test suites using Sput test harness (in sput.h)
 */
void beginTesting(void) {
    
    sput_start_testing();
    sput_set_output_stream(NULL);
    
    /*Unit Tests*/

    sput_enter_suite("Test parsing FD command with valid and invalid combinations");
    sput_run_test(testParseFD);
    sput_leave_suite();
    
    sput_enter_suite("Test parsing RT command with valid and invalid combinations");
    sput_run_test(testParseRT);
    sput_leave_suite();
    
    sput_enter_suite("Test parsing LT command with valid and invalid combinations");
    sput_run_test(testParseLT);
    sput_leave_suite();
    
    sput_enter_suite("Test executing RT Command");
    sput_run_test(testExecuteRT);
    sput_leave_suite();
    
    sput_enter_suite("Test executing LT Command");
    sput_run_test(testExecuteLT);
    sput_leave_suite();
    
    /*Integration Tests*/
    
    sput_enter_suite("Test Illegal Commands");
    sput_run_test(testParserIllegalCommands);
    sput_leave_suite();
    
    sput_enter_suite("Test Legal Commands");
    sput_run_test(testParserLegalCommands);
    sput_leave_suite();
    
    sput_finish_testing();
    
}

/*Unit Tests*/

/**
 Test parsing FD command with various number and variable combinations
 */
void testParseFD(void) {
    //Test parsing FD command with valid numbers (i.e. all numbers)
    sput_fail_unless(parseFD("20") == 0, "Testing FD command with valid number 20, should return 0");
    sput_fail_unless(parseFD("20.5") == 0, "Testing FD command with valid number 20.5, should return 0");
    sput_fail_unless(parseFD("-20") == 0, "Testing FD command with valid number -20, should return 0");
    sput_fail_unless(parseFD("-20.5") == 0, "Testing FD command with valid number -20.5, should return 0");
    sput_fail_unless(parseFD("0") == 0, "Testing FD command with valid number 0, should return 0");

    //Test parsing FD command with valid variable (i.e. variable that has been set)
    getVarlist();
    
    addToVarList('A', 20);
    sput_fail_unless(parseFD("A") == 0, "Testing FD command with valid variable A, should return 0");
    addToVarList('B', 20);
    sput_fail_unless(parseFD("B") == 0, "Testing FD command with valid variable B, should return 0");
    
    //Test parsing FD command with invalid variable (i.e. variable that has not been set)
    sput_fail_unless(parseFD("C") == -1, "Testing FD command with invalid variable C, should return -1");
    sput_fail_unless(parseFD("D") == -1, "Testing FD command with invalid variable D, should return -1");

}

/**
 Test parsing RT command with various number and variable combinations
 */
void testParseRT(void) {
    //Test parsing RT command with valid numbers (i.e. all numbers)
    sput_fail_unless(parseRT("20") == 0, "Testing RT command with valid number 20, should return 0");
    sput_fail_unless(parseRT("20.5") == 0, "Testing RT command with valid number 20.5, should return 0");
    sput_fail_unless(parseRT("-20") == 0, "Testing RT command with valid number -20, should return 0");
    sput_fail_unless(parseRT("-20.5") == 0, "Testing RT command with valid number -20.5, should return 0");
    sput_fail_unless(parseRT("0") == 0, "Testing RT command with valid number 0, should return 0");
    
    //Test parsing RT command with valid variable (i.e. variable that has been set)
    sput_fail_unless(parseRT("A") == 0, "Testing RT command with valid variable A, should return 0");
    sput_fail_unless(parseRT("B") == 0, "Testing RT command with valid variable B, should return 0");
    
    //Test parsing RT command with invalid variable (i.e. variable that has not been set)
    sput_fail_unless(parseRT("C") == -1, "Testing RT command with invalid variable C, should return -1");
    sput_fail_unless(parseRT("D") == -1, "Testing RT command with invalid variable D, should return -1");
}

/**
 Test parsing LT command with various number and variable combinations
 */
void testParseLT(void) {
    //Test parsing LT command with valid numbers (i.e. all numbers)
    sput_fail_unless(parseLT("20") == 0, "Testing LT command with valid number 20, should return 0");
    sput_fail_unless(parseLT("20.5") == 0, "Testing LT command with valid number 20.5, should return 0");
    sput_fail_unless(parseLT("-20") == 0, "Testing LT command with valid number -20, should return 0");
    sput_fail_unless(parseLT("-20.5") == 0, "Testing LT command with valid number -20.5, should return 0");
    sput_fail_unless(parseLT("0") == 0, "Testing LT command with valid number 0, should return 0");
    
    //Test parsing LT command with valid variable (i.e. variable that has been set)
    sput_fail_unless(parseLT("A") == 0, "Testing LT command with valid variable A, should return 0");
    sput_fail_unless(parseLT("B") == 0, "Testing LT command with valid variable B, should return 0");
    
    //Test parsing RT command with invalid variable (i.e. variable that has not been set)
    sput_fail_unless(parseLT("C") == -1, "Testing LT command with invalid variable A, should return -1");
    sput_fail_unless(parseLT("D") == -1, "Testing LT command with invalid variable Z, should return -1");
}

/**
 Test execute RT command against expected new turtle angle
 */
void testExecuteRT(void) {
    initializeTurtle();
    
    sput_fail_unless(executeRT("40") == 40, "Testing turning right 40 degrees, turtle angle should be 40");
    sput_fail_unless(executeRT("360") == 40, "Testing turning right 360 degrees, turtle angle should still be 40");
    sput_fail_unless(executeRT("720") == 40, "Testing turning right 720 degrees, turtle angle should still be 40");
    sput_fail_unless(executeRT("1080") == 40, "Testing turning right 1080 degrees, turtle angle should still be 40");
    sput_fail_unless(executeRT("-20") == 20, "Testing turning right -20 degrees, turtle angle should still be 20");
    sput_fail_unless(executeRT("-360") == 20, "Testing turning right -360 degrees, turtle angle should still be 20");
    sput_fail_unless(executeRT("-720") == 20, "Testing turning right -720 degrees, turtle angle should still be 20");
    sput_fail_unless(executeRT("-1080") == 20, "Testing turning right -1080 degrees, turtle angle should still be 20");
    
    freeTurtle();
}

/**
 Test execute LT command agains expected new turtle angle
 */
void testExecuteLT(void) {
    initializeTurtle();
    
    sput_fail_unless(executeLT("40") == 320, "Testing turning left 40 degrees, turtle angle should be 320");
    sput_fail_unless(executeLT("360") == 320, "Testing turning left 360 degrees, turtle angle should still be 320");
    sput_fail_unless(executeLT("720") == 320, "Testing turning left 720 degrees, turtle angle should still be 320");
    sput_fail_unless(executeLT("1080") == 320, "Testing turning left 1080 degrees, turtle angle should still be 320");
    sput_fail_unless(executeLT("-20") == 340, "Testing turning left -20 degrees, turtle angle should now be 340");
    sput_fail_unless(executeLT("-360") == 340, "Testing turning left -360 degrees, turtle angle should still be 340");
    sput_fail_unless(executeLT("-720") == 340, "Testing turning left -720 degrees, turtle angle should still be 340");
    sput_fail_unless(executeLT("-1080") == 340, "Testing turning left -1080 degrees, turtle angle should still be 340");
    
    freeTurtle();
}

/*Integration tests*/

/**
 Check that all illegal commands are sending error code
 */
void testParserIllegalCommands(void) {
    sput_fail_unless(testTurtleScript("") == -1, "Testing empty turtle file, should return error");
    sput_fail_unless(testTurtleScript("{") == -1, "Testing missing final brace, should return error");
    sput_fail_unless(testTurtleScript("}") == -1, "Testing missing initial brace, should return error");
    sput_fail_unless(testTurtleScript("{ FD }") == -1, "Testing FD command with no argument, should return error");
    sput_fail_unless(testTurtleScript("{ FD Z }") == -1, "Testing FD command with invalid argument, should return error");
    sput_fail_unless(testTurtleScript("{ LT }") == -1, "Testing LT command with no argument, should return error");
    sput_fail_unless(testTurtleScript("{ LT Z }") == -1, "Testing LT command with invalid argument, should return error");
    sput_fail_unless(testTurtleScript("{ RT }") == -1, "Testing RT command with no argument, should return error");
    sput_fail_unless(testTurtleScript("{ RT Z }") == -1, "Testing RT command with no argument, should return error");
    sput_fail_unless(testTurtleScript("{ SET }") == -1, "Testing SET command with no arguments, should return error");
    sput_fail_unless(testTurtleScript("{ SET a }") == -1, "Testing SET command with invalid variable, should return error");
    sput_fail_unless(testTurtleScript("{ SET A }") == -1, "Testing SET command with valid variable but no assignment operator, should return error");
    sput_fail_unless(testTurtleScript("{ SET A :=  }") == -1, "Testing SET command with valid variable and assignment operator but nothing after it, should return error");
    sput_fail_unless(testTurtleScript("{ SET A := unknown }") == -1, "Testing SET command with valid variable and assignment operator, but invalid number or variable after it, should return error");
    sput_fail_unless(testTurtleScript("{ SET A := 13.5 }") == -1, "Testing SET command with valid variable, assignment operator and number, but missing semi-colon, should return error");
    sput_fail_unless(testTurtleScript("{ SET A := 13.5 * ; }") == -1, "Testing SET command with valid variable, assignment operator and number and missing semi-colon, but illegal RPN expression, should return error");
    sput_fail_unless(testTurtleScript("{ SET A := 13.5 3 * + ; }") == -1, "Testing SET command with another illegal RPN expression, should return error");
    sput_fail_unless(testTurtleScript("{ SET A := 13.5 A * + ; }") == -1, "Testing SET command with invalid variables in RPN expression, should return error");
     sput_fail_unless(testTurtleScript("{ DO }") == -1, "Testing DO command with no arguments, should return error");
    sput_fail_unless(testTurtleScript("{ DO unknown }") == -1, "Testing DO command with invalid variable, should return error");
    sput_fail_unless(testTurtleScript("{ DO A }") == -1, "Testing DO command with valid variable but nothing after, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM }") == -1, "Testing DO command with valid variable and FROM keyword, but nothing after, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM unrecognized }") == -1, "Testing DO command with valid variable and FROM keyword, but unrecognized symbol after, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 50.8 }") == -1, "Testing DO command with valid variable and FROM keyword, but invalid number after (i.e. non-integer), should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 }") == -1, "Testing DO command with valid variable, FROM keyword and valid integer but nothing after, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO}") == -1, "Testing DO command with valid variable, FROM keyword, valid integer and TO keyword but nothing after, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO unrecognized}") == -1, "Testing DO command with valid variable, FROM keyword, valid integer and TO keyword, but unrecognized command after, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO 10.5}") == -1, "Testing DO command with valid variable, FROM keyword, integer and TO keyword, but invalid integer after, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO 10 }") == -1, "Testing DO command with valid variable, FROM keyword, valid integer, TO keyword, valid integer, but missing initial brace, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO 10 { }") == -1, "Testing DO command with valid variable, FROM keyword, valid integer, TO keyword, valid integer and initial brace, but no final brace, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO 10 { unrecognized } }") == -1, "Testing valid do loop with unrecognized command, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO 10 { FD } }") == -1, "Testing valid do loop with recognized command but no argument, should return error");
     sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO 10 { FD unrecognized } }") == -1, "Testing valid do loop with recognized command but unrecognized argument, should return error");
     sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO 10  FD 20 } }") == -1, "Testing invalid do loop (missing initial brace) with legal command, should return error");
    sput_fail_unless(testTurtleScript("{ DO A FROM 5 TO 10 { FD 20  }") == -1, "Testing invalid do loop (missing final brace) with legal command, should return error");
}

/**
 Check that all legal commands are NOT sending error code
 */
void testParserLegalCommands(void) {
    sput_fail_if(testTurtleScript("{ }") == -1, "Testing empty pair of braces, script still legal so should not return error");
    sput_fail_if(testTurtleScript("{ FD 20 }") == -1, "Testing FD command with valid number, should not return error");
    sput_fail_if(testTurtleScript("{ SET A := 10 ; FD A }") == -1, "Testing FD command with valid variable, should not return error");
    sput_fail_if(testTurtleScript("{ RT 20.6 }") == -1, "Testing RT command with valid number, should not return error");
    sput_fail_if(testTurtleScript("{ SET A := 10 ; RT A }") == -1, "Testing RT command with valid variable, should not return error");
    sput_fail_if(testTurtleScript("{ LT 20.879 }") == -1, "Testing LT command with valid number, should not return error");
    sput_fail_if(testTurtleScript("{ SET A := 10 ; LT A }") == -1, "Testing LT command with valid variable, should not return error");
    sput_fail_if(testTurtleScript("{ SET A := 10 ; }") == -1, "Testing SET command with valid number, should not return error");
    sput_fail_if(testTurtleScript("{ SET A := 10 ; SET B := A ; }") == -1, "Testing SET command with valid variable after assignment operator instead of number, should not return error");
    sput_fail_if(testTurtleScript("{ SET A := 10 20 * ; }") == -1, "Testing SET command with valid RPN expression, should not return error");
    sput_fail_if(testTurtleScript("{ SET A := 10 20 * 30 + ; }") == -1, "Testing SET command with another valid RPN expression, should not return error");
    sput_fail_if(testTurtleScript("{ SET A := 10 20 30 * + ; }") == -1, "Testing SET command with another valid RPN expression, should not return error");
    sput_fail_if(testTurtleScript("{ SET B := 20 ; SET A := B 20 * 30 + ; }") == -1, "Testing SET command with another valid RPN expression using variable instead of number, should not return error");
    sput_fail_if(testTurtleScript("{ DO A FROM 1 TO 5 { } }") == -1, "Testing DO command with empty loop body, should not return error as still legal");
    sput_fail_if(testTurtleScript("{ DO A FROM 1 TO 5 { FD 20 } }") == -1, "Testing DO command with FD command in loop body using number, should not return error as still legal");
    sput_fail_if(testTurtleScript("{ DO A FROM 1 TO 5 { FD A } }") == -1, "Testing DO command with FD command in loop body using loop variable, should not return error");
    sput_fail_if(testTurtleScript("{ SET B := 50 ; DO A FROM 1 TO 5 { FD B } }") == -1, "Testing DO command with FD command in loop body using variable assigned before loop, should not return error");
    sput_fail_if(testTurtleScript("{ DO A FROM 1 TO 5 { SET B := 50 ; FD B } }") == -1, "Testing DO command with FD command in loop body using variable assigned inside loop body, should not return error");

    

}

/**
 Write turtleScript string to temporary file and reset file position, then tokenize and parse
 */
int testTurtleScript(char *turtleScript) {
    int errorMessage = 0;
    int numberOfTokens = 0;
    FILE *testFile = tmpfile();
    
    if(testFile == NULL) {
        fprintf(stderr, "Error, could not create temporary file\n");
        exit(EXIT_FAILURE);
    }
    
    fputs(turtleScript, testFile);
    rewind(testFile);
    
    char **tokens = tokenize(testFile, &numberOfTokens);
    errorMessage = parse(tokens, tokens, numberOfTokens);
    
    fclose(testFile);
    freeTokens(tokens, numberOfTokens);
    
    return errorMessage;
}


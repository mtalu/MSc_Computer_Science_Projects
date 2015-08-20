//
//  parser.h
//  
//
//  Created by ben on 07/11/2014.
//
//

#ifndef _parser_h
#define _parser_h

#include "../includes/actionQueueDataStructure.h"


int parse(char *inputString);

//initialiser:
void initialiseParser();
//destructors:
void freeParseLists();
void destroyEnvVarList();

//unit test suite:
void testParser();

#endif



//
//  michaelsMain.c
//  Bash_Defender_V2
//
//  My main module for testing purposes
//
//  Created by Michael on 22/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

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

int main (void) {
 int result = parseKill((char *[]) {"NULL", "kill *", "NULL"}, 3);
 printf("Result: %d", result);
 
 }

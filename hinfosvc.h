// Solution for IPK-proj1, 9.2.2022
// File:        hinfosvc.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0

// Included libraries 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DECIMAL 10


/**
 * Parse args 
 * usually return port number
 * return -1 if args are in bad format  
 */
int parse_args(int argc, char *argv[]);

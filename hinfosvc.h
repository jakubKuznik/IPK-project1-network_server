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


/**
 * Return computer network domain name, 
 * that is stored in: /proc/sys/kernel/hostname
 * 
 * returns NULL if not sucessfull. 
 * U HAVE TO FREE MEMORY !!!
*/
char * hostname();

/**
 * Return cpu computer cpu name with basic info.   
 */
char * cpu_name();

/**
 * Return cpu usage info.  
 */
char * cpu_usage();
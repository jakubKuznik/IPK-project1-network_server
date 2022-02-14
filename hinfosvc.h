// Solution for IPK-proj1, 9.2.2022
// File:        hinfosvc.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0

// Included libraries 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>	

#define DECIMAL 10
#define INDENT 33 
#define MAX_STAT 2048 //Max lenght of /proc/stat first line 
#define MESSAGE_MAX_SIZE 8192

#define LOCAL_HOST "127.0.0.1" 

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
 * 
 * 
 * call lscpu | grep Model\ name:
 * 
 * U HAVE TO FREE MEMORY !!! 
 * 
 */
char * cpu_name();

/**
 * Return cpu usage info.  
 */
long double cpu_usage();



/**
 * Get first line from file.
 * Max line size is MAX_STAT
 * if overflow return NULL
 */
char * get_first_line(FILE *f);

/**
 * Return string. 
 * Return null if error  
 */
char * read_file(FILE *f);



/**
 * create socket 
 * AF_INET      == ipv4
 * SOCK_STREAM  == reliable both ways conection (TCP)
 * 0            == IP protocol 
 * 
 * return -1 if error 
 */
int init_socket(int * soc, struct sockaddr_in * server, int port_number);
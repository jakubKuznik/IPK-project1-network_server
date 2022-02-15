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
#define SIZE 128

#define HOSTNAME 0
#define CPUINFO 1
#define CPULOAD 2
#define UNKNOWN 3

char mess_gud[MESSAGE_MAX_SIZE] = "HTTP/1.1 200 OK\r\nContent-Type: text/ plain;\r\n\r\n";
char mess_bad[MESSAGE_MAX_SIZE] = "400 Bad Request";


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
 * return NULL if error 
 * 
 * Calculate cpu usage from /proc/stat 
 * 
 * U HAVE TO FREE MEMORY!!
 */
char * cpu_usage();


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


/**
 * Parse message from client.  
 * 
 * return -1 if not successfull 
 * return HOSTNAME
 * return CPUINFO 
 * return CPULOAD 
 * 
 * return UNKNOWN if dont know the message 
 * 
 */
int parse_client_mess(int client_soc, char client_message[MESSAGE_MAX_SIZE]);


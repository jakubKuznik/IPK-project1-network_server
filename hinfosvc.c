// Solution for IPK-proj1, 9.2.2022
// File:        hinfosvc.c
// Author:      Jakub Kuzník, FIT
// Compiled:    gcc 9.9.3.0


/**
 * EXECUTION 
 * 
 * ./hinfosvc 12345
 *
 * 12345 is port number 
 * 
 */



#include "hinfosvc.h"

int main(int argc, char *argv[]){

    int port_number; // Http server ll listen on this port.

    port_number = parse_args(argc, argv);
    // Program error invalid arguments 
    if(port_number == -1)
        return -1;





    return 0;
}


/**
 * Return computer network domain name. 
*/
char * hostname(){
    return 0;
}

/**
 * Return cpu computer cpu name with basic info.   
 */
char * cpu_name(){
    return 0;
}

/**
 * Return cpu usage info.  
 */
char * cpu_usage(){
    return 0;
}


    
/*
fprintf(stderr," \n");    
*/

/**
 *  Parse args 
 *  usually return port number
 *  return -1 if args are in bad format  
 */
int parse_args(int argc, char *argv[]){
    
    int port;
    char *ptr = NULL; // indicator for strtol 
    
    // there are no arguments 
    if (argc < 2)
        goto error_1;

    // Convert input argument to int 
    port = strtol(argv[1], &ptr, DECIMAL);
    
    // if not a digit 
    if(*ptr != '\0')
        goto error_2;

    return port;

error_1:
    fprintf(stderr, "Missing port argument.\n");
    return -1;

error_2:
    fprintf(stderr, "%s is not a port number. \n",argv[1]);
    return -1;

}

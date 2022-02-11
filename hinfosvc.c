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

    char *d = hostname();
    printf("%s",d);
    free(d);



    return 0;
}

/**
 * Return string. 
 * Return null if error  
 */
char * read_file(FILE *f){

    // there is two for last byte that has to be 0
    char *s = malloc(2 * sizeof(char));
    if(s == NULL)
        return NULL;
    
    int c = 0;      // character 
    int size = 2;   // size of array
    int i = 0;      // index in array

    // store /proc/sys/kernel/hostname to variable hostname 
    while (c != EOF){
        c = fgetc(f);
        s[i++] = c;

        char *tmp = realloc(s, sizeof(char) * (size + 1));
        // realloc failed 
        if(tmp == NULL){
            free(s);
            return NULL;
        }
        size++;
        s = tmp;
    }
    // last byte has to be zero 
    s[i] = 0;
    return s;
}


/**
 * Return computer network domain name, 
 * that is stored in: /proc/sys/kernel/hostname
 * 
 * returns NULL if not sucessfull. 
 * U HAVE TO FREE MEMORY !!!
*/
char * hostname(){
    FILE *f = NULL;
    f = fopen("/proc/sys/kernel/hostname", "r");
    if (f == NULL)
        goto error_hostname_1;


    char *hostname = read_file(f);
    if (hostname == NULL)
        goto error_hostname_2;

    return hostname;


error_hostname_1:
    fprintf(stderr,"Cannot open: /proc/sys/kernel/hostname \n");    
    fclose(f);
    return NULL;

error_hostname_2:
    fprintf(stderr,"MALLOC ERROR \n");    
    fclose(f);
    return NULL;

}

/**
 * Return cpu computer cpu name with basic info.   
 * 
 * 
 * call lscpu | grep Model\ name:
 * 
 */
char * cpu_name(){
    FILE *f = NULL;
    f = popen("lscpu | grep Model\\ name:", "r");




    // popen return status of command that was executed by popen 
    int status = fclose(f);
    if(status == -1)
        return -1;
        //todo error

    return 0;
}

/**
 * Return cpu usage info.  
 */
char * cpu_usage(){
    return 0;
}


    

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
        goto error_args_1;

    // Convert input argument to int 
    port = strtol(argv[1], &ptr, DECIMAL);
    
    // if not a digit 
    if(*ptr != '\0')
        goto error_args_2;

    return port;

error_args_1:
    fprintf(stderr, "Missing port argument.\n");
    return -1;

error_args_2:
    fprintf(stderr, "%s is not a port number. \n",argv[1]);
    return -1;

}

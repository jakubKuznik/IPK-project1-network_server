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
 * U HAVE TO FREE MEMORY !!! 
 * 
 */
char * cpu_name(){
    FILE *f = NULL;
    f = popen("lscpu | grep Model\\ name:", "r");
    if (f == NULL) 
        goto error_cpu_1;

    char *cpu_name = read_file(f);
    if (cpu_name == NULL)
        goto error_cpu_2;

    // popen return status of command that was executed by popen 
    int status = fclose(f);
    if(status == -1)
        goto error_cpu_1;

    // now ouptput is in this format:
    // Model name:                      Intel(R) Core(TM) i7-4810MQ CPU @ 2.80GHz    return cpu_name;

    // format i want is
    // Intel(R) Core(TM) i7-4810MQ CPU @ 2.80GHz    return cpu_name;
    long unsigned int i = 0;
    for (; i <= strlen(cpu_name);){
        if(i >= INDENT)
            cpu_name[i-INDENT] = cpu_name[i];
        i++;
    }
    cpu_name[i-INDENT-2] = 0;

    return cpu_name;

error_cpu_1:
    fprintf(stderr,"lscpu | grep Model\\ name: FAILED\n");    
    return NULL;

error_cpu_2:
    fprintf(stderr,"MALLOC ERROR\n");    
    return NULL;
}

/**
 * Get first line from file.
 * Max line size is MAX_STAT
 * if overflow return NULL
 */
char * get_first_line(FILE *f){
    
    /*** gets first line from /proc/stat */
    bool overload = true;
    char *c = malloc(sizeof(char) * MAX_STAT);
    for(int i = 0; i < MAX_STAT-1; i++){
        c[i] = fgetc(f); 
        if(c[i] == '\n' || c[i] == EOF){
            c[i+1] = '\0';            
            overload = false;
            break;
        }   
    } 
    if (overload == true){
        free(c);    
        return NULL;
    }

    return c;
}

/* 
    from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

        user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
    cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0

    PrevIdle = previdle + previowait
    Idle = idle + iowait

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    NonIdle = user + nice + system + irq + softirq + steal

    PrevTotal = PrevIdle + PrevNonIdle
    Total = Idle + NonIdle

    # differentiate: actual value minus the previous one
    totald = Total - PrevTotal
    idled = Idle - PrevIdle

    CPU_Percentage = (totald - idled)/totald
*/

/**
 * Return cpu usage info. 
 * Return (-inf, -1) if error 
 * 
 * Calculate cpu usage from /proc/stat 
 * 
 * U HAVE TO FREE MEMORY!!
 */
double cpu_usage(){
    //     [0]     [1]    [2]     [3]       [4]    [5]   [6]      [7]    [8]    [9]
    //     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
    //cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
    long double p[7], n[7], cpu_perc, p_sum, n_sum, totald, idleld; // previous new
    FILE *f = NULL;
   
    f = fopen("/proc/stat", "r");
    if (f == NULL)
        goto error_usa_1;
    
    printf("%s", get_first_line(f));
    fclose(f);
    f = fopen("/proc/stat", "r");

    fscanf(f,"%*s %Lf %Lf %Lf %Lf %LF %LF %LF",&p[0],&p[1],&p[2],&p[3],&p[4],&p[5],&p[6]);
    fclose(f);
    usleep(0.5);

    f = fopen("/proc/stat", "r");
    printf("%s", get_first_line(f));
    fclose(f);
    
    f = fopen("/proc/stat", "r");
    fscanf(f,"%*s %Lf %Lf %Lf %Lf %LF %LF %LF",&n[0],&n[1],&n[2],&n[3],&n[4],&n[5],&n[6]);
    fclose(f);

    for(int i = 0 ; i < 7; i++)
        printf("%LF ",p[i]);
    printf("\n");
    for(int i = 0 ; i < 7; i++)
        printf("%LF ",n[i]);
    printf("\n");
    
    p_sum = p[0] + p[1] + p[2] + p[3];
    n_sum = n[0] + n[1] + n[2] + n[3];
    cpu_perc = ( (100*( n_sum - p_sum) - (n[3] - p[3])) / (n_sum - p_sum));
    printf("%LF \n",cpu_perc);

    //todo does not work 
    return ;

/**
    PrevIdle = previdle + previowait
    Idle = idle + iowait

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    NonIdle = user + nice + system + irq + softirq + steal

    PrevTotal = PrevIdle + PrevNonIdle
    Total = Idle + NonIdle

    # differentiate: actual value minus the previous one
    totald = Total - PrevTotal
    idled = Idle - PrevIdle

    CPU_Percentage = (totald - idled)/totald
*/

error_usa_1:
    fprintf(stderr,"Cannot open /proc/stat\n");    
    return NULL;
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
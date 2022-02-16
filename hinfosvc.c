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

    int port_number;                        // Http server ll listen on this port.
    int client_soc, c = 0;             // File descriptor of sockets will be stored here 
    int err_code = 0;                       // for error purpose 
    struct sockaddr_in server, client;      // network struct for client and server sockets 
    char client_message[MESSAGE_MAX_SIZE];  // Message that i ll get from client. 

    port_number = parse_args(argc, argv);
    if(port_number == -1) // Program error invalid arguments 
        return -1;

    // create network socket 
    err_code = init_socket(&ser_soc, &server, port_number);
    if (err_code == -1)
        return -1;

    // Listen on that socket 
    err_code = listen(ser_soc, 3);
    if (err_code < 0){
        fprintf(stderr, "Server is not listening \n");
        return -1;
    }

    int m = 0;                      // indicator 
    char message[MESSAGE_MAX_SIZE] = ""; // Message that ll be sent to client 
    char * help = NULL;
    // INFINITE TIME SERVER 
    // SIGINT signal handler
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = free_resources;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    while (true){
        // closing all resources if the SIGINT signal was received
        sigaction(SIGINT, &sigIntHandler, NULL);
        
        client_soc = accept(ser_soc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_soc > 0){
            // Get client message 
            m = parse_client_mess(client_soc, client_message);
            if (m == HOSTNAME){
                help = hostname();
                strcat(message, mess_gud);
                strcat(message, help);
                send(client_soc, message, strlen(message),0);
                free(help);
            }
            else if (m == CPUINFO){
                help = cpu_name();
                strcat(message, mess_gud);
                strcat(message, help);
                send(client_soc, message, strlen(message),0);
                free(help);
            }
            else if (m == CPULOAD){
                help = cpu_usage();
                strcat(message, mess_gud);
                strcat(message, help);
                send(client_soc, message, strlen(message),0);
                free(help);
            }
            else if (m == UNKNOWN){
                send(client_soc, mess_bad, strlen(mess_bad),0);
            }
            else{
                fprintf(stderr, "Cannot read client socket message\n");
            }
            message[0] = '\0';
            close(client_soc);
        }
    } 
    
    close(ser_soc);
    return 0;
}

/**
 * Free resource after signal SIGINT  
 */
void free_resources(int sig_num){
  
    // close socket etc.
    fprintf(stderr, "[signal %d] -> Process killed\n", sig_num);
    close(ser_soc);
    exit(1); 
}

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
int parse_client_mess(int client_soc, char client_message[MESSAGE_MAX_SIZE]){

    char help[MESSAGE_MAX_SIZE];
    recv(client_soc, client_message, MESSAGE_MAX_SIZE, 0);
    bool succes = false;

    // mesage format 
    // GET /hostname HTTP/1.1
    // this for just store get message if there is some 
    for (int i = 0; i <= MESSAGE_MAX_SIZE; i++){
        if (i < MESSAGE_MAX_SIZE - 5){
            if (client_message[i] == 'G' && client_message[i+1] == 'E' && \
                client_message[i+2] == 'T'){
                i = +4;
                for (int j = 0; j < MESSAGE_MAX_SIZE; j++){
                    if (client_message[i] == ' ') 
                        break;
                    help[j] = client_message[i++];
                    help[j+1] = '\0';
                }
                succes = true;
            }
        }
    }
    if (succes == false)
        return -1;

    if ((strcmp(help, "/hostname")) == 0)
        return HOSTNAME;
    else if ((strcmp(help, "/cpu-name")) == 0)
        return CPUINFO;
    else if ((strcmp(help, "/load")) == 0)
        return CPULOAD;
    else
        return UNKNOWN;
}

/**
 * create socket 
 * AF_INET      == ipv4
 * SOCK_STREAM  == reliable both ways conection (TCP)
 * 0            == IP protocol 
 * 
 * return -1 if error 
 */
int init_socket(int * soc, struct sockaddr_in * server, int port_number){
    
    *soc = socket(AF_INET, SOCK_STREAM, 0);
    if (*soc == -1){
        fprintf(stderr,"Can not create network socket.\n");
        return -1;
    }
    
    server->sin_family      = AF_INET; //ipv4
    server->sin_port        = htons(port_number);
    server->sin_addr.s_addr = INADDR_ANY; //binds a socket to all aviable interfaces

    int opt = 1; 
    setsockopt(*soc, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT , &opt, sizeof(opt));

    // bind() == set  ip 
    int err = bind (*soc, (struct sockaddr *)server, sizeof(*server));
    if (err < 0){
        fprintf(stderr,"Can not create network socket.\n");
        return -1;
    }
    
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

    int i = 0;
    for(; hostname[i] != '\n'; i++){;}
    hostname[i] = '\0';

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
    int j = 0;
    
    // skip Model name: 
    for (; cpu_name[j] != ':' ; j++)
        ;
    j++;
    
    // skip spaces
    for (; ; j++){
        if(cpu_name[j] == ' ')
            continue;
        else {
            j--;
            break;
        }
    }

    for (int i = 0; j <= MESSAGE_MAX_SIZE;i ++){
        j++;
        if(cpu_name[j] == '\n'){
            cpu_name[i] = '\0';
            break;
        }
        cpu_name[i] = cpu_name[j];
    }

    return cpu_name;

error_cpu_1:
    fprintf(stderr,"lscpu | grep Model\\ name: FAILED\n");    
    return NULL;

error_cpu_2:
    fprintf(stderr,"MALLOC ERROR\n");    
    return NULL;
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
 * return NULL if error 
 * 
 * Calculate cpu usage from /proc/stat 
 * 
 * U HAVE TO FREE MEMORY!!
 */
char * cpu_usage(){
    //     [0]     [1]    [2]     [3]       [4]    [5]   [6]      [7]    [8]    [9]
    //     user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
    //cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
    long long int p[10], n[10];
    long long int prev_idle, idle, prev_non_idle, non_idle, prev_total, total, total_ld, idled;
    double cpu_perc;

    FILE *f = NULL;
    f = fopen("/proc/stat", "r");
    if (f == NULL)
        goto error_usa_1;

    // Measure first values 
    fscanf(f,"cpu %lli   %lli   %lli   %lli   %lli   %lli %lli   %lli    %lli   %lli "\
                 ,&p[0], &p[1], &p[2], &p[3], &p[4],&p[5],&p[6], &p[7], &p[8], &p[9]);
    fclose(f);
    
    sleep(1);
    
    // Measure second time 
    f = fopen("/proc/stat", "r");
    fscanf(f,"cpu %lli %lli %lli   %lli  %lli  %lli   %lli  %lli    %lli   %lli "\
                ,&n[0],&n[1],&n[2],&n[3],&n[4],&n[5],&n[6], &n[7], &n[8], &n[9]);
    fclose(f);

    prev_idle   = p[3] + p[4];
    idle        = n[3] + n[4];

    prev_non_idle   = p[0] + p[1] + p[2] + p[5] + p[6] + p[7];
    non_idle        = n[0] + n[1] + n[2] + n[5] + n[6] + n[7];

    prev_total  = prev_idle + prev_non_idle;
    total       = idle + non_idle;

    total_ld    = total - prev_total;
    idled       = idle - prev_idle;

    // if There is 0 divison set to 100%
    if (total_ld == 0) cpu_perc = 100;
    else cpu_perc = ((total_ld - idled) / (double)total_ld)*100;


    int cpu_int = (int)cpu_perc;

    char *cpu_load = malloc(sizeof(char) *2000);
    if (cpu_load == NULL)
        return NULL;

    sprintf(cpu_load, "%d", cpu_int);
    return cpu_load;


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
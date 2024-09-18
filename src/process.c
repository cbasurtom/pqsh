/* process.c: PQSH Process */

#include "pqsh/utils.h"
#include "pqsh/process.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process *process_create(const char *command) {
    
    Process *p = calloc(1,sizeof(Process));
    
    // Check for errors
    if (!p) return NULL;
    
    strcpy(p->command,command);

    // Update arrival time
    p->arrival_time = timestamp();
    return p;
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {

    char *argv[MAX_ARGUMENTS] = {0};
    char command[MAX_ARGUMENTS];
    int i = 0;
    pid_t pid = fork();
    p->start_time = timestamp();
    switch (pid){
        case  0: // child
            strcpy(command,p->command);
            
            for (char *token = strtok(command, " "); token; token = strtok(NULL, " ")) {
                 argv[i++] = token;
            }       
            execvp(argv[0], argv);
            _exit(EXIT_FAILURE);
            break;
            
        case -1: // parent (error)
            return false;
            //break
        default://parent (sucess) - stores child pid
            p->pid = pid;
            break;

    }
    return true;
}

/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {

    if (kill(p->pid,SIGSTOP) == 0) return true;
    return false;
}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_resume(Process *p) {

    if (kill(p->pid,SIGCONT)==0) return true;
    return false;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
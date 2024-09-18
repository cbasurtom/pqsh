/* pqsh.c: Process Queue Shell */

#include "pqsh/utils.h"
#include "pqsh/options.h"
#include "pqsh/scheduler.h"

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

/* Global Variables */

extern Scheduler PQSHScheduler;

/* Help Message */

void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}

/* Main Execution */

int main(int argc, char *argv[]) {
    Scheduler *s = &PQSHScheduler;

    // Parse command line options
    if(!parse_command_line_options(argc,argv,s))
        return 1;
    
    // Register signal handlers
    struct sigaction sigalrm_action = {.sa_handler = scheduler_handle_sigalrm};
    if (sigaction(SIGALRM, &sigalrm_action, NULL) < 0) {
        printf("Error registering SIGALRM");
        exit(EXIT_FAILURE);
    }

    struct sigaction sigchld_action = {.sa_handler = scheduler_handle_sigchld};
    if (sigaction(SIGCHLD, &sigchld_action, NULL) < 0) {
        printf("Error registering SIGCHLD");
        exit(EXIT_FAILURE);
    }

    //Start timer interrupt 
    struct itimerval interval = {
        .it_interval = { .tv_sec = 0, .tv_usec = s->timeout },
        .it_value    = { .tv_sec = 0, .tv_usec = s->timeout },
    };
    if (setitimer(ITIMER_REAL, &interval, NULL) < 0) {
        printf("Error setting timer");
        exit(EXIT_FAILURE);
    }
 
    // Process shell commands
    while (!feof(stdin)) {
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";

        printf("\nPQSH> ");

        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin)) {
            // Check events

            // Timer goes off
            if (s->event == EVENT_TIMER) {
                scheduler_next(s);
            }

            // Child process terminates
            if (s->event == EVENT_CHILD) {
                scheduler_wait(s);
            }

            s->event = 0; // reset event
        }
        
        // User input
        chomp(command);
        if (streq(command, "help")) {
            help();
        } else if (streq(command, "exit") || streq(command, "quit")) {
                break;
        } else if (sscanf(command, "add %[^\t\n]", argument) == 1) {
            /* TODO: Handle add command */
            scheduler_add(s, argument);
        } else if (streq(command, "status") || sscanf(command, "status %[^\t\n]", argument) == 1) {
            /* TODO: Handle status command */
            if (strcmp(argument, "running") == 0) scheduler_status(s, RUNNING);
            else if (strcmp(argument, "waiting") == 0) scheduler_status(s, WAITING);
            else if (strcmp(argument, "finished") == 0) scheduler_status(s, FINISHED);
            else scheduler_status(s, 0);
        } else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        } 
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
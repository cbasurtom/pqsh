/* scheduler.c: PQSH Scheduler */

#include "pqsh/utils.h"
#include "pqsh/scheduler.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <sys/wait.h>

/* Global Variables */

Scheduler PQSHScheduler = {
    .policy    = FIFO_POLICY,
    .cores     = 1,
    .timeout   = 250000,
    .event     = EVENT_INPUT,
};

/* Functions */

/**
 * Add new command to waiting queue.
 * @param   s	    Pointer to Scheduler structure.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, const char *command) {

    // Create process and add to waiting queue
    Process* p = process_create(command);

    // Check if process_create fails
    if (!p) { printf("Failed to add process"); return; }

    queue_push(&s->waiting, p);
    printf("Added process \"%s\" to waiting queue\n", p->command);
}

/**
 * Display status of queues in Scheduler.
 * @param   s	    Pointer to Scheduler structure.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, int queue) {
    
    printf("Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n", 
    s->running.size, s->waiting.size, s->finished.size, s->total_turnaround_time/s->finished.size, s->total_response_time/s->finished.size);
    
    // Specific behavior
    if (queue & RUNNING) {
        if (s->running.size != 0) {
            printf("\nRunning Queue:\n");
            queue_dump(&s->running, stdout);
        }
    } else if (queue & WAITING) {
        if (s->waiting.size != 0) {
            printf("\nWaiting Queue:\n");
            queue_dump(&s->waiting, stdout);
        }
    } else if (queue & FINISHED) {
        if (s->finished.size != 0) {
            printf("\nFinished Queue:\n");
            queue_dump(&s->finished, stdout);
        }
    
    // Default behavior, all
    } else {
        if (s->running.size != 0) {
            printf("\nRunning Queue:\n");
            queue_dump(&s->running, stdout);
        }   

        if (s->waiting.size != 0) {
            printf("\nWaiting Queue:\n");
            queue_dump(&s->waiting, stdout);
        }

        if (s->finished.size != 0) {
            printf("\nFinished Queue:\n");
            queue_dump(&s->finished, stdout);
        }
    }
    
}

/**
 * Schedule next process using appropriate policy.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {
    // Dispatch to appropriate scheduler function
    if (s->policy == FIFO_POLICY) {
        scheduler_fifo(s);
    } else {
        scheduler_rdrn(s);
    }
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {

    pid_t pid;

    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        // Remove process from queues
        Process *found  = queue_remove(&s->running, pid);

        if (!found) printf("Failed to remove from running queue\n");

        // Update Process metrics
        found->end_time = timestamp();

        queue_push(&s->finished, found);

        Update Scheduler metrics
        s->total_turnaround_time += found->end_time   - found->arrival_time;
        s->total_response_time   += found->start_time - found->arrival_time;

    }
}

/**
 * Handle SIGALRM by setting appropriate event.
 * @param   signum  Signal number (ignored).
 **/
void scheduler_handle_sigalrm(int signum) {

    PQSHScheduler.event = EVENT_TIMER;
}

/**
 * Handle SIGCHLD by setting appropriate event.
 * @param   signum  Signal number (ignored).
 **/
void scheduler_handle_sigchld(int signum) {
    PQSHScheduler.event = EVENT_CHILD;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
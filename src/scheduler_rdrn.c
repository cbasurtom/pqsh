/* scheduler_rdrn.c: PQSH Round Robin Scheduler */

#include "pqsh/utils.h"
#include "pqsh/scheduler.h"

#include <assert.h>

/**
 * Schedule next process using round robin policy:
 *
 *  1. If there no waiting processes, then do nothing.
 *
 *  2. Move one process from front of running queue and place in back of
 *  waiting queue.
 *
 *  3. Move one process from front of waiting queue and place in back of
 *  running queue.
 *
 * @param   s	    Scheduler structure
 **/
void scheduler_rdrn(Scheduler *s) {

    if (s->running.size == s->cores){
        Process *process = queue_pop(&s->running);
        if(process_pause(process))
            queue_push(&s->waiting,process);
        else{
            queue_push(&s->waiting,process);
            }
    }

    while (s->running.size < s->cores && s->waiting.size) {
        Process *process = queue_pop(&s->waiting);
        if (process->pid ==0)
            if(process_start(process)){
                queue_push(&s->running,process);
            }else{
                    queue_push(&s->waiting,process);
                }
            
        else {
            if(process_resume(process)) {
                queue_push(&s->running,process);
            } else {
                queue_push(&s->waiting,process);
            }
        }       
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
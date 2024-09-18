/* queue.c: PQSH Queue */

#include "pqsh/utils.h"
#include "pqsh/queue.h"

#include <assert.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void queue_push(Queue *q, Process *p) {
    // Ensure new process next pointer is NULL, as it is last in the queue
    p->next = NULL;

    // Check if queue is empty
    if (q->size == 0) {
        q->head       = p;
        q->tail       = p;
    } else {
        q->tail->next = p;
        q->tail       = p;
    }

    q->size++;

    return;   
}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process * queue_pop(Queue *q) {

    // Check if queue is empty
    if (q->size == 0) return NULL;
    else {
        Process* p  = q->head;

        // Check if queue only has one element
        if (q->size == 1) {
            q->head = NULL;
            q->tail = NULL;
            
        // 2 elements or more
        } else {
            q->head = q->head->next;
        }

        q->size--;
        p->next     = NULL;

        return p;
    }

}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process * queue_remove(Queue *q, pid_t pid) {

    // Check if queue is empty
    if (q->size == 0) return NULL;

    Process* p = q->head;

    // Check if queue only has one element
    if (q->size == 1) {
        if (p->pid == pid) {
            q->head = NULL;
            q->tail = NULL;
            q->size--;
            return p;
        } 
        return NULL;
    }

    // 2 elements or more, must check through next pointers
    while (p->next) {
        if (p->pid == pid) {
            // Check if element to be removed is the head
            if (p == q->head) {
                q->head = q->head->next;
            }
            
            p->next = NULL;
            q->size--;
            return p;
        } else if (p->next->pid == pid) {
            
            Process* target = p->next;
            
            // Check if element to be removed is the tail
            if (p->next->next == NULL) {
                q->tail = p;
                p->next = NULL;
            } else {
                p->next = target->next;
            }
            
            target->next = NULL;
            q->size--;
            return target;
        }
        
        // Check next element
        p = p->next;
    }

    return NULL;
}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void queue_dump(Queue *q, FILE *fs) {
    // Display information for each item in Queue
    if (q->size != 0) {
        fprintf(fs, "%9s %-30s %-13s %-13s %-13s\n",
                "PID", "COMMAND", "ARRIVAL", "START", "END");

        Process* p = q->head;
        while (p) {
            printf("%9u %-30s %11.2lf %11.2lf %11.2lf\n", p->pid, p->command, p->arrival_time, p->start_time, p->end_time);
            p = p->next;
        }
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
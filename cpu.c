/****************************************************************************/
/*                                                                          */
/*				Module CPU				    */
/*			     Internal Routines				    */
/*                                                                          */
/****************************************************************************/


#include "cpu.h"
#include "queue.h"
#include <stdlib.h>
#include <math.h>

int compareTo(PCB *pcb1, PCB *pcb2);
char *toString(PCB *pcb);

Queue queue; //This is probably wrong
void cpu_init()
{
    setTimer(1000000);
    initQueue(&queue);
}



void dispatch()
{
    PCB *p = frontValue(&queue);
    if(PTBR != NULL && PTBR->pcb->status == running) { //If a process is already running
        PTBR->pcb->priority = PTBR-> pcb->burst_estimate - PTBR->pcb->this_cpuburst; //Priority = estimate - bursts so far

        if(compareTo(PTBR->pcb,p) <= 0) {//PTBR <= head of queue priority
            PTBR->pcb->last_dispatch = get_clock();
            return;
        }
    }
    else { //No process running
        if(isEmpty(&queue)) { //queue is empty
            PTBR = NULL;
            return;
        }
    }
    insert_ready(PTBR->pcb);
    deQueue(&queue);
    p->status = running;
    PTBR = p->page_tbl;
    prepage(p);
    p->last_dispatch = get_clock();


}

void insert_ready(PCB *pcb)
{

    if (pcb -> last_dispatch == 0) //If it is a new process
        pcb -> burst_estimate = 5;
    else //If the process has been run before
        pcb -> burst_estimate = (pcb -> burst_estimate*.6) + (.4*pcb-> last_cpuburst);

    pcb -> priority = pcb -> burst_estimate; //Priority = burst estimate
    pcb -> status = ready;
    enQueueSorted(&queue, pcb, *compareTo ); //Add to priority queue

}


char *toString(PCB *pcb)
{
    static char result[BUFSIZ];
    sprintf (result, "pcb-%d(prior-%d) ", pcb->pcb_id, pcb->priority);
    return result;
}


int compareTo(PCB *pcb1, PCB *pcb2)
{
    return pcb1 -> priority - pcb2 ->priority;
}


/* end of module */


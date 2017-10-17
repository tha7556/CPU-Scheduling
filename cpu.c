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

Queue queue;
void cpu_init()
{
    set_timer(1000000);
    initQueue(&queue);
}



void dispatch()
{
	if (______trace_switch) { printf("dispatch called\n"); }
    PCB *p = frontValue(&queue);
    if(PTBR != NULL && PTBR->pcb != NULL && PTBR->pcb->status == running) { //If a process is already running
        PTBR->pcb->priority = PTBR-> pcb->burst_estimate - PTBR->pcb->this_cpuburst; //Priority = estimate - bursts so far
        if (______trace_switch) { printf("1\n"); }
        if(compareTo(PTBR->pcb,p) <= 0) {//PTBR <= head of queue priority
            if (______trace_switch) { printf("2\n"); }
            PTBR->pcb->last_dispatch = get_clock();
            return;
        }
    }
    else { //No process running
        if (______trace_switch) { printf("3\n"); }
        if(isEmpty(&queue)) { //queue is empty
                if (______trace_switch) { printf("4\n"); }
            PTBR = NULL;
            return;
        }
    }
    if (______trace_switch) { printf("5\n"); }
    if(PTBR != NULL)
        insert_ready(PTBR->pcb);
    deQueue(&queue);
    p->status = running;
    PTBR = p->page_tbl;
    if(______trace_switch) {printf("starting prepaging\n");}
    prepage(p);
    p->last_dispatch = get_clock();
	if (______trace_switch) {
                    printf("dispatch finished\n");
                }


}

void insert_ready(PCB *pcb)
{
    int alpha = .5;
    int initial = 10;
	if (______trace_switch) {
                    printf("insertReady called\n");
                }
    if (pcb -> last_dispatch == 0) //If it is a new process
        pcb -> burst_estimate = initial;
    else //If the process has been run before
        pcb -> burst_estimate = (pcb -> burst_estimate*alpha) + ((1-alpha)*pcb-> last_cpuburst);

    pcb -> priority = pcb -> burst_estimate; //Priority = burst estimate
    pcb -> status = ready;
    enQueueSorted(&queue, pcb, *compareTo ); //Add to priority queue
	if (______trace_switch) {
                    printf("insertReady finished\n");
                }

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


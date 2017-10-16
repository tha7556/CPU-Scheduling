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

void cpu_init()
{

}



void dispatch()
{
}



void insert_ready(PCB *pcb)
{

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


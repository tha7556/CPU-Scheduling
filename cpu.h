
/****************************************************************************/
/*                                                                          */
/* 			     Module CPU                                     */
/* 			External Declarations 				    */
/*                                                                          */
/****************************************************************************/


/* OSP constant      */

#define   MAX_PAGE                   16 /* max size of page tables          */
#define   MAX_PCB                    50 /* max number of pcb in system      */

/* OSP enumeration constants */

typedef enum {
    false, true                         /* the boolean data type            */
} BOOL;

typedef enum {
    running, ready, waiting, done       /* types of status                  */
} STATUS;


typedef enum {
    iosvc, devint,                      /* types of interrupt               */
    pagefault, startsvc,
    termsvc, killsvc,
    waitsvc, sigsvc, timeint
} INT_TYPE;

typedef enum {
    read, write                         /* type of actions for I/O requests */
} IO_ACTION;




/* external type definitions */

typedef struct page_entry_node PAGE_ENTRY;
typedef struct page_tbl_node PAGE_TBL;
typedef struct event_node EVENT;
typedef struct iorb_node IORB;
typedef struct pcb_node PCB;
typedef struct int_vector_node INT_VECTOR;




/* external data structures */

struct page_entry_node {
    int    frame_id;    /* frame id holding this page                       */
    BOOL   valid;       /* page in main memory : valid = true; not : false  */
    int    *hook;       /* can hook up anything here                        */
};

struct page_tbl_node {
    PCB    *pcb;        /* PCB of the process in question                   */
    PAGE_ENTRY page_entry[MAX_PAGE];
    int    *hook;       /* can hook up anything here                        */
};

struct pcb_node {
    int    pcb_id;         /* PCB id                                        */
    int    size;           /* process size in bytes; assigned by SIMCORE    */
    int    creation_time;  /* assigned by SIMCORE                           */
    int    last_dispatch;  /* last time the process was dispatched          */
    int    last_cpuburst;  /* length of the previous cpu burst              */
    int    this_cpuburst;  /* accumulated CPU time of this burst            */
    int    burst_estimate; /* estimate of next CPU burst                    */
    int    accumulated_cpu;/* accumulated CPU time                          */
    PAGE_TBL *page_tbl;    /* page table associated with the PCB            */
    STATUS status;         /* status of process                             */
    EVENT  *event;         /* event upon which process may be suspended     */
    int    priority;       /* user-defined priority; used for scheduling    */
    PCB    *next;          /* next pcb in whatever queue                    */
    PCB    *prev;          /* previous pcb in whatever queue                */
    int    *hook;          /* can hook up anything here                     */
};


struct int_vector_node {
    INT_TYPE cause;           /* cause of interrupt                         */
    PCB    *pcb;              /* PCB to be started (if startsvc) or pcb that*/
                              /* caused page fault (if fagefault interrupt) */
    int    page_id;           /* page causing pagefault                     */
    int    dev_id;            /* device causing devint                      */
    EVENT  *event;            /* event involved in waitsvc and sigsvc calls */
    IORB   *iorb;             /* IORB involved in iosvc call                */
};


/* external variables */

extern PAGE_TBL *PTBR;		/* page table base register */
extern INT_VECTOR Int_Vector;   /* interrupt vector                  */

extern int    Quantum;		/* global time quantum; contains the value
				   entered at the beginning or changed 
				   at snapshot. Has no effect on timer
				   interrupts, unless passed to set_timer() */

extern int    ______trace_switch;



/* external routines */

extern prepage(/* pcb */);
extern int start_cost(/* pcb */);
/*  PCB    *pcb; */    

extern set_timer(/* time_quantum */);
/*  int    time_quantum; */

extern int get_clock();


/* internal routines */
void cpu_init();
void dispatch();
void insert_ready(PCB *);


/* end of module */

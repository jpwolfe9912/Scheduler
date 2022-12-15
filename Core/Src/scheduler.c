#include "scheduler.h"

// Tasks_t loops[8];

volatile uint8_t loopTaskIdx[8] = {0};

static void runAllTasksInLoop(Tasks* tasks);

void run(Tasks **head_ref)
{
    while (!flag)
        flag = false;

    LoopFreqs_e loopToRun = FRAME_1000HZ;

    for (; loopToRun < TOTAL_LOOPS; loopToRun++)
    {
        if (((loopMask >> loopToRun) & 0x01)) // if there is a 1 in the spot of the loop to run
        {
            runAllTasksInLoop(head_ref[loopToRun]);    // run all the tasks in that loop
            loopMask &= ~(0x1 << loopToRun); // clear the mask
        }
    }
}

static void runAllTasksInLoop(Tasks* tasks)
{
    while (tasks != NULL)
    {
        tasks->task();
        tasks = tasks->next;
    }
}


/* Linked List Functions */

/* Given a reference (pointer to pointer) to the head of a list and
   an int, inserts a new node on the front of the list. */
void push(Tasks **head_ref, void (*new_task)(void))
{
    /* 1. allocate node */
    Tasks *new_node = (Tasks *)malloc(sizeof(Tasks));

    /* 2. put in the data  */
    new_node->task = new_task;

    /* 3. Make next of new node as head */
    new_node->next = (*head_ref);

    /* 4. move the head to point to the new node */
    (*head_ref) = new_node;
}

/* Given a node prev_node, insert a new node after the given
   prev_node */
void insertAfter(Tasks *prev_node, void (*new_task)(void))
{
    /*1. check if the given prev_node is NULL */
    if (prev_node == NULL)
    {
        printf("the given previous node cannot be NULL");
        return;
    }

    /* 2. allocate new node */
    Tasks *new_node = (Tasks *)malloc(sizeof(Tasks));

    /* 3. put in the data  */
    new_node->task = new_task;

    /* 4. Make next of new node as next of prev_node */
    new_node->next = prev_node->next;

    /* 5. move the next of prev_node as new_node */
    prev_node->next = new_node;
}

/* Given a reference (pointer to pointer) to the head
   of a list and an int, appends a new node at the end  */
void append(Tasks **head_ref, void (*new_task)(void))
{
    /* 1. allocate node */
    Tasks *new_node = (Tasks *)malloc(sizeof(Tasks));

    Tasks *last = *head_ref; /* used in step 5*/

    /* 2. put in the data  */
    new_node->task = new_task;

    /* 3. This new node is going to be the last node, so make next of
          it as NULL*/
    new_node->next = NULL;

    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }

    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last node */
    last->next = new_node;
    return;
}
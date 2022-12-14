#include "scheduler.h"

Loop_t loops[8];

volatile uint8_t loopTaskIdx[8] = {0};

static void runAllTasksInLoop(LoopFreqs_e loopFreq);

void addTaskToLoop(LoopFreqs_e loopFreq, void (*loopTask)(void))
{
    uint8_t index = loops[loopFreq].taskIdx;    // index of the next spot available
    loops[loopFreq].taskList[index] = loopTask; // add task to the next spot in the list
    loops[loopFreq].taskIdx++;                  // increment task index
}

static void runAllTasksInLoop(LoopFreqs_e loopFreq)
{
    uint8_t currentTaskIdx = 0;
    uint8_t taskCount = loops[loopFreq].taskIdx;         // number of tasks to run
    for (; currentTaskIdx < taskCount; currentTaskIdx++) // cycle through
    {
        loops[loopFreq].taskList[currentTaskIdx](); // run the task
    }
}

void run(void)
{
    while (!flag)
        flag = false;

    LoopFreqs_e loopToRun = FRAME_1000HZ;

    for (; loopToRun < TOTAL_LOOPS; loopToRun++)
    {
        if (((loopMask >> loopToRun) & 0x01)) // if there is a 1 in the spot of the loop to run
        {
            runAllTasksInLoop(loopToRun);     // run all the tasks in that loop
            loopMask &= ~(0x1 << loopToRun);      // clear the mask
        }
    }
}
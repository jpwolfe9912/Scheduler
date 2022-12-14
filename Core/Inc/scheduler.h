#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "main.h"

#define COUNT_1000HZ 1U // Number of 1000 Hz frames for 500 Hz Loop
#define COUNT_500HZ 2U  // Number of 1000 Hz frames for 500 Hz Loop
#define COUNT_200HZ 5U  // Number of 1000 Hz frames for 100 Hz Loop
#define COUNT_100HZ 10U // Number of 1000 Hz frames for 100 Hz Loop
#define COUNT_50HZ 20U  // Number of 1000 Hz frames for  50 Hz Loop
#define COUNT_10HZ 100U // Number of 1000 Hz frames for  10 Hz Loop
#define COUNT_5HZ 200U  // Number of 1000 Hz frames for   5 Hz Loop
#define COUNT_1HZ 1000U // Number of 1000 Hz frames for   1 Hz Loop

#define MASK_1000HZ (0x1 << 0U)
#define MASK_500HZ (0x1 << 1U)
#define MASK_200HZ (0x1 << 2U)
#define MASK_100HZ (0x1 << 3U)
#define MASK_50HZ (0x1 << 4U)
#define MASK_10HZ (0x1 << 5U)
#define MASK_5HZ (0x1 << 6U)
#define MASK_1HZ (0x1 << 7U)

#define TOTAL_LOOPS 8U

typedef enum
{
    FRAME_1000HZ = 0,
    FRAME_500HZ,
    FRAME_200HZ,
    FRAME_100HZ,
    FRAME_50HZ,
    FRAME_10HZ,
    FRAME_5HZ,
    FRAME_1HZ
} LoopFreqs_e;

typedef struct
{
    uint8_t taskIdx;
    void (*taskList[256])(void);
} Loop_t;

/* Function Prototypes */
void addTaskToLoop(LoopFreqs_e loopFreq, void (*loopTask)(void));
void run(void);

#endif
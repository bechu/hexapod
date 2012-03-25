#include "sys/axon2.h"
#include "core.h"
#include "timer.h"
#include "uart.h"
#include "rprintf.h"

void appInitHardware(void)
{
    uartInit(UART1, 19200);
    rprintfInit(&uart1SendByte);
}

TICK_COUNT appInitSoftware(TICK_COUNT loopStart)
{
    return 0; // dont pause after
}

TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart)
{
    rprintf("Hello world\n");
    return 1000000; // wait for 1 second before calling me again. 1000000us = 1
}



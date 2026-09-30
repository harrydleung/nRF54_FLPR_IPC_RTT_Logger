#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "SEGGER_RTT.h"
#include <zephyr/logging/log.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include "ipc.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    /* Register the receiver that prints FLPR logs through CPUAPP's RTT console. */
    dbgmsg_ipc_init();
    while (1)
    {
        LOG_INF("Hello from ARM");
        k_sleep(K_MSEC(1000));
    }
    return 0;
}
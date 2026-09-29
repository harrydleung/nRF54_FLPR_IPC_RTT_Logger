#include <stdio.h>
#include <zephyr/kernel.h>

#include "ipc.h"
#include "ipc_logger.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(Main, LOG_LEVEL_INF);

int main(void)
{
    ipc_logger_init();
    while (!ipc_get_dbgmsg_bounded())
    {
        k_sleep(K_MSEC(10));
    }
    while (1)
    {
        LOG_INF("Hello world from flpr");
        k_sleep(K_MSEC(1000));
    }
    return 0;
}

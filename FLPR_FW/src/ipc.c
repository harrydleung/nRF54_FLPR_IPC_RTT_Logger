#include "ipc.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include <zephyr/ipc/ipc_service.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(flpr_ipc, CONFIG_LOG_DEFAULT_LEVEL);

// Debug message IPC

K_SEM_DEFINE(dbgmsg_bound_sem, 0, 1);

static void dbgmsg_bound(void *priv)
{
    k_sem_give(&dbgmsg_bound_sem);
    LOG_INF("dbgmsg ep bounded");
}
static void dbgmsg_unbound(void *priv)
{
    k_sem_take(&dbgmsg_bound_sem, K_FOREVER);
    LOG_INF("dbgmsg ep unbounded");
}
static void ep_error(const char *err, void *priv)
{
    LOG_ERR("ep error %s", err);
}
uint32_t ipc_get_dbgmsg_bounded(void)
{
    return k_sem_count_get(&dbgmsg_bound_sem);
}

static struct ipc_ept_cfg dbgmsg_ep_cfg = {
    .cb = {
        .bound = dbgmsg_bound,
        .unbound = dbgmsg_unbound,
        //.received = dbgmsg_recv,
        .error = ep_error,
    },
    .name = "dbgmsg_ep",
};

static struct ipc_ept dbgmsg_ep;
static const struct device *dbgmsg_ipc_dev;

void dbgmsg_ipc_init(void)
{

    dbgmsg_ipc_dev = DEVICE_DT_GET(DT_NODELABEL(ipc0));
    int ret;

    ret = ipc_service_open_instance(dbgmsg_ipc_dev);
    if ((ret < 0) && (ret != -EALREADY))
    {
        LOG_ERR("ipc_service_open_instance() failure");
    }
    ret = ipc_service_register_endpoint(dbgmsg_ipc_dev, &dbgmsg_ep, &dbgmsg_ep_cfg);
    if (ret < 0)
    {
        LOG_ERR("ipc_service_register_endpoint() failure");
    }
}

void dbgmsg_ipc_send(const void *data, size_t len)
{
    if (k_sem_count_get(&dbgmsg_bound_sem) > 0)
    {
        ipc_service_send(&dbgmsg_ep, data, len);
    }
}

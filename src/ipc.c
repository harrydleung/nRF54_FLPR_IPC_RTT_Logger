#include "ipc.h"

#include <zephyr/ipc/ipc_service.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(cpu_ipc, CONFIG_LOG_DEFAULT_LEVEL);

/* Debug message IPC */
static void dbgmsg_bound(void *priv)
{
    // k_sem_give(&bound_sem);
    LOG_INF("dbgmsg ep bounded");
}
static void dbgmsg_unbound(void *priv)
{
    LOG_INF("dbgmsg ep unbounded");
}
static void dbgmsg_recv(const void *data, size_t len, void *priv)
{
    /* IPC text need not be NUL-terminated; bound output by the received length. */
    printk("FLPR DBG:%.*s", (int)len, (const char *)data);
}
static void ep_error(const char *err, void *priv)
{
    LOG_ERR("ep error %s", err);
}

static struct ipc_ept_cfg dbgmsg_ep_cfg = {
    .cb = {
        .bound = dbgmsg_bound,
        .unbound = dbgmsg_unbound,
        .received = dbgmsg_recv,
        .error = ep_error,
    },
    /* This name must match the endpoint registered by FLPR. */
    .name = "dbgmsg_ep",
};

static struct ipc_ept dbgmsg_ep;
static const struct device *dbgmsg_ipc_dev;

void dbgmsg_ipc_init(void)
{
    int ret;

    dbgmsg_ipc_dev = DEVICE_DT_GET(DT_NODELABEL(ipc0));
    ret = ipc_service_open_instance(dbgmsg_ipc_dev);
    /* An already-open IPC instance can still be used to register this endpoint. */
    if ((ret < 0) && (ret != -EALREADY))
    {
        LOG_ERR("ipc_service_open_instance() failure");
    }
    /* Registration starts the handshake; dbgmsg_bound() reports readiness. */
    ret = ipc_service_register_endpoint(dbgmsg_ipc_dev, &dbgmsg_ep, &dbgmsg_ep_cfg);
    if (ret < 0)
    {
        LOG_ERR("ipc_service_register_endpoint() failure");
    }
}

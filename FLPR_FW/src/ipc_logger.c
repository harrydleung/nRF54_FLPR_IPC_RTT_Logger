#include "ipc_logger.h"

#include <zephyr/logging/log_backend.h>
#include <zephyr/logging/log_output.h>
#include <zephyr/logging/log_ctrl.h>

#include "ipc.h"

/* Formatting scratch space; longer log messages can be flushed in chunks. */
static uint8_t log_format_buffer[256];

static int dbgmsg_ipc_out_func(uint8_t *data, size_t length, void *ctx)
{
    dbgmsg_ipc_send(data, length);
    /* Mark the chunk consumed even if IPC discarded it or failed to send it. */
    return length;
}

LOG_OUTPUT_DEFINE(dbgmsg_ipc_log_output, dbgmsg_ipc_out_func, log_format_buffer, sizeof(log_format_buffer));

static void logger_process(const struct log_backend *const backend, union log_msg_generic *msg)
{
    /* Forward readable text with a severity prefix and formatted timestamp. */
    uint32_t flags = LOG_OUTPUT_FLAG_LEVEL | LOG_OUTPUT_FLAG_TIMESTAMP | LOG_OUTPUT_FLAG_FORMAT_TIMESTAMP;
    log_output_msg_process(&dbgmsg_ipc_log_output, &msg->log, flags);
}

static const struct log_backend_api dbgmsg_log_backend_api = {
    .process = logger_process,
};
/* Disable autostart so main() can set up IPC before enabling this backend. */
LOG_BACKEND_DEFINE(dbgmsg_ipc_backend, dbgmsg_log_backend_api, false);

void ipc_logger_init(void)
{
    dbgmsg_ipc_init();
    /* Binding is asynchronous; main() waits before producing application logs. */
    log_backend_enable(&dbgmsg_ipc_backend, NULL, CONFIG_LOG_MAX_LEVEL);
}

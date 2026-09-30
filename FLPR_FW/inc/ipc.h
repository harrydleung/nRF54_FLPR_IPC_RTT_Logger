#ifndef __ipc_h
#define __ipc_h

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/* Register FLPR's log endpoint; the bound callback signals readiness later. */
void dbgmsg_ipc_init(void);
/* Return the current bound flag (0 or 1) without consuming it. */
uint32_t ipc_get_dbgmsg_bounded(void);
/* Send a text chunk if bound; unbound chunks and send errors are not retried. */
void dbgmsg_ipc_send(const void *data, size_t len);
void magdata_ipc_init(void);
int magdata_ipc_send(const void *data, size_t len);
#endif
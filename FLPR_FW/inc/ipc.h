#ifndef __ipc_h
#define __ipc_h

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

void dbgmsg_ipc_init(void);
uint32_t ipc_get_dbgmsg_bounded(void);
void dbgmsg_ipc_send(const void *data, size_t len);
void magdata_ipc_init(void);
int magdata_ipc_send(const void *data, size_t len);
#endif
#ifndef __ipc_h
#define __ipc_h
#include <zephyr/types.h>

void dbgmsg_ipc_init(void);
void magdata_ipc_init(void);
int magdata_ipc_send(const void *data, size_t len);

#endif /* __ipc_h */

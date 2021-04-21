#ifndef __MOD_TCP_CONNECT__
#define __MOD_TCP_CONNECT__

#include "modbus_scan.h"

int connect_non_block(NWLOG_HANDLE_t *logfp, int sockfd, const struct sockaddr *sock_addr, int socklen, int time);
int modbus_tcp_connect(NWLOG_HANDLE_t *logfp, char *ip, int port);

#endif
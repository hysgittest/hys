#ifndef __MOD_HEADER__
#define __MOD_HEADER__

// MODScan COMMON Header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <getopt.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

// MODScan USER Header
#include "modbus_define.h"

//Nw Libs
#include "NwLogNew.h"
#include "cmd_receiver.h"

// function init
void error_handling(char *message);
unsigned short reverseByte(unsigned short value);
void Print_send_hexa_log(NWLOG_HANDLE_t *logfp, char *buff, size_t len, char *nowday);
void Print_recv_hexa_log(NWLOG_HANDLE_t *logfp, char *buff, size_t len, char *nowday);
void cmd2_init();
void time_change(int argc, char *argv[]);
void modbus_scan(NWLOG_HANDLE_t *logfp, char unitid, char *ip, int port);

#endif
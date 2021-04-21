#ifndef __RECV_MOD_MSG__
#define __RECV_MOD_MSG__

#include "modbus_scan.h"

int recv_bit_msg(NWLOG_HANDLE_t *logfp, int sock, char *nowday, short starting_address_cfg, short number_of_registers_cfg, char function_code, short *reg);
int recv_word_msg(NWLOG_HANDLE_t *logfp, int sock, char *nowday, short starting_address_cfg, short number_of_registers_cfg, char function_code, short *reg);

#endif
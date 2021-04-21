#ifndef __SEND_MOD_MSG__
#define __SEND_MOD_MSG__

#include "modbus_scan.h"

void send_mod_msg(NWLOG_HANDLE_t *logfp, int sock, char unit_id, char function_code, short starting_address_cfg, short number_of_registers_cfg, short count, char *nowday);

#endif
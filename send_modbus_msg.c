#include "modbus_scan.h"

int send_mod_msg(NWLOG_HANDLE_t *logfp, int sock, char unit_id, char function_code, short starting_address_cfg, short number_of_registers_cfg, short count, char *nowday)
{
    char header[12];
    memset(&header[2], 0, 2);

    char send_msg[6];
    memset(send_msg, 0, sizeof(send_msg));
    unsigned short starting_address = starting_address_cfg;

    if (starting_address > 65535 || starting_address < 0)
    {
        NwLOG(logfp, LOG_ERROR, 1, "Over the starting address\n");
        error_handling("starting address must be 0-65535");
    }

    unsigned short number_of_registers = number_of_registers_cfg;
    unsigned short len;

    send_msg[0] = unit_id;
    send_msg[1] = function_code;

    starting_address = reverseByte(starting_address - 1);
    memcpy(&send_msg[2], &starting_address, 2);

    number_of_registers = reverseByte(number_of_registers);
    memcpy(&send_msg[4], &number_of_registers, 2);

    len = sizeof(header);
    len = reverseByte(len);

    count = reverseByte(count);
    memcpy(&header[0], &count, 2);

    memcpy(&header[4], &len, 2);
    memcpy(&header[6], &send_msg, 6);

    if (write(sock, header, sizeof(header)) == -1)
    {
        NwLOG(logfp, LOG_ERROR, 1, "TCP write is failed\n");
        error_handling("write() error");
    }

    Print_send_hexa_log(logfp, header, sizeof(header), nowday);

    return 1;
}
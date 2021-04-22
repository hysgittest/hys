//linux local (main)



#include "modbus_scan.h"

NWLOG_HANDLE_t logfp;

int main(int argc, char *argv[])
{
    NwLOG_Init(&logfp, LOGFILEPATH, "scanner_log", NULL);

    if (argc != 5)
    {
        printf("USAGE : %s <IP> <PORT> <UNIT ID> <PERIOD>\n", argv[0]);

        exit(1);
    }

    cmd2_init();

    modbus_scan(&logfp, atoi(argv[3]), argv[1], atoi(argv[2]));

    return 0;
}

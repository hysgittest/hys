#include "modbus_scan.h"

extern int ms;

extern NWLOG_HANDLE_t logfp;

void time_change(int argc, char *argv[])
{
    if (!strcmp("change", argv[0]))
    {
        if (strcmp("time", argv[1]) == 0)
        {
            ms = atoi(argv[2]);
            NwLOG(&logfp, LOG_DEBUG, 1, "Success Communication Period change(%d)\n", ms);
        }
        else
        {
            printf("NOT exist function!!\n");

            return 0;
        }
    }
}

void cmd2_init()
{
    setvbuf(stdout, NULL, 1, 0);
    setvbuf(stderr, NULL, 1, 0);

    __RunCmdReceiver("SCAN");
    __RegCmdEntry("change", time_change, "Change the TCP Communication Element");

    return;
}
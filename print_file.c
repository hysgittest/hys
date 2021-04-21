#include "modbus_scan.h"

void send_to_file_start(FILE *fpcvs, char *filename)
{
    fpcvs = fopen(filename, "a");

    fprintf(fpcvs, "address\tcoils\tdiscrete input\tholding registers\tinput registers\n");

    fclose(fpcvs);
}

void send_to_file(NWLOG_HANDLE_t *logfp, FILE *fpcvs, char *filename, char *nowtime, short starting_address_cfg, short number_of_registers_cfg, char *bit1, char *bit2, char *word1, char *word2)
{
    fpcvs = fopen(filename, "a");

    fprintf(fpcvs, "[%s]\n", nowtime);
    for (int i = 0; i < number_of_registers_cfg; i++)
    {
        if (bit1[i] == 1 && bit2[i] == 1)
        {
            fprintf(fpcvs, "%d\tTRUE\tTRUE\t%04X\t%04X\n", starting_address_cfg + i, word1[i], word2[i]);
        }
        else if (bit1[i] == 1 && bit2[i] == 0)
        {
            fprintf(fpcvs, "%d\tTRUE\tFALSE\t%04X\t%04X\n", starting_address_cfg + i, word1[i], word2[i]);
        }
        else if (bit1[i] == 0 && bit2[i] == 1)
        {
            fprintf(fpcvs, "%d\tFALSE\tTRUE\t%04X\t%04X\n", starting_address_cfg + i, word1[i], word2[i]);
        }
        else if (bit1[i] == 0 && bit2[i] == 0)
        {
            fprintf(fpcvs, "%d\tFALSE\tFALSE\t%04X\t%04X\n", starting_address_cfg + i, word1[i], word2[i]);
        }
    }

    fclose(fpcvs);
    //printf("*****save to FILE success*****\n");
    NwLOG(logfp, LOG_DEBUG, 1, "save to FILE success\n");
}
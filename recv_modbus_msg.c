#include "modbus_scan.h"

int recv_bit_msg(NWLOG_HANDLE_t *logfp, int sock, char *nowday, short starting_address_cfg, short number_of_registers_cfg, char function_code, short *reg)
{
    char recv_msg[1024];
    int num = 0;
    memset(recv_msg, 0, sizeof(recv_msg));

    int ret = read(sock, recv_msg, sizeof(recv_msg));

    if (ret == -1 || ret == 0)
    {
        error_handling("read() error");
    }

    Print_recv_hexa_log(logfp, recv_msg, ret, nowday);

    if (recv_msg[7] != function_code)
    {
        NwLOG(logfp, LOG_ERROR, 1, "modbus send_msg error\n");
        error_handling("check the modbus send_msg");
    }
    else
    {
        if (number_of_registers_cfg == 1)
        {
            if (recv_msg[9] == 1)
            {
                reg[num] = 1;
                num++;
            }
            else if (recv_msg[9] == 0)
            {
                reg[num] = 0;
                num++;
            }
        }
        else
        {
            int num_10;
            int index_num = recv_msg[8];
            char binary[8];

            memset(&binary, 0, sizeof(binary));

            for (int i = 0; i < index_num; i++)
            {

                int position = 0;

                num_10 = recv_msg[i + 9];

                while (1)
                {
                    binary[position] = num_10 % 2;

                    num_10 = num_10 / 2;

                    position++;

                    if (num_10 == 0)
                    {
                        for (int k = position; k < sizeof(binary); k++)
                        {
                            binary[k] = 0;
                        }
                        break;
                    }
                }
                int total_len = sizeof(binary);
                for (int m = 0; m < total_len; m++)
                {
                    if (binary[m] == 1)
                    {
                        reg[num] = 1;
                        num++;
                    }
                    else if (binary[m] == 0)
                    {
                        reg[num] = 0;
                        num++;
                    }
                }
            }
        }
    }
    num = 0;

    return 1;
}

int recv_word_msg(NWLOG_HANDLE_t *logfp, int sock, char *nowday, short starting_address_cfg, short number_of_registers_cfg, char function_code, short *reg)
{
    char recv_msg[1024];
    int num = 0;
    int ret = read(sock, recv_msg, sizeof(recv_msg));

    if (ret == -1 || ret == 0)
    {
        NwLOG(logfp, LOG_ERROR, 1, "TCP read is failed\n");
        error_handling("read() error");
    }

    Print_recv_hexa_log(logfp, recv_msg, ret, nowday);

    int num_10;
    char binary[16];
    if (recv_msg[7] != function_code)
    {
        NwLOG(logfp, LOG_ERROR, 1, "modbus send_msg error\n");
        error_handling("check the modbus send_msg");
    }
    else
    {
        for (int i = 0; i < (ret - 9); i++)
        {
            if (i % 2 == 0)
            {
                int position = 8;
                int decimal = 0;
                num_10 = recv_msg[i + 9];
                memset(&binary, 0, sizeof(binary));

                while (1)
                {
                    binary[position] = num_10 % 2;
                    num_10 = num_10 / 2;

                    position++;

                    if (num_10 == 0)
                    {
                        break;
                    }
                }

                position = 0;
                for (int k = 0; k < sizeof(binary); k++)
                {
                    if (binary[k] == 1)
                    {
                        decimal += pow(2, k);
                    }
                }

                int totalnum = decimal + recv_msg[i + 10];

                reg[num] = totalnum;
                num++;
            }
        }
    }
    num = 0;

    return 1;
}
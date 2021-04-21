#include "modbus_scan.h"

int ms = 1000;

void modbus_scan(NWLOG_HANDLE_t *logfp, char unitid, char *ipaddress, int portnum)
{
	NwLOG(logfp, LOG_DEBUG, 1, "LOG START\n");

	struct tm *t;
	struct timeb tTime;

	char nowday[11];
	char nowtime[13];

	memset(&nowday, 0, sizeof(nowday));
	memset(&nowtime, 0, sizeof(nowtime));

	ftime(&tTime);
	t = localtime(&tTime.time);
	strftime(nowday, sizeof(nowday), "%Y-%m-%d", t);
	nowday[10] = '\0';

	unsigned short coils[10];
	unsigned short discrete_inputs[10];
	unsigned short holding_registers[10];
	unsigned short input_registers[10];

	memset(coils, 0, sizeof(coils));
	memset(discrete_inputs, 0, sizeof(discrete_inputs));
	memset(holding_registers, 0, sizeof(holding_registers));
	memset(input_registers, 0, sizeof(input_registers));

	unsigned char recv_msg[1024];

	unsigned short count = 1;

	int request_count = 0;
	int response_count = 0;

	unsigned char unit_id = unitid;

	char *ip = malloc(sizeof(ipaddress));
	strcpy(ip, ipaddress);

	int port = portnum;
	int sock = modbus_tcp_connect(logfp, ip, port);

	free(ip);

	char cfgcp[256];
	char cfgcontent[256];
	char cfgname[256];
	unsigned char function_code_cfg;
	unsigned short starting_address_cfg;
	unsigned short number_of_registers_cfg;
	sprintf(cfgname, "%s/%s", CFGFILEPATH, CFGFILE);

	FILE *fp, *fpcvs;
	char filename[256];
	sprintf(filename, "%s/cvss/%s_%s_%d_time.cvs", CFGFILEPATH, nowday, ipaddress, portnum);

	send_to_file_start(fpcvs, filename);

	while (1)
	{
		if ((fp = fopen(cfgname, "r")) == NULL)
		{
			NwLOG(logfp, LOG_ERROR, 1, "config file is not exist\n");
			error_handling("config file error");
		}

		while (fgets(cfgcp, sizeof(cfgcp), fp))
		{
			if (cfgcp[0] == '#')
			{
				continue;
			}

			strcpy(cfgcontent, cfgcp);
			sscanf(cfgcontent, "%hhd %hd %hd", &function_code_cfg, &starting_address_cfg, &number_of_registers_cfg);

			unsigned char function_code = function_code_cfg;

			// function code : 0x01 || 0x02
			if (function_code == 1 || function_code == 2)
			{
				int send_suc = send_mod_msg(logfp, sock, unit_id, function_code, starting_address_cfg, number_of_registers_cfg, count, nowday);

				if (send_suc == 1)
				{
					request_count++;
				}

				int recv_suc;
				if (function_code == 1)
				{
					recv_suc = recv_bit_msg(logfp, sock, nowday, starting_address_cfg, number_of_registers_cfg, 1, coils);
				}
				else if (function_code == 2)
				{
					recv_suc = recv_bit_msg(logfp, sock, nowday, starting_address_cfg, number_of_registers_cfg, 2, discrete_inputs);
				}

				if (recv_suc == 1)
				{
					response_count++;
				}

				if (send_suc == 1 || recv_suc == 1)
				{
					if (function_code == 1)
					{
						//printf("*****coils scanning success*****\n\n");
						NwLOG(logfp, LOG_DEBUG, 1, "coils scanning success\n");
					}
					else if (function_code == 2)
					{
						//printf("*****discrete input scanning success*****\n\n");
						NwLOG(logfp, LOG_DEBUG, 1, "discrete input scanning success\n");
					}
				}

				count++;
			}

			// function code : 0x03 || 0x04
			if (function_code == 3 || function_code == 4)
			{
				int send_suc = send_mod_msg(logfp, sock, unit_id, function_code, starting_address_cfg, number_of_registers_cfg, count, nowday);

				if (send_suc == 1)
				{
					request_count++;
				}

				int recv_suc;
				if (function_code == 3)
				{
					recv_suc = recv_word_msg(logfp, sock, nowday, starting_address_cfg, number_of_registers_cfg, 3, holding_registers);
				}
				else if (function_code == 4)
				{
					recv_suc = recv_word_msg(logfp, sock, nowday, starting_address_cfg, number_of_registers_cfg, 4, input_registers);
				}

				if (recv_suc == 1)
				{
					response_count++;
				}

				if (send_suc == 1 || recv_suc == 1)
				{
					if (function_code == 3)
					{
						//printf("*****holding registers scanning success*****\n\n");
						NwLOG(logfp, LOG_DEBUG, 1, "holding registers scanning success\n");
					}
					else if (function_code == 4)
					{
						//printf("*****input registers scanning success*****\n\n");
						NwLOG(logfp, LOG_DEBUG, 1, "input registers scanning success\n");
					}
				}

				count++;
			}

			ftime(&tTime);
			t = localtime(&tTime.time);
			sprintf(nowtime, "%02d:%02d:%02d.%03d", t->tm_hour, t->tm_min, t->tm_sec, tTime.millitm);
			nowtime[12] = '\0';

			send_to_file(logfp, fpcvs, filename, nowtime, starting_address_cfg, number_of_registers_cfg, coils, discrete_inputs, holding_registers, input_registers);
			//printf("request count : %d\nresponse count : %d\n\n", request_count, response_count);

			NwLOG(logfp, LOG_DEBUG, 1, "[request count] : %d\t[response count] : %d\n", request_count, response_count);

			memset(coils, 0, sizeof(coils));
			memset(discrete_inputs, 0, sizeof(discrete_inputs));
			memset(holding_registers, 0, sizeof(holding_registers));
			memset(input_registers, 0, sizeof(input_registers));

			usleep(ms * 1000);
		}
	}
	fclose(fp);
	close(sock);

	count = 0;
}
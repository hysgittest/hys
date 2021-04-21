#include "modbus_scan.h"

unsigned short reverseByte(unsigned short value)
{
	unsigned short ret = 0;
	((char *)&ret)[0] = ((char *)&value)[1];
	((char *)&ret)[1] = ((char *)&value)[0];
	return ret;
}

void Print_send_hexa_log(NWLOG_HANDLE_t *logfp, char *buff, size_t len, char *nowday)
{
	FILE *fp;

	char filename[1024];
	sprintf(filename, "%s_%s", logfp->szFileName, nowday);
	//printf("%s", filename);
	fp = fopen(filename, "a+");

	size_t i;

	NwLOG(logfp, LOG_DEBUG, 1, "[RSQ] ");
	for (i = 0; i < len; i++)
	{
		fprintf(fp, "%02X ", (unsigned char)buff[i]);
	}
	fprintf(fp, "\n");
	fclose(fp);
}

void Print_recv_hexa_log(NWLOG_HANDLE_t *logfp, char *buff, size_t len, char *nowday)
{
	FILE *fp;

	char filename[1024];
	sprintf(filename, "%s_%s", logfp->szFileName, nowday);
	//printf("%s", filename);
	fp = fopen(filename, "a+");

	size_t i;

	NwLOG(logfp, LOG_DEBUG, 1, "[RSP] ");
	for (i = 0; i < len; i++)
	{

		//NwLOG(logfp, LOG_DEBUG, 1, "%02X ", (unsigned char)buff[i]);
		fprintf(fp, "%02X ", (unsigned char)buff[i]);
	}
	fprintf(fp, "\n");
	//NwLOG(logfp, LOG_DEBUG, 1, "%d\n", (unsigned char)buff[i]);
	fclose(fp);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);

	exit(1);
}
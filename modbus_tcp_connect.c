#include "modbus_scan.h"

int connect_non_block(NWLOG_HANDLE_t *logfp, int sockfd, const struct sockaddr *sock_addr, int socklen, int time)
{
    int flags;
    int conn;
    int error;
    socklen_t len;
    fd_set readfd, writefd;
    struct timeval tv;

    flags = fcntl(sockfd, F_GETFL, 0);

    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    error = 0; // errno 0 : success(작업 성공)

    // connect success
    if ((conn = connect(sockfd, (struct sockaddr *)sock_addr, socklen)) == 0)
    {
        goto end;
    }
    // connect fail
    if (conn < 0)
    {
        if (errno != EINPROGRESS)
        { // errno EINPROGRESS(55) : Operation now in progress(현재 진행중인 작업)
            return -1;
        }
    }
    // if connect fail, non-blocking event
    FD_ZERO(&readfd);
    FD_SET(sockfd, &readfd);
    writefd = readfd;

    tv.tv_sec = 0;
    tv.tv_usec = time;

    // select : 다중 입출력 함수
    /*	select timeout field value			*/
    /*	1) NULL : 신호가 들어올 때 까지 blocking	*/
    /*	2) 0 : non-blocking					*/
    /*	3) not 0 : 지정 시간만큼 blocking		*/

    // select maxfdpl : 최대 fd 번호 + 1(1부터 시작하기 때문)

    /*	select 함수를 사용하여 인자를 받고 timeout 됐을 시 select 함수가 0을 반환		*/
    /*	conn 변수에 select 함수의 반환값을 대입								*/
    /*	해당 값이 timeout value 일 때 -1 반환(connect fail)				*/
    if ((conn = select(sockfd + 1, &readfd, &writefd, NULL, time ? &tv : NULL)) == 0)
    {
        close(sockfd);
        errno = ETIMEDOUT; // errno EIMEDOUT(78) : Connection timed out(연결 시간 초과)
        return -1;
    }

    // FD_ISSET : select 함수 호출 후 반환된 fd가 입출력 준비가 되어있는지 확인하기 위한 함수
    if (FD_ISSET(sockfd, &readfd) || FD_ISSET(sockfd, &writefd))
    {
        len = sizeof(error);
        // socket detail setting => SOL_SOCKET : OPTION PROTOCOL LEVEL / SO_ERROR : ERROR RETURN & CLEAR
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
        {
            return -1;
        }
    }
    else
    {
        NwLOG(logfp, LOG_ERROR, 1, "TCP connect is failed\n");
        error_handling("sockfd is not set");
    }

end:
    if (error)
    {
        close(sockfd);
        errno = error;
        return -1;
    }
    fcntl(sockfd, F_SETFL, flags);
    return 1;
}

int modbus_tcp_connect(NWLOG_HANDLE_t *logfp, char *ip, int port)
{
    int sock;
    struct sockaddr_in sock_addr;
    sock = socket(PF_INET, SOCK_STREAM, 0);

    //printf("%s  ||    %d\n", ip, port);
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(ip);
    sock_addr.sin_port = htons(port);

    if ((connect_non_block(logfp, sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr), 10000)) == -1)
    {
        NwLOG(logfp, LOG_ERROR, 1, "TCP connect is failed\n");
        error_handling("connect error()");
    };

    return sock;
}

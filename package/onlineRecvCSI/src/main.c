/*
 * =====================================================================================
 *       Filename:  main.c
 *
 *    Description:  Here is an example for receiving CSI matrix 
 *                  Basic CSi procesing fucntion is also implemented and called
 *                  Check csi_fun.c for detail of the processing function
 *        Version:  1.0
 *
 *         Author:  Yaxiong Xie
 *         Email :  <xieyaxiongfly@gmail.com>
 *   Organization:  WANDS group @ Nanyang Technological University
 *   
 *   Copyright (c)  WANDS group @ Nanyang Technological University
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <linux/netlink.h>
#include <linux/connector.h>
#include <sys/errno.h>

#include "csi_fun.h"

#define BUFSIZE 4096

#define MAX_PAYLOAD 2048
#define SLOW_MSG_CNT 1

#define SERV_PORT 8090
#define SERV_IP "127.0.0.1"

int serv_port = -1;
char serv_ip[16];

int sock_fd = -1;							// the socket to receive

int sock_cli_fd = -1;							// the socket to send data to server

int quit;
unsigned char buf_addr[BUFSIZE];
unsigned char data_buf[1500];

int         fd;
bool 		log_flag;

COMPLEX csi_matrix[3][3][114];
csi_struct*   csi_status;

void check_usage(int argc, char** argv);
void caught_signal(int sig);

void exit_program(int code);
void exit_program_err(int code, char* func);

int Socket(int, int, int);
void Inet_pton(int , const char *, void *);
void Connect(int, struct sockaddr_in * , int);

/*
void sig_handler(int signo)
{
    if (signo == SIGINT)
        quit = 1;
}
*/
int main(int argc, char* argv[])
{
	unsigned short l, l2;
	struct sockaddr_in servaddr;

    int         i;
    int         total_msg_cnt,cnt;
    unsigned char endian_flag;
    u_int16_t   buf_len;
	

    /* Make sure usage is correct and to use default value when necessary*/
	check_usage(argc, argv);
    
    csi_status = (csi_struct*)malloc(sizeof(csi_struct));

    printf("to connect %s with port %d\n", serv_ip, serv_port);
	
	/* Connect with Internet server*/
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(serv_port);	
	Inet_pton(AF_INET, serv_ip, &servaddr.sin_addr);

	sock_cli_fd = Socket(AF_INET, SOCK_STREAM, 0);
	Connect(sock_cli_fd, &servaddr, sizeof(servaddr));



    fd = open_csi_device();
    if (fd < 0){
        perror("Failed to open the device...");
        return errno;
    }
    
    printf("#Receiving data! Press Ctrl+C to quit!\n");

    quit = 0;
    total_msg_cnt = 0;
	log_flag = 1;

    /* Set up the "caught_signal" function as this program's sig handler */
	signal(SIGINT, caught_signal);
    
    while(1){
        /* keep listening to the kernel and waiting for the csi report */
        cnt = read_csi_buf(buf_addr,fd,BUFSIZE);

        if (cnt){
            total_msg_cnt += 1;

            /* fill the status struct with information about the rx packet */
            record_status(buf_addr, cnt, csi_status);

            /* 
             * fill the payload buffer with the payload
             * fill the CSI matrix with the extracted CSI value
             */
            record_csi_payload(buf_addr, csi_status, data_buf, csi_matrix); 
            
            /* Till now, we store the packet status in the struct csi_status 
             * store the packet payload in the data buffer
             * store the csi matrix in the csi buffer
             * with all those data, we can build our own processing function! 
             */
            //porcess_csi(data_buf, csi_status, csi_matrix);   
            
            printf("Recv %dth msg with rate: 0x%02x | payload len: %d\n",total_msg_cnt,csi_status->rate,csi_status->payload_len);
            
            
			/* Send the data to server */
			buf_len = csi_status->buf_len;
			l = buf_len;
			l2 = htons(l);
			if (write(sock_cli_fd, &l2, sizeof(unsigned short)) < 0)
				exit_program_err(-1, "write");	
			else
				printf("write l2 %d\n",l2);
			if (write(sock_cli_fd, buf_addr, l) < 0)
				exit_program_err(-1, "write");
        }
    }
    close_csi_device(fd);
    free(csi_status);
    return 0;
}

void check_usage(int argc, char** argv)
{
	if (argc < 2)	
	{
		fprintf(stderr, "Usage: %s <Server IP Address>. No file log.\n", argv[0]);	
		exit_program(1);
	}else if(argc == 2)
	{
		serv_port = SERV_PORT;
        strcpy(serv_ip, argv[1]);
	}else if(argc == 3)
	{
		serv_port = atoi(argv[2]);
        strcpy(serv_ip, argv[1]);
	}else
	{
		fprintf(stderr, "Usage: %s <Server IP Address> <Server IP Port> OR %s <Server IP Address>\n", argv[0], argv[0]);
		exit_program(1);
	}
}


int Socket (int family, int type, int protocol)
{
	int n;
	if ( (n=socket(family, type, protocol)) < 0){
		fprintf(stderr, "socket error\n");
		exit_program(1);
	}
	return n;
}

void Inet_pton(int family, const char * ip, void * addptr)
{
	int n;
	if ( (n = inet_pton(family, ip, addptr)) <=0)
	{
		if(n == 0)
			fprintf(stdin, "inet_pton format error : %s\n", ip);
		else
			fprintf(stdin, "inet_pton error : %s\n", ip);
		exit_program(1);
	}
}

void Connect(int fd, struct sockaddr_in * sa, int size)
{
	if ( connect(fd, (struct sockaddr *)sa, size) < 0 )
	{
		fprintf(stderr, "connect error\n");
		exit_program(1);
	}
}

void caught_signal(int sig)
{
	fprintf(stderr, "Caught signal %d\n", sig);
	exit_program(0);
}

void exit_program(int code)
{
	if (sock_fd != -1)
	{
		close(sock_fd);
		sock_fd = -1;
	}

	if(sock_cli_fd != -1)
	{
		close(sock_cli_fd);
		sock_cli_fd = -1;
	}
    close_csi_device(fd);
	exit(code);
}

void exit_program_err(int code, char* func)
{
	perror(func);
	exit_program(code);
}


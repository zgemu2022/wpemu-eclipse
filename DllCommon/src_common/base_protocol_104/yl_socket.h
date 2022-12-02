/*
 * yl_socket.h
 *
 *  Created on: 2018-1-7
 *      Author: root
 */

#ifndef YL_SOCKET_H_
#define YL_SOCKET_H_
#include<netinet/in.h>
/* 类型定义 */
typedef int (*accept_fun)(int, struct sockaddr_in);

enum 
{
    UDP=0,
    TCP=1,
};

typedef struct
{
	char protocol;	
	int fd;
	unsigned short int port; /* Port number; port = htons(PORT) */
    in_addr_t addr; /* Internet address; addr = inet_addr("192.168.0.2"); */
}YL_SERVER_SOCKET;
typedef struct
{
	int fd;
	struct sockaddr_in Addr;
	unsigned short int  tcu_addr;
	pthread_t task_tid;
	void YL_ADDR()
	{
		;
	};
	//YL_SERVER_SOCKET* pserver_sock;
	//void* arg;	//指针, 用于传额外参数, 注意需保证使用过程中指针所分配空间一直存在
}YL_ADDR;

int yl_socket_init_server(YL_SERVER_SOCKET* sock);
int yl_socket_keepalive(int sockfd);
int yl_socket_init(YL_SERVER_SOCKET* sock);
void yl_socket_server_listen(accept_fun pAcceptFun, YL_SERVER_SOCKET server_sock);
int yl_socket_client_init(YL_SERVER_SOCKET* sock);
void yl_socket_server_listen1(YL_SERVER_SOCKET server_sock);
#endif /* YL_SOCKET_H_ */

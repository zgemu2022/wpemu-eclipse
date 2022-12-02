/*
 * yl_socket.cpp
 *
 *  Created on: 2018-1-7
 *      Author: root
 */

#include "yl_socket.h"
#include <sys/socket.h>   // for socket
#include<netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <string.h>
int yl_socket_keepalive(int sockfd)
{
	int keepalive = 1;		// 打开探测
	int keepidle = 60;		// 开始探测前的空闲等待时间
	int keepintvl = 10;		// 发送探测分节的时间间隔
	int keepcnt = 3;		// 发送探测分节的次数

	if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof (keepalive) )< 0)
	{
		perror("fail to set SO_KEEPALIVE");
		return(-1);
	}
	if (setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, (void *) &keepidle, sizeof (keepidle)) < 0)
	{
		perror("fail to set SO_KEEPIDLE");
		return(-1);
	}
	if (setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepintvl, sizeof (keepintvl) )< 0)
	{
		perror("fail to set SO_KEEPINTVL");
		return(-1);
	}
	if (setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcnt, sizeof (keepcnt) )< 0)
	{
		perror("fail to set SO_KEEPALIVE");
		return(-1);
	}
	return 0;
}
int yl_socket_init(YL_SERVER_SOCKET* sock)
{
	printf("\r\n\r\n9999999999999999999999999999999999999999\r\n\r\n");
	printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
	printf("*****  F:%s L:%d\n",__FUNCTION__,__LINE__);
	int iSock;
	int iType, ret;
	struct sockaddr_in ServerAddr;
	int opt = 1;

	if( sock == NULL ) {
		return -1;
	}

	iType = (sock->protocol == UDP) ? SOCK_DGRAM : SOCK_STREAM;
/*   一、获取套接字 */
	iSock = socket( AF_INET, iType, 0 );
	if( iSock < 0 )
	{
		printf("%s:%d socket create fail! \n", __FILE__, __LINE__);
		return -1;
	}
/* 二、准备通信地址 */
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = sock->port;	//监视的端口号
	ServerAddr.sin_addr.s_addr = sock->addr;	//本地IP
	memset( & ( ServerAddr.sin_zero ), 0, sizeof( ServerAddr.sin_zero ) );

		/*三、设置 端口复用*/
		ret = setsockopt(iSock, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt, sizeof(opt));
		if(ret < 0) {
			printf("%s:%d socket setsockopt fail! \n", __FILE__, __LINE__);
	    }
		/*四、绑定*/
		ret = bind( iSock, ( struct sockaddr * )&ServerAddr, sizeof( struct sockaddr ));
		if(ret < 0) {
			printf("%s:%d socket bind fail! \n", __FILE__, __LINE__);
			close(iSock);
			return -1;
	    }

	sock->fd = iSock;
	//printf("iSock:[%d] , sock->fd:[%d] \n", iSock, sock->fd);

	return 0;
}
//int yl_socket_init(YL_SERVER_SOCKET* sock)
//{
//	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
//	//printf("*****  F:%s L:%d\n",__FUNCTION__,__LINE__);
//	int iSock;
//	int iType, ret;
//	struct sockaddr_in ServerAddr;
//	int opt = 1;
//
//	if( sock == NULL ) {
//		printf("n111111111111111111111\r\n");
//		return -1;
//	}
//
//	printf("n33333333333333333333333333\r\n");
//	iType = (sock->protocol == UDP) ? SOCK_DGRAM : SOCK_STREAM;
//	printf("n33333333333333333333333333 iType=%d\r\n",iType);
///*   一、获取套接字 */
//	iSock = socket( AF_INET, iType, 0 );
//	if( iSock < 0 )
//	{
//		printf("%s:%d socket create fail! \n", __FILE__, __LINE__);
//		return -1;
//	}
///* 二、准备通信地址 */
//	ServerAddr.sin_family = AF_INET;
//	ServerAddr.sin_port = sock->port;	//监视的端口号
//	ServerAddr.sin_addr.s_addr = sock->addr;	//本地IP
//	memset( & ( ServerAddr.sin_zero ), 0, sizeof( ServerAddr.sin_zero ) );
//
//		/*三、设置 端口复用*/
//		ret = setsockopt(iSock, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt, sizeof(opt));
//		if(ret < 0) {
//			printf("%s:%d socket setsockopt fail! \n", __FILE__, __LINE__);
//	    }
//		/*四、绑定*/
//		ret = bind( iSock, ( struct sockaddr * )&ServerAddr, sizeof( struct sockaddr ));
//		if(ret < 0) {
//			printf("%s:%d socket bind fail! \n", __FILE__, __LINE__);
//			close(iSock);
//			return -1;
//	    }
//
//	sock->fd = iSock;
//	//printf("iSock:[%d] , sock->fd:[%d] \n", iSock, sock->fd);
//
//	return 0;
//}
void yl_socket_server_listen1(YL_SERVER_SOCKET server_sock)
{
	printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
	struct timeval tv;
	fd_set fdsr;
	struct sockaddr_in ClientAddr;
	int  ifdClientSock;
	int sin_size, ret;

	while (1) {
		tv.tv_sec = 5;
        tv.tv_usec = 0;

		FD_ZERO(&fdsr);
        FD_SET(server_sock.fd, &fdsr);
/*  select   */
		ret = select(server_sock.fd + 1, &fdsr, NULL, NULL, &tv);
        	if (ret < 0) {
            	printf("%s:%d select error:%s!\n", __FUNCTION__, __LINE__, strerror(errno));
				if(errno == EINTR) {
					usleep(100000);	/*  因system interrupt会使select出错, 故等100ms 后再select  */
					continue;
				}
				else {
					FD_CLR(server_sock.fd, &fdsr);
		            return ;
				}
        	}
			else if (ret == 0) {
				printf("%s:%d listen select timeout!\n", __FUNCTION__, __LINE__);
            	continue;
        	}

		if (FD_ISSET(server_sock.fd, &fdsr)) {

			printf("接收了一个新的连接！！！！！！！！！！！！！！！！！！！！！\r\n");
/*  accept  接受连接 */
			ifdClientSock = accept( server_sock.fd, ( struct sockaddr * )&ClientAddr, (socklen_t*)&sin_size );
       		if (ifdClientSock <= 0) {
              		printf("%s:%d accept error:%s!\n", __FUNCTION__, __LINE__, strerror(errno));
				    FD_CLR(server_sock.fd, &fdsr);
              		return ;
       		}


       //     pAcceptFun(ifdClientSock, ClientAddr);	//回调create_new_client
		}
	}
}


void yl_socket_server_listen(accept_fun pAcceptFun, YL_SERVER_SOCKET server_sock)
{
	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
	struct timeval tv;
	fd_set fdsr;
	struct sockaddr_in ClientAddr;
	int  ifdClientSock;
	int sin_size, ret;

	while (1) {
		tv.tv_sec = 5;
        tv.tv_usec = 0;

		FD_ZERO(&fdsr);
        FD_SET(server_sock.fd, &fdsr);
/*  select   */
		ret = select(server_sock.fd + 1, &fdsr, NULL, NULL, &tv);
        	if (ret < 0) {
            	//printf("%s:%d select error:%s!\n", __FUNCTION__, __LINE__, strerror(errno));
				if(errno == EINTR) {
					usleep(100000);	/*  因system interrupt会使select出错, 故等100ms 后再select  */
					continue;
				}
				else {
					FD_CLR(server_sock.fd, &fdsr);
		            return ;
				}
        	}
			else if (ret == 0) {
				printf("%s:%d listen select timeout!\n", __FUNCTION__, __LINE__);
            	continue;
        	}

		if (FD_ISSET(server_sock.fd, &fdsr)) {

			printf("接收了一个新的连接！！！！！！！！！！！！！！！！！！！！！\r\n");
/*  accept  接受连接 */
			ifdClientSock = accept( server_sock.fd, ( struct sockaddr * )&ClientAddr, (socklen_t*)&sin_size );
       		if (ifdClientSock <= 0) {
              		printf("%s:%d accept error:%s!\n", __FUNCTION__, __LINE__, strerror(errno));
				    FD_CLR(server_sock.fd, &fdsr);
              		return ;
       		}
       //     pAcceptFun(ifdClientSock, ClientAddr);	//回调create_new_client
		}
	}
}
int yl_socket_client_connect(int sockfd, struct sockaddr* serv_addr, int timeout)
{
	unsigned long ul = 1;
	int ret = -1;
	int error = -1, len;
	struct timeval tm;
	fd_set set;
	len = sizeof(int);
	printf("使用客户端连接服务器  sockfd=%d",sockfd);
	if( sockfd < 0 || timeout < 0 )
		return -1;
	ioctl(sockfd, FIONBIO, &ul); /*  设置为非阻塞模式*/
	printf("设置为非阻塞模式");
	if( connect(sockfd, /*(struct sockaddr *)&*/serv_addr, sizeof(struct sockaddr_in)) == -1)
	{
		tm.tv_sec = timeout;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(sockfd, &set);
		printf("等待  等待 select");  //comport
		if( select(sockfd+1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
			if(error == 0) {
				printf("连接成功！！！！！");
				ret = 0;
			}
			else {
				close(sockfd);
				printf("对方拒绝  %s:%d socket connect fail error:%s! \n", __FILE__, __LINE__, strerror(error));
				if( error!=ETIMEDOUT ) {
					sleep(timeout);	/* 防止远方拒绝时直接返回, 未等待30miao就跳出了*/
				}
				ret = -1;
			}
		}
		else {
			close(sockfd);
			printf("%s:%d socket connect fail, select error! \n", __FILE__, __LINE__);
			ret = -1;
		}
	}
	else {
		close(sockfd);
		printf("连接套接字失败！！！！！！！！！！！\r\n");
		ret = 0;
	}
	ul = 0;
	printf("查看连接结果   ret=%d\r\n",ret);
	ioctl(sockfd, FIONBIO, &ul); /* 设置为阻塞模式*/
	return ret;
}
int yl_socket_client_init(YL_SERVER_SOCKET* sock)
{
	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
	//printf("*****  F:%s L:%d\n",__FUNCTION__,__LINE__);
   int iSock;
	int iType, ret;
	struct sockaddr_in ServerAddr;
//	int opt = 1;

	if( sock == NULL ) {
		printf("socket 结构为空！！！！");
		return -1;
	}

	printf("建立Socket第一步 获取套接字");
	iType = (sock->protocol == UDP) ? SOCK_DGRAM : SOCK_STREAM;
	printf("建立Socket第一步 获取套接字：选择TCU或UDP  iType=%d\r\n",iType);
/*   一、获取套接字 */
	iSock= socket( AF_INET, iType, 0 );
	if( iSock < 0 )
	{
		printf("%s:%d socket create fail! \n", __FILE__, __LINE__);
		return -1;
	}
/* 二、准备通信地址 */
	printf("建立Socket第二步 准备通信地址");
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = sock->port;	//监视的端口号
	ServerAddr.sin_addr.s_addr = sock->addr;	//本地IP
	memset( & ( ServerAddr.sin_zero ), 0, sizeof( ServerAddr.sin_zero ) );

	ret = yl_socket_client_connect(iSock, (struct sockaddr *)&ServerAddr, 20);	//20秒超时
	if( ret < 0 ) {
		printf("\r\n\r\n111111111111112222222222222222222222233333333333333333333\r\n");
		printf("%s:%d socket connect fail ret < 0 ! \n", __FILE__, __LINE__);
		close(iSock);
		return -1;
	}
	printf("套接字成功连接\r\n");
	sock->fd = iSock;
	return 0;
}



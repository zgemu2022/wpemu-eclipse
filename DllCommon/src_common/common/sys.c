/*
 * sys.c
 *
 *  Created on: 2018-1-22
 *      Author: root
 */

#include "sys.h"
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <pthread.h>
int g_qmeg_key = 10;

int os_create_msgqueue(key_t *key, unsigned char flag)
{
	int qmsgid = -1;
	key_t key_tem;

	if (flag == 0)
		key_tem = *key;
	else
		key_tem = g_qmeg_key + 1;
	while (1)
	{
		qmsgid = msgget(key_tem, IPC_EXCL); /*检查消息队列是否存在*/
		if (qmsgid >= 0 || flag == 0)
		{
			if (flag != 0)
			{
				*key = key_tem;
				g_qmeg_key = key_tem;
			}
			break;
		}
		else
		{
			qmsgid = msgget(key_tem, IPC_CREAT | 0666);
			if (qmsgid < 0)
			{
				printf("failed to create msq | errno=%d [%s]\n", errno, strerror(errno));
			}
			*key = key_tem;
			g_qmeg_key = key_tem;
			break;
		}
	}

	return qmsgid;
}

int os_rev_msgqueue(int __qid, void *__msgp, size_t __msgsz, long int __msgtyp, unsigned int timeout)
{
	int ret_value = -1;
	unsigned int i = 0;
	if (timeout == 0)
		ret_value = msgrcv(__qid, __msgp, __msgsz, __msgtyp, 0);
	else
	{
		for (i = 0; i < timeout; i++)
		{
			ret_value = -1;
			ret_value = msgrcv(__qid, __msgp, __msgsz, __msgtyp, IPC_NOWAIT);
			if (ret_value != -1)
			{
				//				printf("收到数据   !!!!!!!!!!!!!收到 i=%d!\r\n",(int)i);
				break;
			}
			usleep(10000);
		}
	}
	return ret_value;
}

/*******************************************************************************
 * Function Name  : os_create_task
 * Description    : 创建一个任务
 * Input          : (*ptask)(void *arg) 任务执行函数  parg 函数执行参数  psys_arg 任务执行参数
 * Output         : None
 * Return         : 0正确  非0错误
 *******************************************************************************/
unsigned int os_create_task(OS_TaskRet (*ptask)(void *arg1), void *parg, TTaskInfo *psys_arg)
{

	int re;
	pthread_t a_thread;
	pthread_attr_t attr;
	struct sched_param param;
	/*初始化属性值，均设为默认值*/
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	// 设置栈地址和栈大小
	// todo linux去掉栈大小限制
	// pthread_attr_setstack(&attr, psys_arg->Stk, psys_arg->Stk_Size);

	// 设置调度方式和优先级

	pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (psys_arg->Prio != DEFAULT_PRIO)
		param.sched_priority = psys_arg->Prio;
	else
		param.sched_priority = 50;
	pthread_attr_setschedparam(&attr, &param);

	// 创建任务
	// re = pthread_create(&a_thread, &attr, ptask, NULL);
	re = pthread_create(&a_thread, &attr, ptask, parg);

	psys_arg->TaskID = a_thread;

	if (re != 0)
	{
		printf("pthread_create  ID:%d Thread creation failed!!\n", (int)psys_arg->TaskID);
	}
	else
	{
		printf("pthread_create ID:%d Thread Running...!\n", (int)psys_arg->TaskID);
	}
	return re;
}

//************************************************************************//
//*function			:	CreateSettingThread
//*description	:	创建pthread线程
//*input				:	线程标识符，线程属性，线程传入参数，线程绑定状态，线程分离状态
//*output				:
//*return				:	失败返回FAIL
//*others				:
//*notice				:	none
//*author				: feibin
//*date					: 2008-03-28
//************************************************************************//
//*example:
//*CreateSettingThread(&UartReceiveThreadID[j] , &UartReceiveThread_attr[j],(void *) &DjxUartReceiveThread,(void *)  (j+1), 1 , 1);
//
unsigned char CreateSettingThread(pthread_t *threadID,
								  pthread_attr_t *attr,
								  void *(*function),
								  void *arg,
								  unsigned char BindType,	 //线程绑定类型：0---系统默认（非绑定）,1---绑定,2---不绑定
								  unsigned char SeparateType //线程分离状态：0---系统默认（非分离）,1---分离,2---非分离
)
{
	pthread_attr_t *attr_temp;

	attr_temp = attr;

	//取出线程属性--get thread attribute
	pthread_attr_init(attr_temp);
	//设置线程属性
	switch (BindType)
	{
	case 0:
		//系统默认，不进行设置
		break;

	case 1:
		//设置绑定状态
		pthread_attr_setscope(attr_temp, PTHREAD_SCOPE_SYSTEM);
		break;

	case 2:
		//设置非绑定状态
		pthread_attr_setscope(attr_temp, PTHREAD_SCOPE_PROCESS);
		break;

	default:
		//设置默认的绑定状态
		pthread_attr_setscope(attr_temp, PTHREAD_SCOPE_SYSTEM);
		break;
	}

	switch (SeparateType)
	{
	case 0:
		//系统默认，不进行设置
		break;

	case 1:
		//设置分离状态
		pthread_attr_setdetachstate(attr_temp, PTHREAD_CREATE_DETACHED);
		break;

	case 2:
		//设置非分离状态
		pthread_attr_setdetachstate(attr_temp, PTHREAD_CREATE_JOINABLE);
		break;

	default:
		//设置分离状态
		pthread_attr_setdetachstate(attr_temp, PTHREAD_CREATE_DETACHED);
		break;
	}

	//创建线程
	if (0 == pthread_create(threadID, attr_temp, (void *)function, (unsigned char *)arg))
	{
		// DEBUG_PRINTF("create thread successful\n");
		return SUCCESS;
	}
	else
	{
		// SYSERR_PRINTF("create thread failed\n");
		return FAIL;
	}
}

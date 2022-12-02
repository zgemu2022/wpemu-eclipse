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
#include "tools.h"
#include <pthread.h>
int g_qmeg_key=10;
//创建或获得消息队列  flag=0 获得   flag=1创建  需要转换的数据长度
//int os_create_msgqueue(key_t key,u8 flag)
//{
//	int qmsgid=-1;
//	while(1)
//	{
//	   qmsgid = msgget(key,IPC_EXCL );/*检查消息队列是否存在*/
//	   if(qmsgid>=0 || flag==0)
//	   {
//		  break;
//	   }
//	   else
//	   {
//			   qmsgid=msgget(key, IPC_CREAT | 0666);
//	    	   if(qmsgid <0){
//	    		    printf("failed to create msq | errno=%d [%s]\n",errno,strerror(errno));
//	    	   }
//   		      break;
//	    }
//	}
//	return qmsgid;
//}
int os_create_msgqueue(key_t *key,u8 flag)
{
	int qmsgid=-1;
	key_t key_tem;

	if(flag==0)
		key_tem=*key;
	else
		key_tem=g_qmeg_key+1;
	while(1)
	{
	   qmsgid = msgget(key_tem,IPC_EXCL );/*检查消息队列是否存在*/
	   if(qmsgid>=0 || flag==0)
	   {
		   if(flag!=0)
		   {
			   		   *key= key_tem;
			   		g_qmeg_key=key_tem;
		   }
		  break;
	   }
	   else
	   {
			   qmsgid=msgget(key_tem, IPC_CREAT | 0666);
	    	   if(qmsgid <0){
	    		    printf("failed to create msq | errno=%d [%s]\n",errno,strerror(errno));
	    	   }
	    		*key=key_tem;
	    		g_qmeg_key=key_tem;
   		       break;
	    }
	}


	return qmsgid;
}


int os_rev_msgqueue(int __qid, void *__msgp, size_t __msgsz,long int __msgtyp,u32 timeout)
{
   int ret_value=-1;
   u32 i=0;
   int ret;
   if(timeout==0)
	   ret_value = msgrcv(__qid,__msgp,__msgsz,__msgtyp,0);
   else
   {
		for (i = 0; i < timeout; i++) {
			ret_value=-1;
			ret_value = msgrcv(__qid,__msgp,__msgsz,__msgtyp, IPC_NOWAIT);
			if (ret_value !=-1)
			{
//				printf("收到数据   !!!!!!!!!!!!!收到 i=%d!\r\n",(int)i);
				break;
			}
			ret=usleep(10000);
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
u32 os_create_task(OS_TaskRet(*ptask)(void *arg1), void* parg, TTaskInfo* psys_arg)
{

    int re;
    pthread_t a_thread;
    pthread_attr_t attr;
    struct sched_param param;
    ASSERT(ptask != NULL);
    ASSERT(psys_arg != NULL);
    /*初始化属性值，均设为默认值*/
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    // 设置栈地址和栈大小
    //todo linux去掉栈大小限制
    //pthread_attr_setstack(&attr, psys_arg->Stk, psys_arg->Stk_Size);

    // 设置调度方式和优先级

    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(psys_arg->Prio!=DEFAULT_PRIO)
        param.sched_priority = psys_arg->Prio;
   else
    	param.sched_priority = 50;
    pthread_attr_setschedparam(&attr, &param);

    // 创建任务
    //re = pthread_create(&a_thread, &attr, ptask, NULL);
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

/*
 * log.c
 *
 *  Created on: 2017-12-19
 *      Author: root
 */

#include "log.h"
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

//typedef pthread_mutex_t OS_Mutex;
//OS_Mutex LogMutex;
/*******************************************************************************
 * Function Name  : os_mutex_lock
 * Description    : 互斥信号量上锁
 * Input          : mutex 要上锁的信号量   timeout 上锁超时   perr 返回状态
 * Output         : perr 返回状态
 * Return         : None
 *******************************************************************************/
//static u8 os_mutex_lock(OS_Mutex* pmutex)
//{
//    int re = 0;
//
//    ASSERT(pmutex != NULL);
//
//    if((re = pthread_mutex_lock(pmutex)) != 0)
//    {
//        printf("Mutex Lock Fail!! errno %d\n", re);
//    }
//
//    return re;
//}
///*******************************************************************************
// * Function Name  : os_mutex_lock
// * Description    : 互斥信号量解锁
// * Input          : mutex 要解锁的信号量
// * Output         : None
// * Return         : u8 解锁状态
// *******************************************************************************/
//static u8 os_mutex_unlock(OS_Mutex* pmutex)
//{
//    int re = 0;
//
//    ASSERT(pmutex != NULL);
//
//    if((re = pthread_mutex_unlock(pmutex)) != 0)
//    {
//        printf("Mutex unLock Fail!! errno %d\n",re);
//    }
//
//    return re;
//}

/*******************************************************************************
 * Function Name  : load_CurrentDateTime
 * Description    : 加载时间到打印信息中
 * Input          : buf 打印字符串
 * Output         : None
 * Return         : None
 *******************************************************************************/
void load_CurrentDateTime(char *buf)
{
    TDateTime datetime;
    read_current_datetime(&datetime);
    //不加毫秒是21个字节，加上以后是25个字节
    lib_sprintf(buf, "[%04d/%02d/%02d %02d:%02d:%02d.%.3d]", datetime.Year,
            datetime.Month, datetime.Day, datetime.Hour, datetime.Min,
            datetime.Sec, datetime.Msec);
    //*(buf + 21) = 0;
    *(buf + 25) = 0;

}

/*******************************************************************************
 * Function Name  : log_debug
 * Description    : debug级别日志处理
 * Input          : format 带格式信息的打印字符串
 * Output         : None
 * Return         : None
 *******************************************************************************/
void log_debug_new(const char* title,const char *format, ...)  //SOCK_R
{
//	u8 flag_first=1;
    char log_head[40];
    lib_va_list arg = NULL;
 //   printf("888888888888888888888   %s\r\n",title);
//    if(flag_first==1)
//    {
//    	if(pthread_mutex_init(&LogMutex, NULL) ==0)
//    	{
//    		flag_first=0;
//       	}
//
//    }
//    if(flag_first!=0 )
//    	return;
    lib_sprintf(log_head, "%s",title);
    load_CurrentDateTime(log_head + 6);

 //   os_mutex_lock(&LogMutex);

    lib_printf(log_head);

    lib_va_start(arg, format);
	lib_vprintf(format, arg);
    lib_va_end(arg);
//     os_mutex_unlock(&LogMutex);
}

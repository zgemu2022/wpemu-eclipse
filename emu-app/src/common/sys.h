/*
 * sys.h
 *
 *  Created on: 2018-1-8
 *      Author: root
 */

#ifndef SYS_H_
#define SYS_H_
#include <bits/pthreadtypes.h>
#include <sys/ipc.h>
#include "share_type.h"
#include <stddef.h>
#ifdef __cplusplus
extern "C"{
#endif
typedef void* OS_TaskRet;
#define DEFAULT_PRIO        0
typedef struct
{
    int TaskID;
    u8 Prio;
    u32 *Stk;
    u32 Stk_Size;
}TTaskInfo;

int os_create_msgqueue(key_t *key,u8 flag);
//int os_create_msgqueue(key_t key,u8 flag);
int os_rev_msgqueue(int __qid, void *__msgp, size_t __msgsz,long int __msgtyp,u32 timeout);
u32 os_create_task(OS_TaskRet(*ptask)(void *arg), void* parg, TTaskInfo* psys_arg);

#ifdef __cplusplus

}

#endif
#endif /* SYS_H_ */

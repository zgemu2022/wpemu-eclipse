/*
 * log.h
 *
 *  Created on: 2017-12-19
 *      Author: root
 */

#ifndef LOG_MAIN_H_
#define LOG_MAIN_H_

#ifdef __cplusplus

extern "C"{

#endif
#include "com_include.h"
#define LOGCANPATH	"/usr/local/sanway/log/"
#define LOGPMASPATH	"/usr/local/sanway/log/"
#define CAN   1
#define P104  2

#define SEND 1
#define RECV  2

extern void fs_debugcan0(u8 type,u8 ttype,const char *str);
extern void fs_debugcan1(u8 type,u8 ttype,const char *str);
extern void fs_debug(const char *str);


//fs_debug(u8 type,const char *format, ...);
#ifdef __cplusplus

}
#endif

#endif /* LOG_MAIN_H_ */

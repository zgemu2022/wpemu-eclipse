/*
 * app_debug.h
 *
 *  Created on: 2017-12-19
 *      Author: root
 */

#ifndef APP_DEBUG_H_
#define APP_DEBUG_H_


#ifdef __cplusplus

extern "C"{

#endif
#include "log.h"

#define TITLE_MAIN  "[MAIN]"

#define _TCU_MAIN_  1 //与TCU通信CAN口调试
#if(_TCU_MAIN_ == 1)
#define main_debug(x,...) log_debug_new(TITLE_MAIN,"[TCU]"#x"\n", ##__VA_ARGS__)
#else
#define main_debug(x,...)
#endif

#define _TCUCAN_  1 //与TCU通信CAN口调试
#if(_TCUCAN_ == 1)
#define can_debug(x,...) log_debug_new(TITLE_MAIN,"[TCUCAN]"#x"\n", ##__VA_ARGS__)//12345
#else
#define can_debug(x,...)
#endif


#ifdef __cplusplus

}

#endif

#endif /* APP_DEBUG_H_ */




#ifndef PROTOCOL_SCAN_H
#define PROTOCOL_SCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include<unistd.h>
#ifdef __cplusplus

extern "C"{

#endif


//平台安全检查
int protocol_platform_check();
#ifdef __cplusplus

}

#endif
#endif

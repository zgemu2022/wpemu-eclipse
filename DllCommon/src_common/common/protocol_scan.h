#ifndef PROTOCOL_SCAN_H
#define PROTOCOL_SCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include<unistd.h>
typedef enum
{
	   DCA210=1,
	   DCA600 =2,//
	   DCA700 =3,//
	   DCA220 =4,//

}TCU_Type;
//平台安全检查
int protocol_platform_check();

#endif

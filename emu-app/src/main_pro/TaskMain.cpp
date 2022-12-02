/*
 * TaskMain.cpp
 *
 *  Created on: 2020-8-12
 *      Author: root
 */
#include "TaskMain.h"
#include "ffs.h"
#include <stdio.h>
#include "program.h"
#include "sys.h"
#include "log_main.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    void* p;
	int TCU_Type=DAC2800;
	int i;

	printf("argc = %d\n", argc);
	for (i = 0; i < argc; i++)
		printf("argv[%d] is %s\n", i, argv[i]);

	if(argc>1)
		flag_init_lcd= atoi(argv[1]);
	//int ret;
	//TCU_Type=protocol_platform_check();
	printf("测试得到硬件版本=%d\n",TCU_Type);
	FS_Init();
	ReadConf((void*)pconfig);


    pconfig->hardware_ver=TCU_Type;
    pconfig->flag_init_lcd=flag_init_lcd;
	sprintf(_tmp_print_str,"测试得到硬件版本=%d\n",TCU_Type);
	fs_debug(_tmp_print_str);

	p=(void*)pconfig;
	initLCD();
	while(1)
	{

	}
	return 0;
}


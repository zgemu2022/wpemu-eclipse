/*
 * program.cpp
 *
 *  Created on: 2020-8-12
 *      Author: root
 */
#include <stddef.h>
#include <stdio.h>
#include "share_type.h"
#include "lib_time.h"
#include "config-default.h"
#include <ffs.h>
#include <string.h>
#include "app_debug_main.h"
#include "stdlib.h"
#include "program.h"
 #include <dlfcn.h>

#include "cJSON.h"
#include <string>

#include "log_main.h"
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
u32 flag_config=0;
u32 g_fid_conf;
pconf conf;
pconf *pconfig=&conf;
int flag_init_lcd=0;
char _tmp_print_str[128];
cJSON *conf_json;
char workdir_str[1024];
/* Parse text to JSON, then render back to text, and print! */
 static cJSON * doit(char *text)
{
	//char *out;
	cJSON *json;
	json=cJSON_Parse(text);

	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());return NULL;}
	else
	{
		return json;
	}

}

void initLCD(void)
{
   #define LIB_LCD_PATH "/usr/lib/libmodtcp.so"
   typedef int (*p_initlcd)(void*);
	void *handle;
	char *error;

	p_initlcd my_func = NULL;

	handle = dlopen(LIB_LCD_PATH, RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}
	dlerror();

	*(void **) (&my_func) = dlsym(handle, "modbus_tcp_main");


	if ((error = dlerror()) != NULL) {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}

   	my_func((void*)pconfig);


}
void authorize(void)
{
//       #define LIB_AUTH_PATH "/usr/lib/libauthorize.so"
//	   typedef int (*p_authorize)(u8 ,char*);
//		void *handle;
//		char *error;
//		int ret;
//		p_authorize my_func = NULL;
//
//		printf("11111 12345 ssdlh\n");
//
//		handle = dlopen(LIB_AUTH_PATH, RTLD_LAZY);
//		if (!handle) {
//			fprintf(stderr, "%s\n", dlerror());
//			exit(EXIT_FAILURE);
//		}
//		dlerror();
//		*(void **) (&my_func) = dlsym(handle, "createDevCertificate");
//
//		if ((error = dlerror()) != NULL) {
//			fprintf(stderr, "%s\n", error);
//			exit(EXIT_FAILURE);
//		}
//	   	ret = my_func(0,pconfig->devNO);
//
//	   	printf("设备授权证书验证结果 ret =%d\n",ret);
//
//	   	if(ret == 99)//
//	   	{
//		   	printf("软件未获得授权，系统退出   ！！！！！\n");
//
//
//		   	while(1)
//		   	{
//
//		   	}
//	//		exit(EXIT_FAILURE);
//	   	}
//	   	else
//	   	{
//		   	printf("软件获得授权，系统启动  ！！！！！\n");
//	   	   	_callbak_ui_fun(0,2,(void*)2);
//	   	   	_callbak_ui_fun(1,2,(void*)2);
//	   	}
//
//
}
void saveval(pconf *ppconf,int i,int val,char* strval)
{
	TDateTime tm_now;
	read_current_datetime(&tm_now);
	if (strcmp(conf_tab[i].name, "firmid") == 0) {//0
	  ppconf->firm_id = val;
	 } else if (strcmp(conf_tab[i].name, "lcd_server_ip1") == 0) {//4
	  sprintf(&ppconf->lcd_server_ip[0][0], "%s", strval);
	 } else if (strcmp(conf_tab[i].name, "lcd_server_port1") == 0) {//5
	  ppconf->lcd_server_port[0] = val;
	 } else if (strcmp(conf_tab[i].name, "lcd_server_ip2") == 0) {//4
	  sprintf(&ppconf->lcd_server_ip[1][0], "%s", strval);
	 } else if (strcmp(conf_tab[i].name, "lcd_server_port2") == 0) {//5
	  ppconf->lcd_server_port[1] = val;
	 } else if (strcmp(conf_tab[i].name, "lcd_server_ip3") == 0) {//4
	  sprintf(&ppconf->lcd_server_ip[2][0], "%s", strval);
	 } else if (strcmp(conf_tab[i].name, "lcd_server_port3") == 0) {//5
	  ppconf->lcd_server_port[2] = val;
	 } else if (strcmp(conf_tab[i].name, "lcd_server_ip4") == 0) {//4
	  sprintf(&ppconf->lcd_server_ip[3][0], "%s", strval);
	 } else if (strcmp(conf_tab[i].name, "lcd_server_port4") == 0) {//5
	  ppconf->lcd_server_port[3] = val;
	 } else if (strcmp(conf_tab[i].name, "lcd_server_ip5") == 0) {//4
	  sprintf(&ppconf->lcd_server_ip[4][0], "%s", strval);
	 } else if (strcmp(conf_tab[i].name, "lcd_server_port5") == 0) {//5
	  ppconf->lcd_server_port[4] = val;
	 } else if (strcmp(conf_tab[i].name, "lcd_server_ip6") == 0) {//4
	  sprintf(&ppconf->lcd_server_ip[5][0], "%s", strval);
	 } else if (strcmp(conf_tab[i].name, "lcd_server_port6") == 0) {//5
	  ppconf->lcd_server_port[5] = val;


	 } else if (strcmp(conf_tab[i].name, "lcd_num") == 0) {//1
	  ppconf->lcd_num = val;
	 } else if (strcmp(conf_tab[i].name, "pcs_num") == 0) {
		 printf("pcs总数:%d\n",val);//2
	    ppconf->pcs_num = val;
	 }else if (strcmp(conf_tab[i].name, "plc_server_ip") == 0) {//4
		 sprintf(&ppconf->plc_server_ip[0], "%s", strval);
	 }else if (strcmp(conf_tab[i].name, "plc_server_port") == 0) {//5
		 printf("plc_server_port:%d\n",val);//2
		  ppconf->plc_server_port= val;

	 } else if (strcmp(conf_tab[i].name, "balance_rate") == 0) {
		 printf("均衡速率:%d\n",val);//2
	  ppconf->balance_rate = val;
	 } else if (strcmp(conf_tab[i].name, "sys_max_pw") == 0) {
		 printf("系统定义最大功率:%d\n",val);//2
	  ppconf->sys_max_pw = val;
	}else if (strcmp(conf_tab[i].name, "LOGENABLE") == 0) {//24
		printf("tcu是否支持将协议日志写入flash  =%d\r\n ",val);//
		ppconf->iflog=val;
	}else if (strcmp(conf_tab[i].name, "HARDWARE_VER") == 0) {//25
		printf("tcu硬件版本号  =%d\r\n ",val);//
		ppconf->hardware_ver=val;
	}
	else if (strcmp(conf_tab[i].name, "START_LOG") == 0) {//27
		char strx[7];
		char b1,b2;

		sprintf(strx, "%s\n", strval);
		b1=strx[4]-'0';
		b2=strx[5]-'0';
		ppconf->StartLogDay.Day=(b1*10)+b2;

		b1=strx[2]-'0';
		b2=strx[3]-'0';
		ppconf->StartLogDay.Month=(b1*10)+b2;
		b1=strx[0]-'0';
		b2=strx[1]-'0';
		ppconf->StartLogDay.Year=2000+b1*10+b2;

		ppconf->StartLogDay.Hour=0;
		ppconf->StartLogDay.Min=0;
		ppconf->StartLogDay.Sec=0;

		//if(cmp_DateTime(&tm_now,&pconfig->EndLogDay)>0)
		printf("日志开始记录日期  year=%d  month=%d  day=%d\n",ppconf->StartLogDay.Year,ppconf->StartLogDay.Month,ppconf->StartLogDay.Day);
	}else if (strcmp(conf_tab[i].name, "LOG_DAYS") == 0) {//28
		ppconf->logdays=val;
		printf("日志开始记录天数=%d\n",ppconf->logdays);
	}else if (strcmp(conf_tab[i].name, "MAIN_VER") == 0) {//29

		sprintf(ppconf->main_ver, "%s\n", strval);
		printf("tcu主控版本号  =%s\r\n ",ppconf->main_ver);//tcu主控版本号
	}

}


int matchkey_json(void * para1, cJSON *item) {
	int num;
	int i;

	int match_flag = 0;
	static int log_flag = 0;
	TDateTime tm_now; //, EndLogDay;
	num = ARRAY_LEN(conf_tab);
	read_current_datetime(&tm_now);

//	main_debug("从文件读出的key字符串：%s", item->string);
	for (i = 0; i < num; i++) {
//        if(i==4)
		//	main_debug("读出的字符串：%s  %s", item->string, conf_tab[i].name);
		if (strcmp(item->string, conf_tab[i].name) == 0) {
			main_debug("读出的字符串：%s  %s", item->string, conf_tab[i].name);

			if (item->type == cJSON_String && conf_tab[i].typeVal == TY_STR) {
				if (strcmp(item->string, "START_LOG") == 0) {
					if (log_flag == 1) {
						sprintf(conf_tab[i].val, "%02d%02d%02d",
								tm_now.Year - 2000, tm_now.Month, tm_now.Day);
						printf("版本号改变，修改日志纪录开始时间为当前时间 %s \n", conf_tab[i].val);
						match_flag = 1;
					} else {
						strcpy(conf_tab[i].val, item->valuestring);
						printf("版本号没变，无需改变日志纪录开始时间\n");
					}
				} else if (strcmp(item->string, "MAIN_VER") == 0) {
					strcpy(conf_tab[i].val, MAIN_VER_NEW);
					if (strcmp(item->valuestring, MAIN_VER_NEW)) {
						printf("版本号发生变化 old= %s  new=%s\n", item->valuestring,
						MAIN_VER_NEW);
						log_flag = 1;
						match_flag = 1;
					}
				} else if (strcmp(conf_tab[i].val, item->valuestring)) {
					if (strcmp(item->string, "START_LOG")
							&& strcmp(item->string, "MAIN_VER")) {
						printf("此项使用配置文件  i=%d  item->string=%s \n", i,
								item->string);
						strcpy(conf_tab[i].val, item->valuestring);
					}
				}
			} else if ((item->type == cJSON_Number)
					&& (conf_tab[i].typeVal == TY_INT)) {
				if (strcmp(item->string, "LOGENABLE") == 0) {
					if (log_flag == 1 && (atoi(conf_tab[i].val) == 0)) {
						sprintf(conf_tab[i].val, "%d", 1);
						match_flag = 1;
					} else {
						sprintf(conf_tab[i].val, "%d", item->valueint);
					}
				} else if (strcmp(item->string, "LOGENABLE")
						&& atoi(conf_tab[i].val) != item->valueint) {
					printf("此项使用配置文件  i=%d  item->valueint=%d \n", i,
							item->valueint);
					sprintf(conf_tab[i].val, "%d", item->valueint);
				}

			} else {
				match_flag = 1;
			}

			if (match_flag == 0) {
				main_debug("字符串匹配成功，并且值未发生改变：i=%d", i);
				flag_config &= ~(1 << i);
			} else
				main_debug("字符串匹配成功，值发生改变：i=%d", i);

			return i;
		}
	}
	return -1;

}

int myjson(void* para,cJSON *item)
{
	cJSON *child=item->child;
	int num;

	int numentries=0;
	while (child) numentries++,child=child->next;

	printf("numentries=%d \n",numentries);

	child=item->child;
	while (child)
	{
		printf("%s :",(char*)child->string);
	//	(char*)child->string,child->valueint
	   num = matchkey_json(para, child);
		if(child->type==cJSON_String)
		{
            printf("%s\n",child->valuestring);
		}
		else if(child->type==cJSON_Number)
		{
            printf("%d\n",child->valueint);
		}
		else
			printf("err err err!!!!!!!!!!!");

		child=child->next;
	}
	return numentries;
}

void create_objects()
{
	cJSON *root;
    char *out;
	int i;
    int num=ARRAY_LEN(conf_tab);
	root=cJSON_CreateObject();
    for(i=0;i<num;i++)
    {
    	if(conf_tab[i].typeVal==TY_INT)
    		cJSON_AddNumberToObject(root,conf_tab[i].name,atoi(conf_tab[i].val));
    	else
    	   cJSON_AddItemToObject(root, conf_tab[i].name,  cJSON_CreateString(conf_tab[i].val));
    }
	out=cJSON_Print(root);

	cJSON_Delete(root);
	printf("%s\n",out);

    fs_open(FILE_ID_TCU_CONF,"w+");

	fs_write(FILE_ID_TCU_CONF, out, strlen(out));
	fs_close(FILE_ID_TCU_CONF);
	free(out);

}


int ReadConf(void *para)
{
	int ret=-1;
    int i=0;
	TDateTime tm_now;//,EndLogDay;
	int len;
	char pDestBuf[1024];
    int num=ARRAY_LEN(conf_tab);
    int num_json;
    cJSON * pjson;

  //  char temp_str_last[64];
    flag_config=0;
	for(i=0;i<num;i++)
	{
		flag_config|=(1<<i);
	}
    printf("配置文件的条目数据=%d  %x\n",num,(int)flag_config);
    g_fid_conf=0;
    g_fid_conf=fs_registerFile(g_fid_conf,FILE_NAME_TCU_CONF,FILE_SIZEOF_TCU_CONF);
	ret=createStrfile(g_fid_conf);
	printf("建立或打开配置文件  ret=%d\n",ret);
	if (ret == 0) {
		  len=app_fs_read_all(g_fid_conf,  (u8*)&pDestBuf[0]);
		  printf("%s\n",pDestBuf);
		  pjson = doit(pDestBuf);
		if (pjson) {
			num_json=myjson(para, pjson);
			cJSON_Delete(pjson);

			if (flag_config == 0 && num_json==num) {
				printf("配置文件未发生变动，关闭配置文件\n");

			}
		}

	}
	else
	{
		read_current_datetime(&tm_now);
		sprintf(conf_tab[num-1].val, "%02d%02d%02d",
					tm_now.Year - 2000, tm_now.Month, tm_now.Day);
		strcpy(conf_tab[num-2].val, MAIN_VER_NEW);

		printf("生成新的配置文件，使用当前程序版本号：%s  日志纪录开始时间为当前时间 %s \n",conf_tab[num-1].val, conf_tab[num].val);
	}
	for(i=0;i<num;i++)
	{

			 saveval((pconf *)para, i, atoi(conf_tab[i].val),conf_tab[i].val);
	}
	fs_close(FILE_ID_TCU_CONF);
	if(flag_config!=0 ||  (num_json!=num) )
	{
		printf("配置文件发生变动，重写配置文件\n");
		create_objects();
	}
	return 0;
}
int ModiLogMark(void)
{
	int ret=-1,ret1=-1;
    char temp_str[64];
    char* ptemp=NULL;
  //  char temp_str_last[64];
    int lenlen=0,lenlen_last;//,lenlen_last_last;
    int i=0;
    int flag=0;

	ret=createStrfile(FILE_ID_TCU_CONF);
	main_debug("ModiLogMark建立或打开配置文件  ret=%d %s",ret,FILE_NAME_TCU_CONF);
	if (ret == 0) {
		while (1) {

	//		main_debug("配置文件已经到尾部?????  %s  %s", temp_str_last, temp_str);
			ret1 = parse_config(FILE_ID_TCU_CONF, temp_str);

			if (ret1 != 0) {
				main_debug("配置文件已经到尾部！！！！");
				fs_close(FILE_ID_TCU_CONF);
				break;
			}
			lenlen_last = lenlen;
			ptemp = strstr(temp_str, "LOGENABLE"); //conf_tab[i].name
			if (ptemp != NULL) {
				main_debug("配置文件第%d条正确 ！！！！lenstr=%d  str=%s", i,
						sizeof(temp_str), temp_str);
				flag = 1;
				break;

			}
			lenlen += strlen(temp_str);
			i++;

		}
	}
	else
	{
		fs_close(FILE_ID_TCU_CONF);
		return -1;
	}
	fs_close(FILE_ID_TCU_CONF);
	if (flag == 1) {
		fs_open(FILE_ID_TCU_CONF, "r+");
		fs_lseek(FILE_ID_TCU_CONF, lenlen_last, SEEK_SET);
		sprintf(temp_str, "\t\"LOGENABLE\":    0,\n");
		fs_close(FILE_ID_TCU_CONF);
		return 1;

	}
	return 0;
}
void pr_currendir(void)
{
        /* 使用getcwd获取启动目录 */
	if (NULL == getcwd(workdir_str, sizeof(workdir_str))) {
		fprintf(stderr, "getcwd error: %s\n", strerror(errno));
		//exit(1);
	}
	printf("current work path: %s\n", workdir_str);

}


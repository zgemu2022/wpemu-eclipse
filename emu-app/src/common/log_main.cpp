/*
 * log.c
 *
 *  Created on: 2017-12-19
 *      Author: root
 */

#include "log_main.h"
#include "log.h"
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "program.h"
#include "log_main.h"
FILE *canlogfp = NULL;
FILE *pmaslogfp = NULL;


FILE*  __is_new_file(u8 flagfirst,const char* newfilename,const char*  __dirname)
{
	DIR *pdir = NULL;
	FILE* fp;
	pdir = opendir(__dirname);
	printf("__is_new_file __is_new_file \r\n");
  //  if(flagfirst==1)
    {
    	if(!pdir){
    		mkdir(__dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    		pdir = opendir(__dirname);
    	}
    	closedir(pdir);
     	fp = fopen(newfilename, "a+");//"w+");
     	if(fp!=NULL)
     	{

     		printf("can 日志文件%s打开成功！！！！",newfilename);  //fs_debug44444444444444444444444444

     	}
     	else
     	{
     		fp=NULL;
     		printf("can 日志文件%s打开失败！！！！\r\n",newfilename);
     	}
    }
//    else{
//        printf("123 12345 12345111\r\n");
//	    fclose(fp);
//     	fp = fopen(newfilename, "w+");
//	}
    return fp;
}
void __log_file_manage(const char*  __dirname,u16 len)
{
	DIR *pdir = NULL;
	struct dirent *prent = NULL;
	int logFileNameLen = 0;
	int lognum = 0, logTime = 0, leastLogTime = 0;
	int firstQuery = 0;
	char leastFileName[64] = {0};

	pdir = opendir(__dirname);
	if(!pdir){
		return;
	}
    printf("fs_debug44444444444444444444444444 %s\r\n",__dirname);
	while((prent = readdir(pdir)) != NULL){
		if(prent->d_type & DT_REG){
			logFileNameLen = strlen(prent->d_name);
			printf("111日志名称=%s len=%d\r\n",prent->d_name,logFileNameLen);
			if(logFileNameLen == len){
				//if((strstr(prent->d_name, "can") != NULL) && (strstr(prent->d_name, ".log") != NULL)){
				if((strncmp(prent->d_name, "pmas", 4) == 0) && (strstr(prent->d_name, ".log") != NULL)){
				    printf("fs_debug55555555555555555555555 %s\r\n",__dirname);

					logTime = atoi(prent->d_name+4);
					if(firstQuery == 0){
						leastLogTime = logTime;
						firstQuery = 1;
					}
					if(logTime < leastLogTime)
					{
						leastLogTime = logTime;
					}
					lognum++;
				}
			}
			}
		}
		sprintf(leastFileName, "%spmas%d.log", __dirname, leastLogTime);//

		printf("日志名称leastFileName=%s lognum=%d\r\n",leastFileName,lognum);
		if(lognum > 9){
			printf("fail fail fail fail fail \r\n");
			remove(leastFileName);
		}
		closedir(pdir);
		return;
	}



void fs_debugcan0(u8 type,u8 ttype,const char *str)
{
	static u8 flagfirst=1;
    char log_head[64];
  //  lib_va_list arg = NULL;
	char newfilename[64] = {0};
	FILE *fp;
	time_t now;
	struct tm *ptm = NULL;
	return;

//	if(pconfig->iflog==0)
//	{
//		return ;
//	}
	time(&now);
	ptm = localtime(&now);
	 if(ttype==SEND)
     lib_sprintf(log_head, "%s", "[SEND]");
	 else
	  lib_sprintf(log_head, "%s", "[RECV]");
	 fp=canlogfp;
		 sprintf(newfilename, "%scan%04d%02d%02d%02d.log", LOGCANPATH, ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour);
	     if(flagfirst==1 || (ptm->tm_min == 0 && ptm->tm_sec == 0))
		    {
			   fp= __is_new_file(flagfirst,newfilename,LOGCANPATH);
			    flagfirst=0;
	    	    __log_file_manage(LOGCANPATH,19);
			    canlogfp=fp;
		    }
    load_CurrentDateTime(log_head + 6);
     fprintf(fp,log_head);
     fprintf(fp,str);
	fflush(fp);
	return ;
}

void fs_debugcan1(u8 type,u8 ttype,const char *str)
{
	static u8 flagfirst=1;
    char log_head[64];
  //  lib_va_list arg = NULL;
	char newfilename[64] = {0};
	FILE *fp;
	time_t now;
	struct tm *ptm = NULL;
	return;
//	if(pconfig->iflog==0)
//	{
//		return ;
//	}
	time(&now);
	ptm = localtime(&now);
	 if(ttype==SEND)
     lib_sprintf(log_head, "%s", "[SEND]");
	 else
	  lib_sprintf(log_head, "%s", "[RECV]");//4+4+2+2+2+1+3
	 fp=canlogfp;
		 sprintf(newfilename, "%scan1%04d%02d%02d%02d.log", LOGCANPATH, ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour);
	     if(flagfirst==1 || (ptm->tm_min == 0 && ptm->tm_sec == 0))
		    {
			   fp= __is_new_file(flagfirst,newfilename,LOGCANPATH);
			    flagfirst=0;
	    	    __log_file_manage(LOGCANPATH,19);
			    canlogfp=fp;
		    }
    load_CurrentDateTime(log_head + 6);
     fprintf(fp,log_head);
     fprintf(fp,str);
	fflush(fp);
	return ;
}



void fs_debug(const char *str1)
{
	static u8 flagfirst=1;
    char log_head[64];
    char str[128];
    memcpy(str,str1,128);
  //  lib_va_list arg = NULL;
	char newfilename[64] = {0};
	FILE *fp;
	time_t now;
	TDateTime tm_now,EndLogDay;
	struct tm *ptm = NULL;
	if(pconfig->iflog==0)
	{
		return ;
	}
	EndLogDay=pconfig->StartLogDay;
	add_DateTime_Day(&EndLogDay,pconfig->logdays);
	printf("日志开始记录日期  year=%d  month=%d  day=%d  天数=%d\n",EndLogDay.Year,EndLogDay.Month,EndLogDay.Day,pconfig->logdays);
	printf("日志结束记录日期  year=%d  month=%d  day=%d\n",EndLogDay.Year,EndLogDay.Month,EndLogDay.Day);

	read_current_datetime(&tm_now);
	if(cmp_DateTime(&tm_now,&pconfig->StartLogDay)<0)
	{
		     printf("日志还未到记录日期\n");
             return;
	}
	if(cmp_DateTime(&tm_now,&EndLogDay)>0)
	{
		  printf("111日志超过记录日期\n");
		  pconfig->iflog=0;
		  ModiLogMark();


             return;
	}
	time(&now);
	ptm = localtime(&now);

    //os_mutex_lock(&LogMutex);
//	if(cmp_DateTime(num,xx)<=0)
//	{
//
//	}

	  lib_sprintf(log_head, "%s", "[SYC]");
//这里加编译
		fp=pmaslogfp;
			sprintf(newfilename, "%spmas%04d%02d%02d.log", LOGPMASPATH, ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday);
		    if((fp==NULL) || ((ptm->tm_min == 0) && (ptm->tm_hour == 0)))// ||
		    {

		    	fp=__is_new_file(flagfirst,newfilename,LOGPMASPATH);
			    __log_file_manage(LOGPMASPATH,16);
			    pmaslogfp=fp;

		    }
		    else if(flagfirst==1)
		    {
		    	printf("第一次打开日志！！！\n");
		    	fp=__is_new_file(flagfirst,newfilename,LOGPMASPATH);
			    __log_file_manage(LOGPMASPATH,16);
			    pmaslogfp=fp;
			    flagfirst=0;
		    }

    load_CurrentDateTime(log_head + 6);
     fprintf(fp,log_head);
     fprintf(fp,str);
	fflush(fp);
	return ;
}












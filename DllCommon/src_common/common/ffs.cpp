/*
 * fs.cpp
 *
 *  Created on: 2018-3-27
 *      Author: root
 */

#include "ffs.h"
#include <sys/time.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
TFileInfo FileInfo[MAX_FILE_SUPPORT];
sem_t sem_readfs;
u32 g_fid=0;
/******************************************************************************
 * Function Name  : fs_registerFile
 * Description    : 在文件系统中注册文件
 * Input          : file_id 文件ID  file_name 文件名  store_size 存储大小
 * Output         : None
 * Return         : s32 注册后的文件ID
 *******************************************************************************/
s32 fs_registerFile(u32 file_id, const char *file_name, u32 store_size)
{
//	static u32 fid=0;
	u32 i;

	for(i=0;i<g_fid;i++)
	{
		if(strcmp(file_name,FileInfo[i].Name)==0)
			return i;

	}
	file_id=g_fid;
	g_fid++;


    if(file_id+1 > MAX_FILE_SUPPORT)
    {
        return -1;
    }

    if((strlen(file_name) + strlen(FS_DAT_PATH) )> MAX_FILENAME_LEN)
    {
    //    printf("fs_registerFile: file name[%s] is too long! Only 8 bytes!", file_name);
        return -1;
    }

    FileInfo[file_id].Valid = False;
    FileInfo[file_id].ID = file_id;

    FileInfo[file_id].Name[0] = 0;//置为空字符串

    sprintf(FileInfo[file_id].Name,"%s/%s",FS_DAT_PATH,file_name);//"date -s \"20%02d-%02d-%02d %02d:%02d:%02d\"", year, mon, day, hour, min, ms/1000);
   // str=FS_DAT_PATH+file_name;
//    memcpy((char *)FileInfo[file_id].Name, FS_DAT_PATH,strlen(FS_DAT_PATH));
//
//    memcpy((char *)FileInfo[file_id].Name, file_name,strlen(file_name));

    printf("生成的文件名：%s\r\n",FileInfo[file_id].Name);//初始化得到的设置的时段数
    FileInfo[file_id].Pointer = 0;
    FileInfo[file_id].Size = store_size;

    return file_id;
}

/******************************************************************************
 * Function Name  : fs_open
 * Description    : 打开文件
 * Input          : file_id 文件ID  oflag 打开属性  mode 文件存取模式
 * Output         : None
 * Return         : s32 文件描述符FID（Linux下FID为文件描述符）
 *******************************************************************************/
FILE * fs_open(u32 file_id, const char *mode)
{
	// 大小为0说明不需要该文件
	if (FileInfo[file_id].Size == 0) {
		FileInfo[file_id].Valid = False;
		return NULL;
	}

	printf("文件系统打开的文件名 %s  %d\r\n",FileInfo[file_id].Name,FileInfo[file_id].Valid);
	if (!FileInfo[file_id].Valid) {
		FileInfo[file_id].fp= fopen(FileInfo[file_id].Name, mode);
		FileInfo[file_id].Valid = True;
		FileInfo[file_id].Pointer = 0;
	}
	return FileInfo[file_id].fp;
}

/******************************************************************************
 * Function Name  : fs_chmod
 * Description    : 设置文件权限
 * Input          : file_id 文件ID  mode 权限模式
 * Output         : None
 * Return         : s32 权限设置结果
 *******************************************************************************/
s32 fs_chmod(u32 file_id, u32 mode)
{
    s32 re = 0;

    if(FileInfo[file_id].Valid){
           re = chmod(FileInfo[file_id].Name, mode);
        FileInfo[file_id].Pointer = 0;
    }
    return re;
}
/******************************************************************************
 * Function Name  : fs_rm
 * Description    : 删除文件
 * Input          : file_id 文件ID
 * Output         : None
 * Return         : 
 *******************************************************************************/
s32 fs_rm(u32 file_id)
{
    s32 re = -1;
    re=unlink(FileInfo[file_id].Name);
    rmdir(FileInfo[file_id].Name);
    return re;
}


/******************************************************************************
 * Function Name  : fs_lseek
 * Description    : 偏移文件
 * Input          : file_id 文件ID offset 偏移量  whence 移动方式
 * Output         : None
 * Return         : s32 移动情况
 *******************************************************************************/
s32 fs_lseek(u32 file_id, u32 offset, s32 whence){
    s32 re = -1;
    if(FileInfo[file_id].Valid){
       re = fseek(FileInfo[file_id].fp, offset, whence);
        switch(whence){
            case SEEK_SET:
            // 绝对偏移
            FileInfo[file_id].Pointer = offset;
            break;
            case SEEK_CUR:
            // 相对偏移
            FileInfo[file_id].Pointer += offset;
            break;
            case SEEK_END:
            // 相对偏移
            FileInfo[file_id].Pointer = FileInfo[file_id].Size - offset - 1;
            break;
        }
    }

    return re;
}
/******************************************************************************
 * Function Name  : fs_read
 * Description    : 从文件读入数据
 * Input          : file_id 文件ID  pbuf 需写入数据  len 数据长度
 * Output         : None
 * Return         : s32 读取长度
 *******************************************************************************/
int fs_read(u32 file_id, void *pbuf, u32 len)
{
	int re = -1;
	if (FileInfo[file_id].Valid) {
		re = fread(pbuf, 1,len,FileInfo[file_id].fp);
		FileInfo[file_id].Pointer += len;
	}

	return re;
}


/******************************************************************************
 * Function Name  : fs_close
 * Description    : 关闭文件
 * Input          : file_id 文件ID
 * Output         : None
 * Return         : s32 关闭情况
 *******************************************************************************/
int fs_close(u32 file_id)
{
    int re = -1;
    if(FileInfo[file_id].Valid){
         re =fclose(FileInfo[file_id].fp);
         FileInfo[file_id].Valid = False;
         FileInfo[file_id].Pointer = 0;
    }

    return re;
}
//=====================提供给应用层的函数，尽量简化用法==============================
//应用层读文件
//fid文件id offset偏移量
//pDestBuf要写入的数据 len要写的长度
int app_fs_read(u32 fid, u32 offset, u32 read_len, u8* pDestBuf)
{
     s8 s;
     struct timespec ts;
	 int count=0;
	 FILE *p=NULL;
     gettimeofday((struct timeval *)&ts, NULL);
	//    tmp =
	 gmtime(&ts.tv_sec);
 //    ts.tv_nsec +=500;
	 ts.tv_sec+=2;
     s = sem_timedwait(&sem_readfs, &ts);//阻塞2秒（不精确）
     p= fs_open(fid,"r");
     if(p==NULL)
     {
         FileInfo[fid].Valid = False;
    	  if(s==0)
    		  sem_post(&sem_readfs);
    	 printf("文件读取失败!!!!\r\n");
    	 return 0;
     }
       fs_lseek(fid, offset, SEEK_SET);
        count = fs_read(fid, pDestBuf , read_len);

    fs_close(fid);

    if(s==0)
	  sem_post(&sem_readfs);

    return count;
}

//fid文件id offset偏移量
//pDestBuf要的数据 len要写的长度
int app_fs_read_all(u32 fid, u8* pDestBuf)
{
     s8 s;
     struct timespec ts;
	 int count=0,read_len=0;
	 FILE *p=NULL;
     gettimeofday((struct timeval *)&ts, NULL);
	//    tmp =
	 gmtime(&ts.tv_sec);
 //    ts.tv_nsec +=500;
	 ts.tv_sec+=2;
     s = sem_timedwait(&sem_readfs, &ts);//阻塞2秒（不精确）
     p= fs_open(fid,"r");
     if(p==NULL)
     {
         FileInfo[fid].Valid = False;
    	  if(s==0)
    		  sem_post(&sem_readfs);
    	 printf("文件读取失败!!!!\r\n");
    	 return 0;
     }
       fs_lseek(fid, 0, SEEK_END);
       read_len=ftell(FileInfo[fid].fp);
       fs_lseek(fid, 0, SEEK_SET);
      count = fs_read(fid, pDestBuf , read_len);
   	 printf("文件读取长度=%d  %d !!!!\r\n",read_len,count);
    fs_close(fid);

    if(s==0)
	  sem_post(&sem_readfs);

    return count;
}
/******************************************************************************
 * Function Name  : fs_write
 * Description    : 向文件写入数据
 * Input          : file_id 文件ID  pbuf 需写入数据  len 数据长度
 * Output         : None
 * Return         : s32 写入长度
 *******************************************************************************/
int fs_write(u32 file_id, const void *pbuf, u32 len)
{
    s32 re = -1;
    if(FileInfo[file_id].Valid){
    //	write(fd, buf, nbyte);
        re = fwrite( pbuf, 1,len,FileInfo[file_id].fp);
        FileInfo[file_id].Pointer += len;
    }
    return re;
}
/******************************************************************************
 * Function Name  : fs_write_null
 * Description    : 不向文件写入数据，只移动指针
 * Input          : file_id 文件ID  pbuf 需写入数据  len 数据长度
 * Output         : None
 * Return         : s32 写入长度
 *******************************************************************************/
int fs_write_null(u32 file_id, const void *pbuf, u32 len)
{
 //   s32 re = -1;
    if(FileInfo[file_id].Valid){
    //	write(fd, buf, nbyte);
  //      re = fwrite( pbuf, 1,len,FileInfo[file_id].fp);
        FileInfo[file_id].Pointer += len;
 //       re=0;
    }
    return 0;
}
//应用层写文件，简化用法
//fid文件id offset偏移量
//pDestBuf要写入的数据 len要写的长度
int app_fs_write(u32 fid, u32 offset, u32 write_len, u8* pSourceBuf)
{
    s8 s;

	 struct timespec ts;
    gettimeofday((struct timeval *)&ts, NULL);
	 gmtime(&ts.tv_sec);
	 ts.tv_sec+=2;
//	 s = sem_wait(&sem_readfs);//死等 测了两天并没发现死锁，但还是不放心,改成下面的阻塞一秒方式，防止死锁
     s = sem_timedwait(&sem_readfs, &ts);//阻塞一秒

    int  count;


   if(access(FileInfo[fid].Name, 0)==0)
	     fs_open(fid, "r+");
   else
	     fs_open(fid, "w+");
    //lib_printf("wf %d\n", fid);

  //  fs_chmod(fid, S_IREAD | S_IWRITE);

    fs_lseek(fid, offset, SEEK_SET);

    count = fs_write(fid, pSourceBuf, write_len);
     printf("到底写成功没呢  太奇怪了吧  ！！！！count=%d write_len=%d  offset\r\n",count,(int)write_len);
    fs_close(fid);
    if(s==0)
	  sem_post(&sem_readfs);
//
//    log_debug("======write File: fileid=%d id=%d offset=%d write_len=%d count=%d name=[%s]\n",
//        file_id, fid, offset, write_len, count, FileInfo[fid].Name);
    //lib_printf("wfe\n");
    return count;
}
int fs_access(u32 file_id)
{
	return access(FileInfo[file_id].Name, 0);
}

char* fs_fgets(u32 file_id,char* buf,int len)
{
	char* p;
	if (FileInfo[file_id].Valid) {
		p=fgets(buf,len,FileInfo[file_id].fp);
		if(p==NULL)
		{
			return NULL;
		}
		FileInfo[file_id].Pointer=strlen(p);

     	return(p);//FileInfo[file_id].Name
	}
	return NULL;
}
int parse_config(u32 fid,char *value)
{
//	char buf[64] = {0};
	//char *p = NULL;
	//int match_flag = 0;

  //   fs_lseek(fid, FileInfo[fid].Pointer, SEEK_SET);
    if( fs_fgets(fid,value, 64) != NULL)
    	return 0;
    
    
	return 1;
}
int parse_config1(u32 fid,const char *key, char* newcfg)
{
	char buf[64] = {0};
	char *p = NULL;
    FILE *fp;
    fp=fs_open(fid, "r");
    int offset=0;
     fs_lseek(fid, 0, SEEK_SET);
    printf("yyyyyyyyyyyyyyyyyyyyyyyyyyyy\r\n");
	while(fs_fgets(fid,buf, 64) != NULL){
		printf("xxxxxxxxxxxx =%s\r\n",buf);

		p = strstr(buf, key);//如can_num

		if(p){
		    fs_close(fid);
			printf("qqqqqqqqqqqqqqq =%s\r\n",buf);
		    p = strstr(buf, "=");
		    p=p+1;

		    memcpy(p,newcfg,strlen(newcfg));

			printf("qqqqqqqqqqqqqqq buf=%s\r\n",buf);
			buf[strlen(buf)+1]=0x00;
		    fp=fs_open(fid, "r+");

		     fs_lseek(fid, offset, SEEK_SET);
		    fs_write(fid, buf, strlen(buf));
 		    fs_close(fid);
 			 return 1;
			}
		else
			offset+=strlen(buf);
		}
    fs_close(fid);
	return 0;
}
int parse_config(u32 fid,const char *key, int &value)
{

	char buf[64] = {0};
	char *p = NULL;
	int match_flag = 0;

    fs_open(fid, "r");
//     fs_chmod(fid, S_IREAD | S_IWRITE);

     fs_lseek(fid, 0, SEEK_SET);

	while(fs_fgets(fid,buf, 64) != NULL){
		if(buf[strlen(buf) - 2] == '\r')
			buf[strlen(buf) - 2] = '\0';
		else if(buf[strlen(buf) - 1] == '\r')
			buf[strlen(buf) - 1] = '\0';

		p = strstr(buf, key);//如can_num=
		if(p){
			p = strstr(buf, "=");
			if(*(p+1) != '\0'){
	//			printf("matched %s %s\n", key, p+1);
				value = atoi(p+1);
				match_flag = 1;
			}
		}
	}

	if(match_flag == 0)
	{
		printf("没找到！！！！！！！！！！！！！！！");
		return -1;
	}

	return 0;
}

int parse_config(u32 fid,const char *key, char* value)
{
	char buf[64] = {0};
	char *p = NULL;
	int match_flag = 0;

     fs_open(fid, "r");
//     fs_chmod(fid, S_IREAD | S_IWRITE);
    fs_lseek(fid, 0, SEEK_SET);

	while(fs_fgets(fid,buf, 64) != NULL){

		if(buf[strlen(buf) - 2] == '\r')
			buf[strlen(buf) - 2] = '\0';
		else if(buf[strlen(buf) - 1] == '\r')
			buf[strlen(buf) - 1] = '\0';
		p = strstr(buf, key);//如can_num=
		if(p){
			p = strstr(buf, "=");
			if(*(p+1) != '\0'){
				//printf("matched %s %s\n", key, p+1);
			//	value = p+1;
				memcpy(value,p+1,strlen(p+1));
				value[strlen(p+1)]='\r';
				match_flag = 1;
				break;
			}
		}
	}

	if(match_flag == 0)
	{
		return -1;
	}

	return 0;
}

//int parse_config1(u32 fid, const char *key, char * value)
//{
//	return 0;access
//}

int createStrfile(u32 fid)
{
	int ret;
    FILE *fp;
     ret=fs_access(fid);
     if(ret==-1)
     {
         fp=fs_open(fid, "w+");
         if(fp==NULL)
         {
        	 printf("文件建立失败!!!!!!!!!!!!!!!!!!!!\r\n");
        	 return -1;
         }
         else
         {
        	 printf("文件建立成功!!!!!!!!!!!!!!!!!!!!\r\n");
     		FileInfo[fid].Pointer=0;
        	 return 1;
         }
         
     }

     fp=fs_open(fid, "r");
     if(FileInfo[fid].Valid==False)
     {
    	 printf("文件存在，打开文件失败!!!!!!!!!!!!!!!!!!!!\r\n");
    	 return -1;
     }
     fs_lseek(fid, 0, SEEK_SET);
     printf("文件存在，打开文件成功!!!!!!!!!!!!!!!!!!!!\r\n");
     return 0;
     
}
//int createStrfile(u32 fid)
//{
//	int ret;
//    FILE *fp;
//     ret=fs_access(fid);
//     if(ret==-1)
//     {
//         fp=fs_open(fid, "w+");
//         if(fp==NULL)
//         {
//        	 printf("文件建立失败!!!!!!!!!!!!!!!!!!!!\r\n");
//        	 return -1;
//         }
//         else
//         {
//        	 printf("文件建立成功!!!!!!!!!!!!!!!!!!!!\r\n");
//     		FileInfo[fid].Pointer=0;
//        	 return 1;
//         }
//
//     }
//
//     fp=fs_open(fid, "r");
//     if(FileInfo[fid].Valid==False)
//     {
//    	 printf("文件存在，打开文件失败!!!!!!!!!!!!!!!!!!!!\r\n");
//    	 return -1;
//     }
//     fs_lseek(fid, 0, SEEK_SET);
//     printf("文件存在，打开文件成功!!!!!!!!!!!!!!!!!!!!\r\n");
//     return 0;
//
//}
int createStrfile(u32 fid,char* pname,u32 size)
{
	int ret;
    FILE *fp;



    if(fid==0)
    {
    	fid=fs_registerFile(fid,pname,size);
    	if(fid<0)
    	{
    		return -1;
    	}

    }
     ret=fs_access(fid);
     if(ret==-1)
     {
         fp=fs_open(fid, "w+");
         if(fp==NULL)
         {
        	 printf("文件建立失败!!!!!!!!!!!!!!!!!!!!\r\n");
        	 return -1;
         }
         else
         {
        	 printf("文件建立成功!!!!!!!!!!!!!!!!!!!!\r\n");
     		FileInfo[fid].Pointer=0;
        	 return 1;
         }

     }

     fp=fs_open(fid, "r");
     if(FileInfo[fid].Valid==False)
     {
    	 printf("文件存在，打开文件失败!!!!!!!!!!!!!!!!!!!!\r\n");
    	 return -1;
     }
     fs_lseek(fid, 0, SEEK_SET);
     printf("文件存在，打开文件成功!!!!!!!!!!!!!!!!!!!!\r\n");
     return 0;

}
// 初始化文件系统
void FS_Init(void)
{
//	int ret;
	printf("初始化文件系统");
    sem_init(&sem_readfs, 0, 1);
	mkdir(FS_DAT_PATH, S_IRWXU);//PARA_MODEL
//    ret= fs_registerFile(FILE_ID_TCU_CONF, FILE_NAME_TCU_CONF,  FILE_SIZEOF_TCU_CONF);
//    ret =fs_registerFile(FILE_ID_TCU_PARA, FILE_NAME_TCU_PARA,  FILE_SIZEOF_TCU_PARA);
//
//
//    ret =fs_registerFile(FILE_ID_CHARGE_RD_0, FILE_NAME_CHARGE_RD_0,  FILE_SIZEOF_CHARGE_RD_0);
//    ret =fs_registerFile(FILE_ID_CHARGE_RD_1, FILE_NAME_CHARGE_RD_1,  FILE_SIZEOF_CHARGE_RD_1);
//
//    ret =fs_registerFile(FILE_ID_CHARGE_RD_2, FILE_NAME_CHARGE_RD_2,  FILE_SIZEOF_CHARGE_RD_2);
//    ret =fs_registerFile(FILE_ID_CHARGE_RD_3, FILE_NAME_CHARGE_RD_3,  FILE_SIZEOF_CHARGE_RD_3);
//
//    ret =fs_registerFile(FILE_ID_CHARGE_RD_4, FILE_NAME_CHARGE_RD_4,  FILE_SIZEOF_CHARGE_RD_4);
//    ret =fs_registerFile(FILE_ID_CHARGE_RD_5, FILE_NAME_CHARGE_RD_5,  FILE_SIZEOF_CHARGE_RD_5);
//
//    ret =fs_registerFile(FILE_ID_CHARGE_RD_6, FILE_NAME_CHARGE_RD_6,  FILE_SIZEOF_CHARGE_RD_6);
//    ret =fs_registerFile(FILE_ID_CHARGE_RD_7, FILE_NAME_CHARGE_RD_7,  FILE_SIZEOF_CHARGE_RD_7);
//
//    ret =fs_registerFile(FILE_ID_HMD, FILE_NAME_HMD,  FILE_SIZEOF_HMD);
//#ifdef XJ_MQTT_CLIENT
//    ret =fs_registerFile(FILE_ID_QRCODE, FILE_NAME_QRCODE,  FILE_SIZEOF_QRCODE);
//#endif
//
//    ret =fs_registerFile(FILE_ID_FEECFG, FILE_NAME_FEECFG,  FILE_SIZEOF_FEECFG);
//
//    if(ret==-1)
//    	printf("初始化文件系统失败!");

}

/*
 * fs.h
 *
 *  Created on: 2018-3-27
 *      Author: root
 */

#ifndef FFS_H_
#define FFS_H_
#include "share_type.h"
#include <stdio.h>

#define MAX_FILENAME_LEN 50
//#define RD_SIZE   96
//#define MAX_RDSIZE_NAND  29360  //80+80*366  头+366*充电数据大小
////#define MAX_RDSIZE_NAND  35232  //96+96*366  头+366*充电数据大小(小桔）
//
#define FS_DAT_PATH "/usr/local/sanway/emu_doc" //默认路径，避免文件杂乱无章，统一放到files目录下  /home/root/sanway  /usr/local/sanway/dbTest
#define MAX_FILE_SUPPORT 60 //最大支持文件数
//#define PARA_START     0
//#define PARA_BASIC_AREA    PARA_START
//#define PARA_MODEL_AREA     PARA_BASIC_AREA+64
//#define PARA_END     PARA_MODEL_AREA+5000
//#define SIZEOF_CHARGE_MODEL   256
//
//#define TRAN_RECORD_START  0
//#define TRAN_RECORD_END     2560
//
//#define TRAN_RECORD_START  0
//#define TRAN_RECORD_END    MAX_RDSIZE_NAND



//
//
//#define FEECFG_START  0
//#define FEECFG_END  6400
//
//#define HMD_START  0
//#define HMD_END  1024


typedef enum
{
	FS_R =0,
	FS_W,
}RW;

typedef struct {
	boolean Valid; //文件是否有效
	u32 ID;  //注册id
    FILE *fp;
	char Name[MAX_FILENAME_LEN];//文件名 按最长8位处理  加路径，预留20个字节
	u8 Offset;
	u32 Size;  //文件大小
    u32 Pointer;//文件指针
}TFileInfo;// 文件信息 用于生成文件系统数据
//#define FILE_ID_TCU_CONF             0 //TCU参数设置文件
//#define FILE_NAME_TCU_CONF         "charge.conf"
//#define FILE_SIZEOF_TCU_CONF       256
//
//#define FILE_ID_TCU_PARA             1 //TCU参数设置文件
//#define FILE_NAME_TCU_PARA         "tcu_para.p"
//#define FILE_SIZEOF_TCU_PARA       PARA_END
//
//
//#define FILE_ID_CHARGE_RD_0             2 //当前充电记录
//#define FILE_NAME_CHARGE_RD_0         "charge_rd_0.d" //
//#define FILE_SIZEOF_CHARGE_RD_0       MAX_RDSIZE_NAND
//
//
//#define FILE_ID_CHARGE_RD_1             3 //当前充电记录
//#define FILE_NAME_CHARGE_RD_1         "charge_rd_1.d" //
//#define FILE_SIZEOF_CHARGE_RD_1       MAX_RDSIZE_NAND
//
//
//#define FILE_ID_CHARGE_RD_2             4 //当前充电记录
//#define FILE_NAME_CHARGE_RD_2         "charge_rd_2.d" //
//#define FILE_SIZEOF_CHARGE_RD_2       MAX_RDSIZE_NAND
//
//
//#define FILE_ID_CHARGE_RD_3             5 //当前充电记录
//#define FILE_NAME_CHARGE_RD_3         "charge_rd_3.d" //
//#define FILE_SIZEOF_CHARGE_RD_3       MAX_RDSIZE_NAND
//
//
//#define FILE_ID_CHARGE_RD_4            6 //当前充电记录
//#define FILE_NAME_CHARGE_RD_4         "charge_rd_4.d" //
//#define FILE_SIZEOF_CHARGE_RD_4       MAX_RDSIZE_NAND
//
//
//#define FILE_ID_CHARGE_RD_5             7 //当前充电记录
//#define FILE_NAME_CHARGE_RD_5         "charge_rd_5.d" //
//#define FILE_SIZEOF_CHARGE_RD_5       MAX_RDSIZE_NAND
//
//
//#define FILE_ID_CHARGE_RD_6             8 //当前充电记录
//#define FILE_NAME_CHARGE_RD_6         "charge_rd_6.d" //
//#define FILE_SIZEOF_CHARGE_RD_6       MAX_RDSIZE_NAND
//
//
//#define FILE_ID_CHARGE_RD_7             9 //当前充电记录
//#define FILE_NAME_CHARGE_RD_7         "charge_rd_7.d" //
//#define FILE_SIZEOF_CHARGE_RD_7       MAX_RDSIZE_NAND
//
//
//
//
//
//
//#define FILE_ID_HMD             10
//#define FILE_NAME_HMD  "hmd.d" //
//#define FILE_SIZEOF_HMD       1024
//
//#define FILE_ID_QRCODE            11//二维码
//#define FILE_NAME_QRCODE        "qrcode.p" //
//#define QRCODESIZE      256
//#define FILE_SIZEOF_QRCODE       4096
//
//#define FILE_ID_FEECFG           12 //本次充电费率
//#define FILE_NAME_FEECFG         "feeconfig.p" //
//#define FILE_SIZEOF_FEECFG       6400


void FS_Init(void);
s32 fs_rm(u32 file_id);
int app_fs_read(u32 fid, u32 offset, u32 read_len, u8* pDestBuf);
int app_fs_write(u32 fid, u32 offset, u32 write_len, u8* pSourceBuf);
int createStrfile(u32 fid);
int fs_write(u32 file_id, const void *pbuf, u32 len);
int fs_write_null(u32 file_id, const void *pbuf, u32 len);
int fs_close(u32 file_id);
s32 fs_lseek(u32 file_id, u32 offset, s32 whence);
int parse_config(u32 fid,char *value);
int parse_config(u32 fid,const char *key, int &value);
int parse_config(u32 fid,const char *key, char* value);
int parse_config1(u32 fid,const char *key, char* newcfg);
//int app_fs_write1(u32 fid, u32 offset, u32 write_len, u8* pSourceBuf);
s32 fs_registerFile(u32 file_id, const char *file_name, u32 store_size);
FILE * fs_open(u32 file_id, const char *mode);
int app_fs_read_all(u32 fid, u8* pDestBuf);
#endif /* FS_H_ */

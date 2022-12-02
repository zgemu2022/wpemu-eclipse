/*
 * standard104.h
 *
 *  Created on: 2018-1-8
 *      Author: root
 */

#ifndef STANDARD104_H_
#define STANDARD104_H_

#include "com_include.h"
#ifdef __cplusplus

extern "C"{

#endif
#include "share_type.h"
#define MAX_GIN_SIZE		255
typedef struct
{
	int V_S;	//发送序号
	int V_R;	//接收序号
}My_TmpClient;

//U格式    --起始命令/配置、停止命令/配置、测试命令/配置
#define U_STARTDT_COMMAND	0x02
#define U_STARTDT_CONFIRM	0x04
#define U_STOPDT_COMMAND	0x08
#define U_STOPDT_CONFIRM	0x10
#define U_TESTFR_COMMAND	0x20
#define U_TESTFR_CONFIRM	0x40
//超时的定义
#define RECV_TIMEOUT_T0		10
#define RECV_TIMEOUT_T1		12
#define RECV_TIMEOUT_T2		5
#define RECV_TIMEOUT_T3		15
#define RECV_TIMEOUT_T4		8
#define MAX_DTE_VALUE_K		12		/* 发送方不得发送超过k个未经确认的I格式APDU报文 */
#define MAX_BUF_SIZE		1024
#define MAX_DTE_VALUE_W		8		/* 接收方收到w个I格式APDU后确认 */


enum CtrlFormat{
      I=  0,//编号的信息传输（I格式）
	  S,     //编号的监视功能（S格式）
	  U,     //无编号
//	  I_MAS, //主站I包
};
enum U_SETType{
	CONFIRM   = 0,
	ACT,

};
enum UType{
	 STARTDT   = 0,
	 STOPDT,
   TESTFR,

};
typedef struct
{
	u8 type;
	u8 settype;
}Uoder;

typedef struct
{
	u8  buf[MAX_BUF_SIZE];	//数据
	int len;	//buf的长度
}MyData;
typedef struct
{
	char format;		/*   0:I格式  1:S格式  3:U格式*/
	int V_S;//发送序列号
	int V_R;//接收序列号

	unsigned char TYP;	/*   类型*/
//	unsigned char VSQ;  /*   可变结构限定词*/
	unsigned char VSQ_sq;
	unsigned char VSQ_num;
	unsigned short COT;	/*   传送原因,2字节*/
	unsigned short addr;/*   地址,2字节*/
	unsigned char infoaddr[3];
    Uoder uoder;
    u8 record_type;//记录类型
	MyData inf_buf;		/*   更多地信息数据,主要是解析时存储使用*/

	/* 组帧时有可能用到的参数,解帧时未使用 */
	unsigned short command;			/*    0x80:选择; 0x00:执行;	 QOI, QRP: 别人是多少就是多少;  延时获得:表示毫秒*/
}APDU_Describe;

int check_APDU(unsigned char* pdata, int datalen, int* lpdu_start, int* lpdu_len);
int parse_APDU(unsigned char* pdata, int datalen, APDU_Describe* frame_info);
//int merge_APCI(MyData* dest, APDU_Describe head);
u16 merge_APCI_U(MyData* dest, APDU_Describe head);
//u16 merge_APCI_I(MyData* dest, APDU_Describe head);

#ifdef __cplusplus

}

#endif
#endif /* STANDARD104_H_ */

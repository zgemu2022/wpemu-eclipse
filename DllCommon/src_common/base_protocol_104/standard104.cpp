/*
 * standard104.cpp
 *
 *  Created on: 2018-1-8
 *      Author: root
 */
#include "standard104.h"
#include <string.h>

/*******************************************************************************************
 *寻找数据包头包尾,前两参数是接收到的数据和长度,
 *后两个参数是本函数匹配LPDU包后回送的位置和包长。
 *返回值: -1:未匹配头部; 0:匹配头部但未接收完; 1:匹配头部且数据已接收完毕
 ******************************************************************************************/
int check_APDU(unsigned char* pdata, int datalen, int* lpdu_start, int* lpdu_len)
{
	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
	//LOG("***  F:%s L:%d \n", __FILE__, __LINE__);
	int i,  len;
	int ret = -1;

	if( NULL == pdata || datalen <= 0 )
		return ret;

	i = 0;
	while( i<datalen )
	{
		if( i+5 > datalen )	/* 数据不足以匹配, 退出循环*/
			break;

		if( pdata[i] == 0x68 ) {	    /* 可变帧*/
			len = pdata[i+1]+pdata[i+2]*256;           /* APDU 长度 */
			if( (i+len+3) > datalen )	/* len = 控制域*4 + ASDU*/
				ret = 0;
			else
				ret = 1;

			/*  lpdu_start -->报文起始位置(68H) ,  lpdu_len-->报文长度*/
			if( NULL != lpdu_start )
				*lpdu_start = i;
			if( NULL != lpdu_len )
				*lpdu_len = len+3;	/* 2 = ( 0x68 长度 )头2字节*/
			break;
		}
		i++;
	}
	printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
	return ret;
}
/*******************************************************************************************
 *读取数据包基本信息,前两参数是接收到的数据和长度,
 *后两个参数是本函数解析信息后回送的基本信息。
 *返回值:  -1:异常返回;      0:正常返回
 ******************************************************************************************/
int parse_APDU(unsigned char* pdata, int datalen, APDU_Describe* frame_info)
{
	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
	int len;
	int ret = -1;
	unsigned char* p;
	APDU_Describe f_inf;
	unsigned short u16_tmp;

	if ( NULL == pdata || datalen <= 0)
		return ret;

	memset(&f_inf, 0, sizeof(f_inf));
	if (pdata[0] != 0x68) /* 起始字符 */
		return ret;
	ret = 0;
	if (((pdata[3] & 0x01) == 0) && ((pdata[5] & 0x01) == 0)) { /*  控制域位组1  、控制域位组3   */
		f_inf.format = I;	//I格式
		//		LOG_D("IIIIIIII IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");
	} else if((pdata[3] & 0x03)==1){
		f_inf.format = S;
	}
	else if ((pdata[3] & 0x03) == 3) {
		f_inf.V_S=0;
		f_inf.V_R =0;
		f_inf.format = U;
		if (pdata[3] & (1 << 2)) {
			f_inf.uoder.type = STARTDT;
			f_inf.uoder.settype = ACT;
		} else if (pdata[3] & (1 << 3)) {
			f_inf.uoder.type = STARTDT;
			f_inf.uoder.settype = CONFIRM;
		} else if (pdata[3] & (1 << 4)) {
			f_inf.uoder.type = STOPDT;
			f_inf.uoder.settype = ACT;
		} else if (pdata[3] & (1 << 5)) {
			f_inf.uoder.type = STOPDT;
			f_inf.uoder.settype = CONFIRM;
		} else if (pdata[3] & (1 << 6)) {
			f_inf.uoder.type = TESTFR;
			f_inf.uoder.settype = ACT;
		} else if (pdata[3] & (1 << 7)) {
			f_inf.uoder.type = TESTFR;
			f_inf.uoder.settype = CONFIRM;
		}
		//		LOG_D("&&&&&&&&&&&   f_inf.format = %d    \n", f_inf.format);
	}
	else
		return -1;

	if (f_inf.format != U) {
		f_inf.V_S = ((pdata[3] >> 1) & 0x7F) | (pdata[4] << 7); /* 7F-->0111  1111  */
		f_inf.V_R = ((pdata[5] >> 1) & 0x7F) | (pdata[6] << 7);
	}
		//	LOG("***  format=%d V_S=%d V_R=%d  ***\n",f_inf.format,f_inf.V_S,f_inf.V_R);
		if (datalen > 16) {
			p = pdata + 7; /* 前面6个为APCI  , 指针p  指向ASDU */
			f_inf.TYP = *p++; /* ASDU[1]--> 类型标识*/
//			f_inf.VSQ = *p++; /* ASDU[2]--> 可变结构限定词*/
			f_inf.VSQ_sq=*p++;
			f_inf.VSQ_num=f_inf.VSQ_sq;
			f_inf.VSQ_num &= 0x7f ;
			f_inf.VSQ_sq >>=7;
			u16_tmp = (*p++) & 0xFF; /*  传送原因COT 低字节  */
			u16_tmp = u16_tmp | (((*p++) & 0xFF) << 8); /*  传送原因COT 高字节  */
			f_inf.COT = u16_tmp; /*  COT换2字节了...  */
			u16_tmp = (*p++) & 0xFF; /*  应用服务数据单元公共地址addr  低字节  */
			u16_tmp = u16_tmp | (((*p++) & 0xFF) << 8); /*  应用服务数据单元公共地址addr  高字节  */
			f_inf.addr = u16_tmp; /*  地址换2字节了...   */
			len = datalen - (p - pdata); /*  ASDU 的长度*/
			f_inf.inf_buf.len = len;
			if (len > 0)
			{

				memcpy(f_inf.inf_buf.buf, p, len); /* f_inf.inf_buf.buf 中存放ASDU  的数据 */
				f_inf.infoaddr[0]=f_inf.inf_buf.buf[0];
				f_inf.infoaddr[1]=f_inf.inf_buf.buf[1];
				f_inf.infoaddr[2]=f_inf.inf_buf.buf[2];
			}
//			LOG("***  TYP = %d VSQ = %d COT = %d addr = %d  ***\n",f_inf.TYP,f_inf.VSQ,f_inf.COT,f_inf.addr);
		}


	if ( NULL != frame_info) {
		memcpy(frame_info, &f_inf, sizeof(f_inf));
	}
	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
	return ret;
}
u16 merge_APCI_U(MyData* dest, APDU_Describe head)
{
	u8 pos=0;
	u8 len;
	u8 ctrl=0;
	u8 shift=0;
	if(dest == NULL)
		return -1;
	memset(dest, 0, sizeof(MyData));
	dest->buf[pos++] = 0x68;
	dest->buf[pos++] = 0x00;
	dest->buf[pos++] = 0x00;

	if(head.format==U)
	{
		if(head.uoder.type==STARTDT)
			shift=2;
		else if(head.uoder.type==STOPDT)
			shift=4;
		else if(head.uoder.type==TESTFR)
			shift=6;
		if(head.uoder.settype==CONFIRM)
			ctrl|=(1<<(shift+1));
		else if(head.uoder.settype==ACT)
			ctrl|=(1<<shift);
	}
	ctrl|=0x03;
	dest->buf[pos++]=ctrl;
	dest->buf[pos++]=0;
	dest->buf[pos++]=0;
	dest->buf[pos++]=0;
	len=pos-3;
	dest->buf[1]=len%256;
	dest->buf[2]=len/256;
	dest->len=pos;
	return 0;
}


//	switch(papdu->typDU)
//	{
//		case M_SP_NA_1:
//		{
//			for(i=0;i<papdu->vsq_NUM;i++)
//			{
//					if(papdu->vsq_SQ==0)
//					{
//
//						memcpy(&pbuf[pos],gun_addr[papdu->noGun],3);
//						pbuf[pos]|=i;
//						pos+=3;
//					}
//					//pbuf[pos++]=papdu->isu.ioder.ac_sp.qds[i];
//					pbuf[pos++]=papdu->isu.ioder.ac_sp.yx[i];
//
//			}
//		}
//
//		break;
//		case M_ME_NB_1:
//		{
//			for(i=0;i<papdu->vsq_NUM;i++)
//			{
//					if(papdu->vsq_SQ==0)
//					{
//
//						memcpy(&pbuf[pos],gun_addr[papdu->noGun],3);
//						pbuf[pos]|=i;
//						pos+=3;
//					}
//				  pbuf[pos++]=(u8)papdu->isu.ioder.ac_me.yc[i];
//					pbuf[pos++]=(u8)(papdu->isu.ioder.ac_me.yc[i]>>8);
//					pbuf[pos++]=papdu->isu.ioder.ac_me.qds[i];
//			}
//		}
//
//
//			break;
//		case M_MD_NA_1:
//		{
//           memcpy(&pbuf[pos],gun_addr[papdu->noGun],3);
//			     pbuf[pos+1]=1;
//					 pos+=3;
//			     pbuf[pos++]=4;
//			     pbuf[pos++]=(u8)papdu->isu.ioder.ac_md.kWh;
//			     pbuf[pos++]=(u8)(papdu->isu.ioder.ac_md.kWh>>8);
//			     pbuf[pos++]=(u8)(papdu->isu.ioder.ac_md.kWh>>16);
//			     pbuf[pos++]=(u8)(papdu->isu.ioder.ac_md.kWh>>24);
//			     pbuf[pos++]=papdu->isu.ioder.ac_md.qds;
//
//    }
//
//			break;
//			case C_IC_NA_1:
//		{
//           memcpy(&pbuf[pos],gun_addr[papdu->noGun],3);
//					 pos+=3;
//			     pbuf[pos++]=0x14;
//
//    }
//		break;
//			case C_CS_NA_1://假装主站发送对时帧 15年3月16日9时16分
//			{
//	         memcpy(&pbuf[pos],gun_addr[papdu->noGun],3);
//					 pos+=3;
//           pbuf[pos++]=0x01;
//				   pbuf[pos++]=0x05;
//           pbuf[pos++]=0x10;
//				   pbuf[pos++]=0x09;
//           pbuf[pos++]=0x10;
//				   pbuf[pos++]=0x03;
//           pbuf[pos++]=0x11;
////           pbuf[pos++]=0x01;
////				   pbuf[pos++]=0x05;
////           pbuf[pos++]=0x10;
////				   pbuf[pos++]=0x09;
////           pbuf[pos++]=0x10;
////				   pbuf[pos++]=0x03;
////           pbuf[pos++]=0x0f;
//			}
//			break;
//
//		default:
//			break;
//
//	}
//	len=pos-3;
//	pbuf[1]=len%256;
//	pbuf[2]=len/256;
//	return pos;
//}
////组合报文到dest中, 无ASDU, 可用于S格式或U格式
//int merge_APCI(MyData* dest, APDU_Describe head)
//{
//	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
//	if(dest == NULL)
//		return -1;
//	memset(dest, 0, sizeof(MyData));
//
//	switch( head.format )
//	{
//		case I: //I
//			dest->buf[3] = 0;
//			dest->buf[3] |= ((head.V_S<<1)&0xFF);
//			dest->buf[4] = (head.V_S>>7)&0xFF;
//			break;
//		case S: //S
//			dest->buf[3] = 1;
//			dest->buf[4] = 0;
//			break;
//		case U: //U
//			{
//				u8 ctrl=0;
//				u8 shift=0;
//				if(head.uoder.type==STARTDT)
//					shift=2;
//				else if(head.uoder.type==STOPDT)
//					shift=4;
//				else if(head.uoder.type==TESTFR)
//					shift=6;
//				if(head.uoder.settype==CONFIRM)
//					ctrl|=(1<<(shift+1));
//				else if(head.uoder.settype==ACT)
//					ctrl|=(1<<shift);
//			}
//
//
//			dest->buf[3] = 3;
//			dest->buf[3] |= ((head.V_S<<1)&0xFC);
//			dest->buf[4] = (head.V_S>>7)&0xFF;
//			break;
//		default:
//			return -1;
//	}
//	dest->buf[0] = 0x68;
//	dest->buf[1] = 0x04;
//	dest->buf[2] = 0x0;
//	dest->buf[5] = ((head.V_R&0xFF)<<1);
//	dest->buf[6] = (head.V_R>>7)&0xFF;
//	dest->len = 7;
//	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
//	return 0;
//}
////组合报文到dest中, 有ASDU, 可用于S格式或I格式
//int merge_APDU(MyData* dest, APDU_Describe head, POINT_INFO* data)
//{
//	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
//	int i, ret = -1;
//	unsigned char *p,*asdu, u8;
//	MyData s_merge_apdu;
//	unsigned short u16;
//
//	unsigned short ms;
//	struct timeval tv;
//	struct tm* timenow;
//	if(dest==NULL)
//		return -1;
//	//printf("F:%s L:%d TYP=%d VSQ=%d format=%d COT=%d V_R=%d V_S=%d  ***\n",__FUNCTION__,__LINE__,head.TYP,head.VSQ,head.format,head.COT,head.V_R,head.V_S);
//
//	ret = merge_APCI(&s_merge_apdu, head);
//	if( ret <0 ) {
////		LOG("-- merge_APCI failed.\n");
//		return ret;
//	}
//
//		p = asdu = &s_merge_apdu.buf[7];
//		*p++ = head.TYP;
//		*p++ = head.VSQ;
//		*p++ = LOW_BYTE(head.COT);
//		*p++ = HIGH_BYTE(head.COT);
//		*p++ = LOW_BYTE(head.addr);
//		*p++ = HIGH_BYTE(head.addr);
//
//		if(head.TYP == 1){
//			if(head.COT == 20){
//				for( i=0; i<(head.VSQ&0x7F); i++ ) {
//					if( i==0 || ( i>0 && (head.VSQ&0x80)==0 ) ) {
//						*p++ = LOW_BYTE(data[i].addr);
//						*p++ = HIGH_BYTE(data[i].addr);
//						if( GIN_ADDRESS_LEN == 3 ) {
//							*p++ = HIGH_HIGH_BYTE(data[i].addr);	/*  信息体地址, 有的是3字节*/
//						}
//					}
//					u8 = *data[i].data;
//					*p++ = u8;
//				}
//			}else if(head.COT == 3){
//				for( i=0; i<(head.VSQ&0x7F); i++ ) {
//					if( i==0 || ( i>0 && (head.VSQ&0x80)==0 ) ) {
//						*p++ = LOW_BYTE(i);
//						*p++ = HIGH_BYTE(i);
//						if( GIN_ADDRESS_LEN == 3 ) {
//							*p++ = HIGH_HIGH_BYTE(i);	/*  信息体地址, 有的是3字节*/
//						}
//					}
//					if(gnetcom->type == 0){
//						u8 = preST.dc.ind[i];
//						//printf("preST.dc.ind[%d] = %hhd\n", i, preST.dc.ind[i]);
//					}else if(gnetcom->type == 1){
//						u8 = preST.ac.ind[i];
//						//printf("preST.ac.ind[%d] = %hhd\n", i, preST.ac.ind[i]);
//					}
//					*p++ = u8;
//				}
//			}
//		}
//		else if(head.TYP == 11) {	/*     数据->不带时标: 归一化值/标度化值*/
//			if(head.COT == 20){
//				for( i=0; i<(head.VSQ&0x7F); i++ ) {
//					if( i==0 || ( i>0 && (head.VSQ&0x80)==0 ) ) {
//						*p++ = LOW_BYTE(data[i].addr);
//						*p++ = HIGH_BYTE(data[i].addr);
//						if( GIN_ADDRESS_LEN == 3 ) {
//							*p++ = HIGH_HIGH_BYTE(data[i].addr);	/*     信息体地址, 有的是3字节*/
//						}
//					}
//
//					u16 = (unsigned short)(*data[i].data);
//					*p++ = LOW_BYTE(u16);
//					*p++ = HIGH_BYTE(u16);
//					*p++ = data[i].q;
//				}
//			}else if(head.COT == 3){
//				for( i=0; i<(head.VSQ&0x7F); i++ ) {
//					if( i==0 || ( i>0 && (head.VSQ&0x80)==0 ) ) {
//						*p++ = LOW_BYTE(i);
//						*p++ = HIGH_BYTE(i);
//						if( GIN_ADDRESS_LEN == 3 ) {
//							*p++ = HIGH_HIGH_BYTE(i);	/*     信息体地址, 有的是3字节*/
//						}
//					}
//
//					if(gnetcom->type == 0){
//						u16 = preMX.dc.MX[i];
//						//printf("preMX.dc.MX[%d] = %hd\n", i, preMX.dc.MX[i]);
//					}else if(gnetcom->type == 1){
//						u16 = preMX.ac.MX[i];
//						//printf("preMX.ac.MX[%d] = %hd\n", i, preMX.ac.MX[i]);
//					}
//
//					*p++ = LOW_BYTE(u16);
//					*p++ = HIGH_BYTE(u16);
//					*p++ = 0;
//				}
//			}
//		}
//
//		else if( head.TYP == 45 || head.TYP == 46 ) {	/*单点遥控, 双点遥控*/
//			if( data == NULL ) {
//				return -1;
//			}
//			u8 = (unsigned char)head.command;
//			/* if( head.command == 1 ) {
//				u8 = 0x80;
//			} else if( head.command == 2 ) {
//				u8 = 0x00;
//			} */
//			*p++ = LOW_BYTE(data[0].addr);// 2字节GIN
//			*p++ = HIGH_BYTE(data[0].addr);
//			if( GIN_ADDRESS_LEN == 3 ) {
//				*p++ = HIGH_HIGH_BYTE(data[0].addr);	/*     信息体地址, 有的是3字节*/
//			}
//			u8 |= (*data[0].data);
//			*p++ = u8;
//		}
//		else if( head.TYP == 100 || head.TYP == 105 ) {	/*  召唤命令QOI   //复位进程QRP  */
//			*p++ = 0;
//			*p++ = 0;
//			if( GIN_ADDRESS_LEN == 3 ) {
//				*p++ = 0;	/*  信息体地址, 有的是3字节 */
//			}
//			*p++ = head.command;	/*  QOI召唤限定词, 对方请求的是多少, 回应就得是多少  */
//		}
//
//		else if( head.TYP == 103 ) {	/*      对时*/
//			*p++ = 0;
//			*p++ = 0;
//			if( GIN_ADDRESS_LEN == 3 ) {
//				*p++ = 0;	/*   信息体地址, 有的是3字节*/
//			}
//			/*** 7个8位位组时间 ***/
//			gettimeofday (&tv, NULL);
//			timenow = localtime(&tv.tv_sec);
//			ms = timenow->tm_sec * 1000 + (tv.tv_usec/1000);
//			*p++ = ms&0xFF;
//			*p++ = (ms>>8)&0xFF;
//			*p++ = timenow->tm_min;
//			*p = (timenow->tm_isdst>0)?0x80:0x00;	//夏令时
//			*p++ |= (timenow->tm_hour&0x1F);
//			*p++ = /*((timenow->tm_wday&0x06)<<5) | */(timenow->tm_mday&0x1F);	//星期+日期
//			*p++ = timenow->tm_mon+1;
//			*p++ = timenow->tm_year-100;
//		}else if(head.TYP == 130){
//			if(head.command == 13){
//				if(data){
//					MergeTYP130(p, head);
//				}else{
//					switch(startpara.result){
//						case 1:
//							MergeTYP130(p, head, 1);
//							break;
//						case 2:
//							MergeTYP130(p, head, 2);
//							break;
//						default:
//							break;
//					}
//				}
//			}else if(head.command == 14){
//				//endpara.result = 1;
//				printf("endpara.result = %d\n", endpara.result);
//				if(endpara.result == 1){
//					MergeTYP130(p, head);
//				}else if(endpara.result == 2){
//					MergeTYP130(p, head, (1 << 7) | endpara.cause);
//				}
//			}else{
//				MergeTYP130(p, head);
//			}
//		}else{
//			LOG_W("%s:%d not implement merge TYP.\n", __FILE__, __LINE__);
//			return -1;
//		}
//		//数据完成, 计算长度
//		s_merge_apdu.len += p - asdu;
//		s_merge_apdu.buf[1] = (s_merge_apdu.len - 3) & 0xFF;
//		s_merge_apdu.buf[2] = ((s_merge_apdu.len - 3) >> 8) & 0xFF;
//		memcpy(dest, &s_merge_apdu, sizeof(s_merge_apdu));
//
//	//printf("---------------------[F:%s]--------------[F:%s]-----------[L:%d]--------------\n",__FILE__,__FUNCTION__,__LINE__);
//	return ret;
//}

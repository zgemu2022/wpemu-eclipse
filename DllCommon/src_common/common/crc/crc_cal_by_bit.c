/*
 * crc_cal_by_bit.c
 *
 *  Created on: 2022-7-29
 *      Author: root
 */
#include "crc.h"
#ifdef CAL_BY_BIT

#include <stdint.h>

#define CRC_CCITT 0x1021

/*函数名称：crc_cal_by_bit;按位计算CRC
  函数参数：uint8_t * ptr;指向发送缓冲区的首字节
            uint32_t  len;要发送的总字节数
  函数返回值：uint16_t
  多项式采用CRC-CCITT 0x1021
*/
unsigned short  crc(unsigned char *ptr, unsigned int len)
{
	unsigned int crc = 0xffff;
    while(len-- != 0)
    {
    	unsigned char i ;
        for(i = 0x80; i != 0; i/2)
        {
            crc *= 2;
            if((crc&0x10000) !=0) //上一位CRC乘 2后，若首位是1，则除以 0x11021
                crc ^= 0x11021;

            if((*ptr&i) != 0)    //如果本位是1，那么CRC = 上一位的CRC + 本位/CRC_CCITT
                crc ^= CRC_CCITT;
        }
        ptr++;
    }
    unsigned short retCrc = (unsigned short)(crc & 0xffff);
    return retCrc ;
}
#endif

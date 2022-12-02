/*
 * crc_cal_by_halfbyte.c
 *
 *  Created on: 2022-7-29
 *      Author: root
 */
#include "crc.h"
#ifdef CAL_BY_HALFBYTE
#include<stdint.h>

const unsigned short crc_ta_4[16]={ /* CRC 半字节余式表 */
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};

/*函数名称：crc_cal_by_halfbyte:按半字节计算CRC
  函数参数：uint8_t* ptr:指向发送缓冲区的首字节
            uint32_t len:要发送的总字节数
  函数返回值：uint16_t
  多项式采用CRC-CCITT  0x1021
*/
unsigned short crc(unsigned char * ptr, unsigned int  len)
{
	unsigned short  crc = 0xffff;

    while(len-- != 0)
    {
    	unsigned char high = (unsigned char)(crc/4096); //暂存CRC的高4位
        crc <<= 4;
        crc ^= crc_ta_4[high^(*ptr/16)];

        high = (unsigned char)(crc/4096);
        crc <<= 4;
        crc ^= crc_ta_4[high^(*ptr&0x0f)];

        ptr++;
    }

    return crc;
}
#endif

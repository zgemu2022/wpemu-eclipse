/*
 * tools.c
 *
 *  Created on: 2017-12-19
 *      Author: root
 */
#include "tools.h"
#include <stdio.h>
#include <string.h>

char OutBuffer[MAX_OUTPUT_PRINT_SIZE + 5 + 3];//3个* 1个换行 1个\0 ..表示未打印完全
char Str_Buffer[MAX_OUTPUT_BUFFERSIZE];//打印输出缓冲，超过最大忽略后面的内容

char OutBuffer1[MAX_OUTPUT_PRINT_SIZE + 5 + 3];//3个* 1个换行 1个\0 ..表示未打印完全
char Str_Buffer1[MAX_OUTPUT_BUFFERSIZE];//打印输出缓冲，超过最大忽略后面的内容
/*******************************************************************************
 * Function Name  : assert_failed
 * Description    : 断言处理
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void assert_failed(void)
{
    /* Infinite loop */
    while (1)
    {
    	printf("ASSERT FAIL HAPPEND AT: FILE %s ON LINE %d\n\n",
    			__FUNCTION__, __LINE__);

    }
}
/*******************************************************************************
 * Function Name  : hex2char
 * Description    : 把0-15转成'0'-'9' 'A'-'F'
 * Input          : hex 16进制数字
 * Output         : None
 * Return         : u8 ASC（'0'-'9' 'A'-'F'）
 *******************************************************************************/
u8 hex2char(u8 hex)
{
    if(hex > 9)
        return (hex + 'A' - 10);
    else
        return (hex + '0');
}

int strBcd2BcdArray(const char* str,char *buf)
{
	int i=0;
	u8 b1,b2;
	char temp_str[strlen(str)+1];
	int len=strlen(str);
	printf("需要转换的数据长度=%d\r\n",len);
	if(len%2 !=0 )
	{
		sprintf(temp_str,"%s%s",str,"0");
	printf("?????????????????长度奇数吗  %s\r\n",str);
		len+=1;
	}
	else
	{
		printf("?????????????????长度偶数吗  %s\r\n",str);
		sprintf(temp_str,"%s",str);
	}

   while(i!=len)
    {
         b1=temp_str[i]-'0';
         b2=temp_str[i+1]-'0';

         buf[i/2]=(b1<<4)+b2;
  //       printf("b1=%x   b2=%x  b[%d]=%x   \r\n",b1,b2,i/2,buf[i/2]);
         i+=2;

    }
    return len/2;


}
/*******************************************************************************
 * Function Name  : lib_strcpy
 * Description    : 字符串拷贝
 * Input          : dest 目的字符串  src 源字符串
 * Output         : dest 目的字符串
 * Return         : u8* 目的字符串指针
 *******************************************************************************/
char *lib_strcpy(char *dest, const char *src)
{
    char *address = dest;

    ASSERT((dest != NULL));
    ASSERT((src != NULL));

    while ((*dest++ = *src++) != '\0')
        ;
    return address;
}

/*******************************************************************************
 * Function Name  : lib_gcvt
 * Description    : 浮点数转字符串(MAX 4294967295 MIN 0.4294967295)
 * Input          : value 浮点数值   ndigit 小数点位数  buf 目的字符串
 * Output         : None
 * Return         : u8 字符串长度
 *******************************************************************************/
char *lib_gcvt(fp64 value, u16 ndigit, char *buf)
{
    u8 start_digit = 0; //取得最高位的标志，即第一个不是0的数字
    u32 temp;
    u8 i, j;
    u32 place;
    u8 digit = 10; //最大有效数字取10位 (根据情况修改,double型可设为15位)
    char *start = buf;

    ASSERT(buf!=NULL);

    if(IsNaN(value)){
        lib_strcpy(buf, "NAN");
        return start;
    }

    if(IsINF(value)){
        lib_strcpy(buf, "INF");
        return start;
    }

    if (ndigit > (digit - 1))
    {
        ndigit = digit - 1;
    }
    i = ndigit;

    while (i--) //考虑小数点位数
    {
        value = value * 10;
    }

    // 负号处理
    if (value < 0)
    {
        *buf++ = '-';
        start++;
        value = value * (-1);
    }

    // 防止越界，导致输出错误
    while (value > 0xffffffff)
    {
        value = value / 10;
        ndigit--;
    }

    temp = (u32) (value);

    //四舍五入处理
    if ((value - temp) >= 0.5)
    {
        temp = temp + 1;
    }

    i = digit - 1; //i由有效位数决定

    while (i)
    {
        j = i;
        place = 1;
        while (j--)
        {
            place = place * 10;
        }
        *buf = '0' + temp / place; //第i位
        if (*buf != '0' || start_digit == 1 || i == ndigit) //去掉首部的'0'值,
        //但小数点前面的'0'保留
        {
            buf++;
            start_digit = 1;
        }
        temp = temp % place;

        if (i == ndigit) //在小数点位置加入'.'
            *buf++ = '.';

        i--;
    }

    *buf++ = '0' + temp;
    *buf = '\0';

    /**** 去掉尾部的小数点  ********/
    if (ndigit != 0)
    {
        buf--;
        while (1)
        {
            if (*buf == '0')
            {
                buf--;
                ndigit--;
                if (ndigit == 1)
                { //小数点前的0留着
                    break;
                }
            }
            else
            {
                break;
            }
        }
        buf++;
        *buf = '\0';
    }
    return start;
}


/*******************************************************************************
 * Function Name  : lib_memcpy
 * Description    : 内存拷贝
 * Input          : dest 目的源  src 拷贝源  count 拷贝字节数
 * Output         : dest 拷贝目的源
 * Return         : None
 *******************************************************************************/
void *lib_memcpy(void *dest, const void *src, u32 count)
{
    u8 *tmp = (u8*)dest;
    const u8 *s = (u8*)src;

    ASSERT(dest != NULL);
    ASSERT(src != NULL);

    if(count == 0) return dest;

    while (count--)
    {
        *tmp++ = *s++;
    }
    return dest;
}

/*******************************************************************************
 * Function Name  : ConsolePortPrint
 * Description    : 控制台打印
 * Input          : buf 需要打印的字符串
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ConsolePortPrint1(char *buf)
{
    printf("mymymy%s", buf);
    syslog(LOG_INFO, "%s", buf);
}

/*******************************************************************************
 * Function Name  : ConsolePortPrint
 * Description    : 控制台打印
 * Input          : buf 需要打印的字符串
 * Output         : None
 * Return         : None
 *******************************************************************************/
void ConsolePortPrint(char *buf)
{
    printf("%s", buf);
 //   syslog(LOG_INFO, "%s", buf);
}

/*******************************************************************************
 * Function Name  : loadFormatUnit
 * Description    : 加载格式单元
 * Input          : format_unit 格式单元  format 格式字符串
 * Output         : format_unit 格式单元
 * Return         : u16 格式字符串长度
 *******************************************************************************/
u16 loadFormatUnit(TFormatUnit *format_unit, const char *format, lib_va_list *parg)
{
    u16 start = 0;

    format_unit->align_direct = 0; //0 右对齐  1左对齐
    format_unit->zero_space = 0; //0 填空格  1填'0'
    format_unit->align_len = 0; // 字符宽度，不包括符号
    format_unit->size_type = 0; //0 int型  1 整型的long 实数的double型   2 short型
    format_unit->digit_size = 0; //精度

    while (1)
    {
        format++;
        start++;

        if (*(format) == '-')
        {
            format_unit->align_direct = 1;
        }
        else if (*(format) == '0')
        {
            // -0 根据标准库的作法，－0在一起,0应该被忽略
            if (format_unit->align_direct == 0) {
                format_unit->zero_space = 1;
            }
        }
        else if ((*(format) <= '9') && (*(format) > '0'))
        {
            format_unit->align_len = *(format) - '0';
            if ((*(format + 1) <= '9') && (*(format + 1) >= '0'))
            {
                format_unit->align_len = (format_unit->align_len * 10)
                        + *(format + 1) - '0';
                format++;
                start++;
            }
        }
        else if (*(format) == '.' && (*(format + 1) <= '9') && (*(format + 1)
                > '0'))
        {
            format_unit->digit_size = *(format + 1) - '0';
            format++;
            start++;
        }
        else if (*(format) == 'l' || *(format) == 'h')
        {
            format_unit->size_type = *(format);

        }
        else if (*(format) == '*')
        {
            format_unit->align_len = (u32) lib_va_arg(*parg, u32);
        }
        else if (*(format) == '.' && (*(format + 1) == '*'))
        {
            format_unit->digit_size = (u32) lib_va_arg(*parg, u32);
            format++;
            start++;
        }
        else
        {
            format_unit->format_type = *(format);
            return start - 1;
        }
    }
    return 0;
}
/*******************************************************************************
 * Function Name  : lib_strlen
 * Description    : 获取字符串长度
 * Input          : str 输入字符串
 * Output         : None
 * Return         : u32 字符串长度
 *******************************************************************************/
u32 lib_strlen(const char *str)
{
    const char *c = str;

    ASSERT((str != NULL));

    while (*c++)
        ;
    return ((u32) (c - str - 1));
}

/*******************************************************************************
 * Function Name  : formatOutputString
 * Description    : 根据格式单元格式化输出数据字符串
 * Input          : format_unit 格式单元  format 格式字符串
 * Output         : format_unit 格式单元
 * Return         : u16 格式字符串长度
 *******************************************************************************/
u16 formatOutputString(TFormatUnit *format_unit, char *str_data)
{
    u8 fill_char = ' ';
    u8 sign_bit = 0;
    s32 i = 0;
    u16 temp = 0;
    u16 len = 0;
    u16 digit_size = 0;

    ASSERT(format_unit!=NULL);
    ASSERT(str_data!=NULL);

    len = lib_strlen(str_data);

    if (format_unit->zero_space == 1)
    {
        fill_char = '0';
    }

    if ((format_unit->format_type == 'd')
            || (format_unit->format_type == 'f')
            || (format_unit->format_type == 'x')
            || (format_unit->format_type == 'X')
            || (format_unit->format_type == 'o'))
    {
        if ((str_data[0] == '-') || (str_data[0] == '+'))
        {
            sign_bit = 1;
        }
    }

    if ((format_unit->digit_size != 0) && (format_unit->format_type == 'f'))
    {
        for(i = 0; i<len; i++){
            if(len-i-1 > 0){
                if(str_data[len-i-1] == '.'){
                    digit_size = i;
                }
            }
        }

        if(digit_size < (format_unit->digit_size)){
            for (i = 0; i < ((format_unit->digit_size) - digit_size); i++) {
                if((len + i) < MAX_OUTPUT_BUFFERSIZE-2){
                    str_data[len+i] = '0';
                    temp++;
                }else{
                    break;
                }
            }
            str_data[len + temp] = '\0';
            len += temp;
        }else if(digit_size > (format_unit->digit_size)){
            temp = digit_size - (format_unit->digit_size);

            if(len > temp){
                len -= temp;
            }
        }
        temp = 0;
    }else if ((format_unit->digit_size != 0)
            && ((format_unit->format_type == 's') || (format_unit->format_type == 'a')))
    {
         if (len > format_unit->digit_size)
         {
             len = format_unit->digit_size;
             temp = 0;
         }
    }else if ((format_unit->digit_size != 0) && (format_unit->format_type != 'f'))
    {
         if ((len - sign_bit) < format_unit->digit_size)
         {
             for (i = 1; i < len - sign_bit + 1; i++)
             {
                 if(format_unit->digit_size - i + sign_bit > 0){
                     if((format_unit->digit_size - i + sign_bit) < MAX_OUTPUT_BUFFERSIZE-2){
                         str_data[format_unit->digit_size - i + sign_bit] = str_data[len - i];
                     }else{
                         break;
                     }
                 }
             }

             for (i = 0; i < (format_unit->digit_size - len + sign_bit); i++)
             {
                 if(format_unit->digit_size - len + sign_bit > 0){
                     if((i + sign_bit) < MAX_OUTPUT_BUFFERSIZE-2){
                         str_data[i + sign_bit] = '0';
                         temp++;
                     }else{
                         break;
                     }
                 }
             }

             len += temp;
             temp = 0;
         }
     }

    if ((format_unit->align_len != 0) && (len < format_unit->align_len))
    {
        // 左对齐模式
        if (format_unit->align_direct == 0)
        {
            // 带符号位且填充0的处理
            if ((sign_bit == 1) && (format_unit->zero_space == 1)) {
                for (i = 1; i < (len - sign_bit + 1); i++)
                {
                    if(format_unit->align_len - i > 0){
                        if((format_unit->align_len - i) < MAX_OUTPUT_BUFFERSIZE-2){
                            str_data[format_unit->align_len - i] = str_data[len - i];
                        }else{
                            break;
                        }
                    }else{
                        break;
                    }
                }
                for (i = sign_bit; i < (format_unit->align_len - len + sign_bit); i++)
                {
                    if((i) < MAX_OUTPUT_BUFFERSIZE-2){
                        str_data[i] = fill_char;
                        temp++;
                    }else{
                        break;
                    }
                }
            }else{// 非填充0的处理
                for (i = 1; i < (len + 1); i++)
                {
                    if((format_unit->align_len - i) > 0){
                        if((format_unit->align_len - i) < MAX_OUTPUT_BUFFERSIZE-2){
                            str_data[format_unit->align_len - i] = str_data[len - i];
                        }
                    }
                }

                for (i = 0; i < (format_unit->align_len - len); i++)
                {
                    if((i) < MAX_OUTPUT_BUFFERSIZE){
                        str_data[i] = fill_char;
                        temp++;
                    }else{
                        break;
                    }
                }
            }
        }
        // 右对齐模式
        else if (format_unit->align_direct == 1)
        {
            for (i = 0; i < (format_unit->align_len - len); i++)
            {
                if((len + temp) < MAX_OUTPUT_BUFFERSIZE-2){
                    str_data[len + temp] = fill_char;
                    temp++;
                }else{
                    break;
                }
            }
        }
        len += temp;
    }

    if((len) > MAX_OUTPUT_BUFFERSIZE-1){
        len = MAX_OUTPUT_BUFFERSIZE - 1;
    }
    str_data[len] = '\0';

    return len;
}

/*******************************************************************************
 * Function Name  : lib_uitoa
 * Description    : 无符号整数按进制转字符串
 * Input          : num 整数 *str 字符串 radix基数
 * Output         : *str 目的字符串
 * Return         : u8 字符串长度
 *******************************************************************************/
char *lib_uitoa(u32 num, char *str, u16 radix)
{
    u8 string[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* ptr = str;
    u16 start = 0;
    u16 i = 0;
    u16 j = 0;
    u16 k = 0;

    ASSERT(str!=NULL);

    if(radix==0)
    {
        radix = 10;
    }

    if (num == 0)
    {
        str[0] = '0';
        str[1] = 0;
        return str;
    }

    if ((num < 10) && (num > 0))
    {
        str[0] = '0' + num;
        str[1] = 0;
        return str;
    }

    while (num)
    {
        k = num - (num / radix) * radix;
        *ptr++ = string[k];
        num /= radix;

        if (num < radix)
        {
            if (num != 0)
            {
                *ptr++ = string[num];
            }
            *ptr = '\0';
            break;
        }
    }

    j = ptr - str - 1;

    for (i = start; i < (ptr - str) / 2; i++)
    {
        u16 temp = str[i];
        str[i] = str[j];
        str[j--] = temp;
    }

    return str;
}


/*******************************************************************************
 * Function Name  : lib_itoa
 * Description    : 整数按进制转字符串
 * Input          : num 整数 *str 字符串 radix基数
 * Output         : *str 目的字符串
 * Return         : u8 字符串长度
 *******************************************************************************/
char *lib_itoa(s32 num, char *str, u16 radix)
{
    u8 string[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* ptr = str;
    u16 start = 0;
    u16 i = 0;
    u16 j = 0;
    u16 k = 0;

    ASSERT(str!=NULL);

    if(radix==0)
    {
        radix = 10;
    }

    if (num < 0)
    {
        str[0] = '-';
        num *= -1;
        start++;
        ptr++;
    }

    if ((num < 10) && (num >= 0))
    {
        str[start] = '0' + num;
        str[start + 1] = 0;
        return str;
    }

    while (num)
    {
        k = num - (num / radix) * radix;
        *ptr++ = string[k];
        num /= radix;

        if (num < radix)
        {
            if (num != 0)
            {
                *ptr++ = string[num];
            }
            *ptr = '\0';
            break;
        }
    }

    j = ptr - str - 1;

    for (i = start; i < ((ptr - str + start) / 2); i++)
    {
        u16 temp = str[i];
        str[i] = str[j];
        str[j--] = temp;
    }

    return str;
}


u8 CountCS(u8* pData,u16 usLen)
{
    u8 cs = 0;
    u16 i=0;
    for(i=0; i < usLen; i++)
    {
       cs += *pData++;
    }
    
    return cs;    
}

/*******************************************************************************
 * Function Name  : bcd_byte_encode
 * Description    : 十进制数转BCD码
 * Input          : num 十进制数
 * Output         : None
 * Return         : u8 BCD码
 *******************************************************************************/
u8 byte2bcd(u8 num)
{
    return ((num / 10) << 4) | (num % 10);
}

/*******************************************************************************
 * Function Name  : bcd_byte_decode
 * Description    : BCD码转十进制数
 * Input          : bcd BCD码
 * Output         : None
 * Return         : u8 十进制数
 *******************************************************************************/
u8 bcd2byte(u8 bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0x0f);
}

u8* lib_reverse(u8* data, int len)
{
	u8 buf[1024] = {0};
	int i = 0;
	int pos = 0;
	int n;

	for(i=0;i<len;i++)
		buf[i] = data[i];
	memset(data,0,len);

	for(i=0;i<len;i++)
		data[i] = buf[len-1-i];

	for(i=0;i<len;i++)
		if(data[i] != 0)
			break;

	memset(buf,0,sizeof(buf));
	n=i;
	for(i=n;i<len;i++)
		buf[pos++] = data[i];

	memset(data,0,len);
	memcpy(data,buf,pos);

	return data;
}

/*******************************************************************************
 * Function Name  : bcd2uint
 * Description    : BCD码转整数
 * Input          : *data BCD 码数组  data_len 数组长度
 * Output         : None
 * Return         : u32 整数结果
 *******************************************************************************/
u32 bcdtouint(const u8* psrc_buf, u32 data_len)
{
    s8 i = 0;
    u32 re = 0;

    if(data_len > 0)
	{
    	for (i = data_len - 1; i >= 0; i--)
    	{
        	re = re * 100 + (psrc_buf[i] & 0x0f) + ((psrc_buf[i] >> 4) & 0x0f) * 10;
    	}
	}
    return re;
}
/*******************************************************************************
 * Function Name  : bcd2sint
 * Description    : BCD码转整数(带符号位）
 * Input          : *data BCD 码数组  data_len 数组长度
 * Output         : None
 * Return         : s32 有符号整数结果
 *******************************************************************************/
s32 bcdtosint(const u8* psrc_buf, u32 data_len)
{
    s8 i = 0;
    s32 re = 0;
    u8 tem;
	  u8 sign=0;
	  if(data_len==0)
			return 0;
	  tem=psrc_buf[data_len-1];
	  if(tem & 0x80)
		{
			sign=1;
			tem &=0x7f;
		}
		re = (tem & 0x0f) + ((tem >> 4) & 0x0f) * 10;
    if(data_len>1)
		{
    	for (i = data_len - 2; i >= 0; i--)
    	{
        	re = re * 100 + (psrc_buf[i] & 0x0f) + ((psrc_buf[i] >> 4) & 0x0f) * 10;
    	}
		}
		if(sign==1)
			re=-re;

    return re;
}

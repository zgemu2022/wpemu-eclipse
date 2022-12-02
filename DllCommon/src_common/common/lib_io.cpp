/*
 * lib_io.c
 *
 *  Created on: 2017-12-19
 *      Author: root
 */

#include "lib_io.h"
/*******************************************************************************
 * Function Name  : lib_sprintf
 * Description    : None
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
u16 lib_vsprintf(char* buf, char* out_buf, const char *format, lib_va_list arg, boolean float_fix)
{
    char* str_data = 0;
    u32 len = 0;
    u8 u8_value = 0;
    u8 left_size = 0; //todo stm32移植时需要修改此值
    u32 u32_value = 0;
    s32 s32_value = 0;
    fp64 fp64_value = 0;
    u32 pos = 0;
    TFormatUnit format_unit;
    ASSERT(format != NULL);

    if(float_fix){
        left_size = 4;
    }else{
        left_size = 0;
    }
    while((*format != '\0') && (pos < MAX_OUTPUT_PRINT_SIZE))
    {
        if (*format == '%')
        {
            format += loadFormatUnit(&format_unit, format, &arg);

            switch (*(format + 1))
            {
            case 'c':
                if (left_size == 4)
                {
                    left_size = 0;
                }
                else
                {
                    left_size = 4;
                }
                u8_value = (u8) lib_va_arg(arg, u8);
                out_buf[pos++] = u8_value;
                format += 2;
                break;

            case 'd':
                if (left_size == 4)
                {
                    left_size = 0;
                }
                else
                {
                    left_size = 4;
                }

                s32_value = lib_va_arg(arg, s32);
                lib_itoa(s32_value, buf, 10);

                formatOutputString(&format_unit, buf);
                str_data = buf;
                while ((*str_data != '\0')&&(pos < MAX_OUTPUT_PRINT_SIZE))//判断是否溢出
                {
                    out_buf[pos++] = *str_data;
                    str_data++;
                }
                format += 2;
                break;

            case 'u':
                if (left_size == 4)
                {
                    left_size = 0;
                }
                else
                {
                    left_size = 4;
                }

                u32_value = lib_va_arg(arg, u32);
                lib_uitoa(u32_value, buf, 10);
                formatOutputString(&format_unit, buf);
                str_data = buf;
                while ((*str_data != '\0') && (pos < MAX_OUTPUT_PRINT_SIZE))//判断是否溢出
                {
                    out_buf[pos++] = *str_data;
                    str_data++;
                }
                format += 2;
                break;
            case 'o':
                if (left_size == 4)
                {
                    left_size = 0;
                }
                else
                {
                    left_size = 4;
                }

                s32_value = lib_va_arg(arg, s32);
                lib_itoa(s32_value, buf, 8);
                formatOutputString(&format_unit, buf);
                str_data = buf;
                while ((*str_data != '\0') && (pos < MAX_OUTPUT_PRINT_SIZE))//判断是否溢出
                {
                    out_buf[pos++] = *str_data;
                    str_data++;
                }
                format += 2;
                break;

            case 'X':
            case 'x':
                if (left_size == 4)
                {
                    left_size = 0;
                }
                else
                {
                    left_size = 4;
                }

                s32_value = lib_va_arg(arg, u32);
                lib_itoa(s32_value, buf, 16);
                formatOutputString(&format_unit, buf);
                str_data = buf;
                while ((*str_data != '\0')&&(pos < MAX_OUTPUT_PRINT_SIZE))//判断是否溢出
                {
                    if (*(format + 1) == 'X'
                            && ((*str_data >= 'a') && (*str_data <= 'f')))
                    {
                        *str_data -= 32;
                    }
                    out_buf[pos++] = *str_data;
                    str_data++;
                }
                format += 2;
                break;

            case 's':
                str_data = lib_va_arg(arg, char*);
                len = lib_strlen(str_data) + 1; // 复制包括最后的'\0'

                // 超过字符串输出最大缓存值处理
                if(len >= MAX_OUTPUT_BUFFERSIZE)
                {
                    len = MAX_OUTPUT_BUFFERSIZE - 1;// 只复制255个字符
                    *(buf + len) = '\0'; //填写字符串结束符
                }

                lib_memcpy(buf, str_data, len);

                formatOutputString(&format_unit, buf);
                str_data = buf;

                while ((*str_data != '\0') && (pos < MAX_OUTPUT_PRINT_SIZE))//判断是否溢出
                {
                    out_buf[pos++] = *str_data;
                    str_data++;
                }
                format += 2;
                break;

			//16进制字符串
            case 'a':
                str_data = lib_va_arg(arg, char*);
                len = format_unit.digit_size;
                // 超过字符串输出最大缓存值处理
                if(len >= MAX_OUTPUT_BUFFERSIZE)
                {

                //	 printf("超过字符串输出最大缓存值处理  len=%d   MAX_OUTPUT_BUFFERSIZE=%d  MAX_OUTPUT_PRINT_SIZE=%d \r\n",(int)len,MAX_OUTPUT_BUFFERSIZE,MAX_OUTPUT_PRINT_SIZE);
                    len = MAX_OUTPUT_BUFFERSIZE - 1;// 只复制255个字符
                    *(buf + len) = '\0'; //填写字符串结束符
                }

                lib_memcpy(buf, str_data, len);

                formatOutputString(&format_unit, buf);
                str_data = buf;

                while (len > 0)
                {
                	if(pos < MAX_OUTPUT_PRINT_SIZE - 2)//判断是否溢出 -2
               		{
	                    out_buf[pos++] = hex2char(((*str_data) >> 4) & 0x0f);
	                    out_buf[pos++] = hex2char((*str_data) & 0x0f);
	                    out_buf[pos++] = ' ';
	                    str_data++;
	                    len--;
               		}
					else
						break;//直接break
                }
                format += 2;
                break;

            case 'f':
                if (left_size % 8 != 0)
                {
                    arg += 4;
                    left_size = 0;
                }

                fp64_value = lib_va_arg(arg, fp64);

                if(format_unit.digit_size == 0){
                    lib_gcvt(fp64_value, 6, buf); //未指定精度时，隐含精度为小数点后6
                }else{
                    lib_gcvt(fp64_value, format_unit.digit_size, buf); //未指定精度时，隐含精度为小数点后6
                }
                formatOutputString(&format_unit, buf);
                str_data = buf;

                while (*str_data != '\0')
                {
                    out_buf[pos++] = *str_data;
                    str_data++;
                }
                format += 2;
                break;
            case '%':
                out_buf[pos++] = '%';
                format += 2;
                break;
            default:
                out_buf[pos++] = *(format++);
                break;
            }
        }
        else
        {
            out_buf[pos++] = *(format++);
        }
    }
	if(*format != '\0')
	{
		//输出3个*，代表长度太长，未打印完
	    out_buf[pos++] = '*';
		out_buf[pos++] = '*';
	    out_buf[pos++] = '*';
		out_buf[pos++] = '\n';//自动附加一个换行
	}
    out_buf[pos] = '\0';

    return pos;
}
///*******************************************************************************
// * Function Name  : lib_vprintf
// * Description    : 支持格式  %-0lm.n %d %X %x %o %u %s %c %f 完全按照标准C库
// * Input          : None
// * Output         : None
// * Return         : None
// *******************************************************************************/
//u16 lib_vprintf1(const char *format, lib_va_list arg,FILE * fp)
//{
//    u16 len = 0;
//
//    ASSERT(format != NULL);
//  //  ASSERT(arg != NULL);
//    len = lib_vsprintf(Str_Buffer, OutBuffer, format, arg, True);
////    len = lib_vsprintf(Str_Buffer1, OutBuffer1, format, arg, True);
//    printf("打印33%s\r\n",OutBuffer);
//	fprintf(fp, OutBuffer);
//
//    return len;
//}

/*******************************************************************************
 * Function Name  : lib_vprintf
 * Description    : 支持格式  %-0lm.n %d %X %x %o %u %s %c %f 完全按照标准C库
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
u16 lib_vprintf1(const char *format, lib_va_list arg)
{
    u16 len = 0;

    ASSERT(format != NULL);
    ASSERT(arg != NULL);

    len = lib_vsprintf(Str_Buffer, OutBuffer, format, arg, True);
    ConsolePortPrint1(OutBuffer);

    return len;
}

/*******************************************************************************
 * Function Name  : lib_vprintf
 * Description    : 支持格式  %-0lm.n %d %X %x %o %u %s %c %f 完全按照标准C库
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
u16 lib_vprintf(const char *format, lib_va_list arg)
{
    u16 len = 0;

    ASSERT(format != NULL);
    ASSERT(arg != NULL);

    len = lib_vsprintf(Str_Buffer, OutBuffer, format, arg, True);
    ConsolePortPrint(OutBuffer);

    return len;
}
//u16 lib_printf1(FILE* fp,const char *format, ...)
//{
//    lib_va_list arg = NULL;
//    u16 len = 0;
//
//    ASSERT(format != NULL);
//
//    lib_va_start (arg, format);
//    len = lib_vprintf1(format, arg,fp);
//    lib_va_end(arg);
//
//    return len;
//}
/*******************************************************************************
 * Function Name  : lib_printf
 * Description    : 支持格式  %-0lm.n %d %X %x %o %u %s %c %f 完全按照标准C库
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/

u16 lib_printf(const char *format, ...)
{
    lib_va_list arg = NULL;
    u16 len = 0;

    ASSERT(format != NULL);

    lib_va_start (arg, format);
    len = lib_vprintf(format, arg);
    lib_va_end(arg);

    return len;
}


/*******************************************************************************
 * Function Name  : lib_sprintf
 * Description    : 支持格式  %-0lm.n %d %X %x %o %u %s %c %f 完全按照标准C库
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
u16 lib_sprintf(char* buf, const char *format, ...)
{
    lib_va_list arg = NULL;
    u16 len = 0;
    char buffer[74];//打印输出缓冲，超过最大忽略后面的内容

    ASSERT(format != NULL);

    if(lib_strlen(buf) > 60){
        for (len = 0; len < 200; ++len) {
            lib_printf("lib_sprintf over flow!! len:%d str:%s\n",lib_strlen(buf), buf);
        }
        buf[60] = '\n';
    }

    lib_va_start(arg, format);
    len = lib_vsprintf(buffer, buf, format, arg, False);
    lib_va_end(arg);

    return len;
}

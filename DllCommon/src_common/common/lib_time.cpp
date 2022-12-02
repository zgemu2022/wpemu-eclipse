/*
 * lib_time.c
 *
 *  Created on: 2017-12-19
 *      Author: root
 */

#include "lib_time.h"

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include "tools.h"
const u8 DayofMonth[12] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//每月的天数

/*the seconds of round year = 3600*24*366 */
#define SECONDOFROUNDYEAR 31622400

/*the seconds of general year = 3600*24*365 */
#define SECONDOFYEAR 31536000
bool IsRound(int year)
{
    /*is round year?*/
    if((year%100)&&(year%4==0)) return 1;
    if((year%100==0)&&(year%400==0)) return 1;
    return 0;
}




u8 countWeek(TDateTime *datetime)
{
	 u8 month[]={13,14,3,4,5,6,7,8,9,10,11,12};
	 u8 year[]={1,1,0,0,0,0,0,0,0,0,0,0};
	 int W;
	 u8 p[3];
	 u8 week;
	 p[0]=datetime->Year-2000;
	 p[1]=datetime->Month;
	 p[2]=datetime->Day;
	 W=(p[2]-year[p[1]-1])+(p[2]-year[p[1]-1])/4+20/4-2*20+(26*(month[p[1]-1]+1))/10+p[0]-1;
	 W %= 7;
	 if(W<=0)
		  week=7+W;
	 else
		 week=W;
     datetime->Week=week;
	 return week;
	 //	W = y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
//	 49+49/4+20/4-2*20+(26*(10+1))/10+1-1=5
	 //中华人民共和国成立100周年(2049年10月1日)那天是周5

}
u8 countWeek(u8 *datetime)
{
	TDateTime DT;
	u16 ms;
	 u8 week;
	 ms=datetime[0]+datetime[1]*256;
	 DT.Sec=ms/1000;
	 DT.Min=datetime[2];
	 DT.Hour=datetime[3];
	 DT.Day=datetime[4];
	 DT.Month=datetime[5];
	 DT.Year=datetime[6];
	 week=countWeek(&DT);
	 return week;


}


/*******************************************************************************
 * Function Name  : read_current_datetime
 * Description    : 读取当前时间
 * Input          : datetime 获取当前时间数据体
 * Output         : datetime 当前时间
 * Return         : None
 *******************************************************************************/
void read_current_datetime(TDateTime *datetime)
{
	struct timeval tv;
	struct tm* timenow;

	gettimeofday(&tv, NULL);
	timenow = localtime(&tv.tv_sec);
	datetime->Year=timenow->tm_year+1900;
	datetime->Month=timenow->tm_mon+1;
	datetime->Day=timenow->tm_mday;
	datetime->Week =timenow->tm_wday ;
	datetime->Hour= timenow->tm_hour;
	datetime->Min= timenow->tm_min;
	datetime->Sec= timenow->tm_sec;
	datetime->Msec=tv.tv_usec/1000;

}

//把4位数的年份，转换成2位数的bcd码
u8 year2bcd(u16 year)
{
	return byte2bcd(year % 100);
}

//把4位数的年份，转换成2位数的bcd码
u16 byte2year(u8 year)
{
    TDateTime now;
    read_current_datetime(&now);

    return ((now.Year / 100)*100 + year);
}
/*******************************************************************************
 * Function Name  : diff_DateTime_Month
 * Description    : 获取datetime1与datetime2的时间差（单位：月）
 * Input          : datetime1 基准时间 datetime2 对比时间
 * Output         : None
 * Return         : s32 相差的月数  > 0(datetime1早于 datetime2) > 0 (datetime1晚于 datetime2)
 *******************************************************************************/
s32 diff_DateTime_Month(TDateTime *datetime1, TDateTime *datetime2)
{
    s32 re = 0;

    re = (datetime2->Year - datetime1->Year) * 12 + datetime2->Month
            - datetime1->Month;
    return re;
}
/*******************************************************************************
 * Function Name  : getDayofYear
 * Description    : 计算某年的天数
 * Input          : year 某年
 * Output         : None
 * Return         : u8 天数
 *******************************************************************************/
u16 getDayofYear(u16 year)
{
    if (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0)
    {
        return 366;//闰年
    }
    else
    {
        return 365;
    }
}

/*******************************************************************************
 * Function Name  : getDayofMonth
 * Description    : 计算某年某月的天数
 * Input          : year 某年  month 某月
 * Output         : None
 * Return         : u8 天数
 *******************************************************************************/
u8 getDayofMonth(u16 year, u8 month)
{
    if (2 != month)
        return DayofMonth[month - 1];
    else
    {
        //为4的倍数但是不是100的倍数
        //或者是400的倍数
        if (((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0)
            return 29;
        else
            return 28;
    }
}

/*******************************************************************************
 * Function Name  : getCurDayOfYear
 * Description    : 计算从该年开始到该年某月某日为止的天数
 * Input          : year 某年 month 某月  day 某日
 * Output         : None
 * Return         : u16 天数
 *******************************************************************************/
u16 getCurDayOfYear(u16 year, u8 month, u8 day)
{
    u32 i;
    u16 temp_day = 0;

    for (i = 1; i < month; i++)
    {
        temp_day += getDayofMonth(year, i);
    }

    return (temp_day + day);
}



/*******************************************************************************
 * Function Name  : diff_DateTime_Day
 * Description    : 获取datetime1与datetime2的时间差（单位：天）
 * Input          : datetime1 基准时间 datetime2 对比时间
 * Output         : None
 * Return         : s32 相差的天数  > 0(datetime1早于 datetime2) > 0 (datetime1晚于 datetime2)
 *******************************************************************************/
s32 diff_DateTime_Day(TDateTime *datetime1, TDateTime *datetime2)
{
    u16 i;
    s32 re = 0;
    s32 diff_day = 0;

    if (datetime1->Year > datetime2->Year)
    {
        for (i = datetime2->Year; i < datetime1->Year; i++)
        {
            re += getDayofYear(i);
        }

        re *= -1;
    }
    else
    {
        for (i = datetime1->Year; i < datetime2->Year; i++)
        {
            re += getDayofYear(i);
        }
    }

    diff_day = getCurDayOfYear(datetime2->Year, datetime2->Month,
            datetime2->Day) - getCurDayOfYear(datetime1->Year,
            datetime1->Month, datetime1->Day);

    re += diff_day;

    return re;
}

/*******************************************************************************
 * Function Name  : diff_DateTime_Hour
 * Description    : 获取datetime1与datetime2的时间差（单位：小时）
 * Input          : datetime1 基准时间 datetime2 对比时间
 * Output         : None
 * Return         : s32 相差的小时数  > 0(datetime1早于 datetime2) > 0 (datetime1晚于 datetime2)
 *******************************************************************************/
s32 diff_DateTime_Hour(TDateTime *datetime1, TDateTime *datetime2)
{
    s32 re = 0;

    //求出天数
    re = diff_DateTime_Day(datetime1, datetime2);

    //求出小时
    re = re * 24 + datetime2->Hour - datetime1->Hour;

    return re;
}

/*******************************************************************************
 * Function Name  : diff_DateTime_Min
 * Description    : 获取datetime1与datetime2的时间差（单位：分） datetime2 - datetime1
 * Input          : datetime1 基准时间 datetime2 对比时间
 * Output         : None
 * Return         : s32 相差的分钟数  >0(datetime1早于 datetime2)   <0(datetime1晚于 datetime2)
 *******************************************************************************/
s32 diff_DateTime_Min(TDateTime *datetime1, TDateTime *datetime2)
{
    s32 re = 0;

    re = diff_DateTime_Hour(datetime1, datetime2);

    re = re * 60 + datetime2->Min - datetime1->Min;
    return re;
}

/*******************************************************************************
 * Function Name  : diff_DateTime_Sec
 * Description    : 获取datetime1与datetime2的时间差（单位：秒）
 * Input          : datetime1 基准时间 datetime2 对比时间
 * Output         : None
 * Return         : s32 相差的秒数  > 0(datetime1早于 datetime2) <0 (datetime1晚于 datetime2)
 *******************************************************************************/
s32 diff_DateTime_Sec(TDateTime *datetime1, TDateTime *datetime2)
{
    s32 re = 0;

    re = diff_DateTime_Min(datetime1, datetime2);
    //求秒数
    re = re * 60 + datetime2->Sec - datetime1->Sec;
    return re;
}

/*******************************************************************************
 * Function Name  : add_DateTime_Day
 * Description    : datetime的day天后的时间
 * Input          : datetime 基准时间  day 后移天数
 * Output         : datetime datetime的day天后的时间
 * Return         : None
 *******************************************************************************/
void add_DateTime_Day(TDateTime *datetime, u32 day)
{
    u8 month, _day;
    u32 temp;
    u16 year;

    year = datetime->Year;
    month = datetime->Month;
    _day = datetime->Day;

    temp = day + _day;//temp此时代表天数

    if (temp > getDayofMonth(year, month))//如果大于本月的天数，月份需要进位
    {
        //有可能跨几个月,需要1个月1个月的计算，因为每个月的天数不同
        while (temp > getDayofMonth(year, month))
        {
            temp = temp - getDayofMonth(year, month);

            month++;
            if (month > 12)//跨年
            {
                month = month % 12;
                year++;
            }
        }
    }
    _day = temp;

    datetime->Year = year;
    datetime->Month = month;
    datetime->Day = _day;
}

/*******************************************************************************
 * Function Name  : add_DateTime_Month
 * Description    : datetime的mon月后的时间
 * Input          : datetime 基准时间  mon后移月数
 * Output         : datetime datetime的 mon月后的时间
 * Return         : None
 *******************************************************************************/
void add_DateTime_Month(TDateTime *datetime, u32 mon)
{
    u8 month;
    u16 year;
    u32 temp;

    year = datetime->Year;
    month = datetime->Month;

    temp = month + mon;

    //只考虑temp<=24的情况
    while (1)
    {
        if (temp > 12)
        {
            temp -= 12;//月
            year++;//年
        }
        else
        {
            month = temp;
            break;
        }
    }
    datetime->Year = year;
    datetime->Month = month;
}

/*******************************************************************************
 * Function Name  : add_DateTime_Min
 * Description    : datetime的minute分钟后的时间
 * Input          : datetime 基准时间  minute后移分钟数
 * Output         : datetime的minute分钟后的时间
 * Return         : None
 *******************************************************************************/
void add_DateTime_Min(TDateTime *datetime, u32 minute)
{
    u8 month, day, hour, min;
    u16 year;
    u32 temp;

    year = datetime->Year;
    month = datetime->Month;
    day = datetime->Day;
    hour = datetime->Hour;
    min = datetime->Min;

    temp = min + minute;
    min = temp % 60;

    temp = temp / 60;//得到小时数

    if (temp > 0)//小时进位
    {
        temp = temp + hour;
        hour = temp % 24;

        temp = temp / 24; //得到天数

        if (temp > 0)//天数改变了
        {
            temp = temp + day;//temp此时代表天数

            if (temp > getDayofMonth(year, month))//如果大于本月的天数，月份需要进位
            {
                //有可能跨几个月,需要1个月1个月的计算，因为每个月的天数不同
                while (temp > getDayofMonth(year, month))
                {
                    temp = temp - getDayofMonth(year, month);

                    month++;
                    if (month > 12)//跨年
                    {
                        month = month % 12;
                        year++;
                    }
                }
                day = temp;
            }
            else
            {
                day = temp;
            }
        }
    }

    datetime->Year = year;
    datetime->Month = month;
    datetime->Day = day;
    datetime->Hour = hour;
    datetime->Min = min;
}
void add_DateTime_Sec(TDateTime *datetime, u32 sec)
{
	u32 min =(datetime->Sec + sec) / 60;
	datetime->Sec = (datetime->Sec + sec) % 60;
    add_DateTime_Min(datetime, min);
}
/*******************************************************************************
 * Function Name  : add_DateTime_Hour
 * Description    : datetime的minute分钟后的时间
 * Input          : datetime 基准时间  minute后移分钟数
 * Output         : datetime的minute分钟后的时间
 * Return         : None
 *******************************************************************************/
void add_DateTime_Hour(TDateTime *datetime, u32 hour)
{
    add_DateTime_Min(datetime, hour*60);
}


/*******************************************************************************
 * Function Name  : add_DateTime_Month
 * Description    : datetime的mon月前的时间
 * Input          : datetime 基准时间  mon前移月数
 * Output         : datetime datetime的 mon月前的时间
 * Return         : None
 *******************************************************************************/
void dec_DateTime_Month(TDateTime *datetime, u32 mon)
{
    u8 month;
    u16 year;
    s32 temp = 0;

    year  = datetime->Year;
    month = datetime->Month;

    temp = month - mon;

    while(temp <= 0)
    {
        year--;
        temp += 12;
    }

    month = temp;

    datetime->Year  = year;
    datetime->Month = month;
}

/*******************************************************************************
 * Function Name  : add_DateTime_Day
 * Description    : datetime的day天前的时间
 * Input          : datetime 基准时间  day 前移天数
 * Output         : datetime datetime的day天前的时间
 * Return         : None
 *******************************************************************************/
void dec_DateTime_Day(TDateTime *datetime, u32 day)
{
    s32 temp;
    u16 year;

    temp = datetime->Day - day;

    //有可能跨几个月,需要1个月1个月的计算，因为每个月的天数不同
    while (temp <= 0)
    {
        dec_DateTime_Month(datetime, 1);

        year = datetime->Year;
        temp += getDayofMonth(year, datetime->Month);
    }

    datetime->Day = temp;
}
/*******************************************************************************
 * Function Name  : add_DateTime_Day
 * Description    : datetime的hour小时前的时间
 * Input          : datetime 基准时间  hour 小时前移天数
 * Output         : datetime datetime的hour小时前的时间
 * Return         : None
 *******************************************************************************/
void dec_DateTime_Hour(TDateTime *datetime, u32 hour)
{
    s32 temp;

    temp = datetime->Hour - hour;

    //有可能跨几个月,需要1个月1个月的计算，因为每个月的天数不同
    while (temp < 0)
    {
        temp += 24;

        dec_DateTime_Day(datetime, 1);
    }

    datetime->Hour = temp;
}

/*******************************************************************************
 * Function Name  : dec_DateTime_Min
 * Description    : datetime的minute分钟前的时间
 * Input          : datetime 基准时间  minute前移分钟数
 * Output         : datetime的minute分钟前的时间
 * Return         : None
 *******************************************************************************/
void dec_DateTime_Min(TDateTime *datetime, u32 minute)
{
    s32 temp = 0;

    temp = datetime->Min - minute;

    while(temp < 0)
    {
        temp += 60;
        dec_DateTime_Hour(datetime, 1);
    }

    datetime->Min = temp;
}
void dec_DateTime_Sec(TDateTime *datetime, u32 sec)
{
    s32 temp = 0;

    temp = datetime->Sec - sec;

    while(temp < 0)
    {
        temp += 60;
        dec_DateTime_Min(datetime, 1);
    }

    datetime->Sec = temp;
}

/*******************************************************************************
 * Function Name  : cmp_DateTime
 * Description    : 比较日期时间大小
 * Input          : datetime1 对比基准时间  datetime2 对比时间
 * Output         : None
 * Return         : s32 = 0 (datetime1 等于 datetime2) < 0 (datetime1早于 datetime2) > 0 (datetime1晚于 datetime2)
 *******************************************************************************/
s32 cmp_DateTime(TDateTime *datetime1, TDateTime *datetime2)
{

	printf("1111111111111111111\r\n");
    if (datetime1->Year > datetime2->Year) //年
        return 1;
    else if (datetime1->Year < datetime2->Year)
        return -1;

    if (datetime1->Month > datetime2->Month) //月
        return 1;
    else if (datetime1->Month < datetime2->Month)
        return -1;

    printf("2222222222222222222222\r\n");
    printf("datetime1->Day=%d  datetime2->Day=%d",datetime1->Day,datetime2->Day);
    if (datetime1->Day > datetime2->Day) //日
        return 1;
    else if (datetime1->Day < datetime2->Day)
        return -1;

    if (datetime1->Hour > datetime2->Hour) //时
        return 1;
    else if (datetime1->Hour < datetime2->Hour)
        return -1;

    if (datetime1->Min > datetime2->Min) //分
        return 1;
    else if (datetime1->Min < datetime2->Min)
        return -1;

    if (datetime1->Sec > datetime2->Sec) //分
        return 1;
    else if (datetime1->Sec < datetime2->Sec)
        return -1;

    return 0;
}

/*******************************************************************************
 * Function Name  : max_3DateTime
 * Description    : 3个时间取最晚时间
 * Input          : datetime1 对比时间1  datetime2 对比时间2 datetime3 对比时间3
 * Output         : None
 * Return         : TDateTime* 最晚的时间指针
 *******************************************************************************/
TDateTime *max_3DateTime(TDateTime *datetime1, TDateTime *datetime2,
        TDateTime *datetime3)
{

    if (cmp_DateTime(datetime1, datetime2) >= 0)
    {

        if (cmp_DateTime(datetime1, datetime3) >= 0)
        {
            return datetime1;
        }
        else
        {
            return datetime3;
        }
    }
    else
    {
        if (cmp_DateTime(datetime2, datetime3) >= 0)
        {
            return datetime2;
        }
        else
        {
            return datetime3;
        }
    }
}

/*******************************************************************************
 * Function Name  : fix_DateTime_Min
 * Description    : 取datetime规整min后的时间  (10:10 规整 15 -> 10:15)
 * Input          : datetime 基准时间  minute 规整倍数
 * Output         : datetime 规整后的基准时间
 * Return         : None
 *******************************************************************************/
void fix_DateTime_Min(TDateTime *datetime, u8 minute)
{
    u32 temp;
    u8 min;

    min = datetime->Min;

    temp = minute - (min % minute);
    if (temp != minute)
        add_DateTime_Min(datetime, temp);
}

/*******************************************************************************
 * Function Name  : DateTime2Bcd
 * Description    : datetime转Bcd字节数组
 * Input          : datetime 基准时间  bcd Bcd数组 format 时间格式定义
 * Output         : bcd 时间Bcd数组
 * Return         : None
 *******************************************************************************/
//void DateTime2Bcd(TDateTime *datetime, u8 *bcd, TDFormatType format)
void DateTime2Bcd(TDateTime *datetime, u8 *bcd, u8 format)
{
    u8 i = 0;
    u8 pos = 0;
    u8 buf[7];

    if (format & YY)
    {
        *(buf + pos) = byte2bcd(datetime->Year % 100);
        pos++;
    }

    if (format & MM)
    {
        *(buf + pos) = byte2bcd(datetime->Month);
        pos++;
    }

    if (format & DD)
    {
        *(buf + pos) = byte2bcd(datetime->Day);
        pos++;
    }

    if (format & WW)
    {
        *(buf + pos) = byte2bcd(datetime->Week);
        pos++;
    }

    if (format & HH)
    {
        *(buf + pos) = byte2bcd(datetime->Hour);
        pos++;
    }

    if (format & MIN)
    {
        *(buf + pos) = byte2bcd(datetime->Min);
        pos++;
    }

    if (format & SEC)
    {
        *(buf + pos) = byte2bcd(datetime->Sec);
        pos++;
    }

    for (i = 0; i < pos; i++)
    {
        *(bcd + i) = *(buf + pos - 1 - i);
    }
}

/*******************************************************************************
 * Function Name  : Bcd2DateTime
 * Description    : Bcd字节数组转datetime
 * Input          : datetime 基准时间  bcd Bcd数组  len 数组长度  format 时间格式定义
 * Output         : datetime Bcd数组转化的时间
 * Return         : None
 *******************************************************************************/
//void Bcd2DateTime(TDateTime *datetime, u8 *bcd, u8 len, TDFormatType format)
void Bcd2DateTime(TDateTime *datetime, u8 *bcd, u8 len, u8 format)
{

    if (len > 7)
    {
        lib_printf("bcd2datetime length error!\n");
        return;
    }
    if (format & YY)
    {
        datetime->Year = byte2year(bcd2byte(*(bcd + len - 1)));
        len--;
    }
    else
    {
        datetime->Year = 0xFFFF;
    }

    if (format & MM)
    {
        datetime->Month = bcd2byte(*(bcd + len - 1));
        len--;
    }
    else
    {
        datetime->Month = 0xFF;
    }

    if (format & DD)
    {
        datetime->Day = bcd2byte(*(bcd + len - 1));
        len--;
    }
    else
    {
        datetime->Day = 0xFF;
    }

    if (format & WW)
    {
        datetime->Week = bcd2byte(*(bcd + len - 1));
        len--;
    }
    else
    {
        datetime->Week = 0xFF;
    }

    if (format & HH)
    {
        datetime->Hour = bcd2byte(*(bcd + len - 1));
        len--;
    }
    else
    {
        datetime->Hour = 0xFF;
    }

    if (format & MIN)
    {
        datetime->Min = bcd2byte(*(bcd + len - 1));
        len--;
    }
    else
    {
        datetime->Min = 0xFF;
    }

    if (format & SEC)
    {
        datetime->Sec = bcd2byte(*(bcd + len - 1));
        len--;
    }
    else
    {
        datetime->Sec = 0xFF;
    }
}

unsigned int date2timestamp(TDateTime *datetime_s)
{

    unsigned int tTemp=0;
    unsigned int tSecond=0;
    int month_s[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},
    {31,29,31,30,31,30,31,31,30,31,30,31}};
    int nCount=0;
    int i;
    int j;
    TDateTime tm=*datetime_s;
    TDateTime *datetime=&tm;
    dec_DateTime_Hour(datetime,8);//调整为伦敦时间
    tSecond=(datetime->Hour)*3600+datetime->Min*60+datetime->Sec;

    for (i=1970;i<datetime->Year;++i)
    {
        if (IsRound(i))
            ++nCount;
    }

    tTemp+=(datetime->Year-1970-nCount)*SECONDOFYEAR+nCount*SECONDOFROUNDYEAR;

    if (datetime->Month>1)
    {
        if (IsRound(datetime->Year))
        {
            for (j=0;j<datetime->Month-1;++j)
            {
                tTemp+=month_s[1][j]*24*60*60;
            }
            tTemp+=(datetime->Day-1)*24*60*60+tSecond;
        }
        else
        {
            for (j=0;j<datetime->Month-1;++j)
            {
                tTemp+=month_s[0][j]*24*60*60;
            }
            tTemp+=(datetime->Day-1)*24*60*60+tSecond;
        }
    }
    else
    {
        tTemp+=(datetime->Day-1)*24*60*60+tSecond;
    }

    return tTemp;
}


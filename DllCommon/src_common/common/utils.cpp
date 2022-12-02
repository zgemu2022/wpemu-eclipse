/*
 * utils.c
 *
 *  Created on: 2017-12-31
 *      Author: root
 */
#include "utils.h"
#include <stdio.h>

u16 TableSearch(u32 key, u8 *pbuf, u8 unit_size, u16 total_num,
        u8 keytype)
{
	u16 i;
	u32 temp;

	for(i=0;i<total_num;i++)
	{

        switch (keytype)
        {
        case sizeof(u8):
            temp = *(u8 *)(pbuf + i * unit_size);

       //    printf("观察一下问题？？？？？temp=%x key=%x\r\n",(int)temp,(int)key);
            break;
        case sizeof(u16):
            temp = *(u16 *)(pbuf + i * unit_size);
            break;
        case sizeof(u32):
            temp = *(u32 *)(pbuf + i * unit_size);
            break;
        }
        //    printf("观察一下问题？？？？？temp=%x key=%x\r\n",(int)temp,(int)key);
	       if (key == temp)
	       	{
	  //  	   printf("111观察一下问题？？？？？temp=%x key=%x  i=%d\r\n",(int)temp,(int)key,i);
	    	   return i;
	       	}

	}
	return 0xffff;
}

/*******************************************************************************
 * Function Name  : MidSplitTableSearch
 * Description    : 二分查找法搜索数据项表
 * Input          : key:关键字,pbuf:表起始地址,UnitSize:单条记录的大小,total_num:总记录数量
 *                  keytype:关键字的字节长度
 * Output         : None
 * Return         : u16 搜索到的位置
 *******************************************************************************/
u16 MidSplitTableSearch(u32 key, u8 *pbuf, u8 unit_size, u16 total_num,
        u8 keytype)
{
    u16 low = 0;
    u16 high = 0;
    u16 mid = 0;
    u32 temp = 0;

    high = total_num;
    mid = (low + high) / 2;

//	log_debug("MidSplitTableSearch start low=%d high=%d mid=%d key=%d unit_size=%d keytype=%d=========\n",
//		low, high, mid, key, unit_size, keytype);

    while (low <= high) //折中算法
    {
        mid = (low + high) / 2;

        switch (keytype)
        {
        case sizeof(u8):
            temp = *(u8 *)(pbuf + mid * unit_size);
            break;
        case sizeof(u16):
            temp = *(u16 *)(pbuf + mid * unit_size);
            break;
        case sizeof(u32):
            temp = *(u32 *)(pbuf + mid * unit_size);
            break;
        }
//		log_debug("显示一下到底怎么回事--00 temp=%d key=%d\n",temp,key);
//		log_debug("MidSplitTableSearch low=%d high=%d mid=%d key=%d temp=%d\n",
//			low, high, mid, key, temp);
	
        if (key == temp)
       	{
		//	log_debug("MidSplitTableSearch return mid=%d*************\n", mid);
            return mid;
       	}

		if(low == high)
		{
			//low == high说明是最后1个元素，还不相等就直接退出
			return ERROR16;
		}


        if (key < temp)
       	{
       		if(mid > 0)
            	high = mid - 1;
			else
			{
				//到达起始位置还不相等就直接退出
//				lib_printf("MidSplitTableSearch Error return FFFF***ke=%d unit_size=%d total_num=%d keytype=%d\n",
//					key, unit_size, total_num, keytype);
				return ERROR16;
			}				
       	}
        else
            low = mid + 1;
    }
//	lib_printf("MidSplitTableSearch low=%d high=%d mid=%d key=%d temp=%d\n",
//		low, high, mid, key, temp);

    return ERROR16;
}

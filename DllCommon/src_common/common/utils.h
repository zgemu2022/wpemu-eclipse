/*
 * utils.h
 *
 *  Created on: 2017-12-31
 *      Author: root
 */

#ifndef UTILS_H_
#define UTILS_H_
#ifdef __cplusplus
extern "C"{
#endif
#include "share_type.h"
u16 MidSplitTableSearch(u32 key, u8 *pbuf, u8 unit_size, u16 total_num,
        u8 keytype);

u16 TableSearch(u32 key, u8 *pbuf, u8 unit_size, u16 total_num,
        u8 keytype);

#ifdef __cplusplus

}

#endif
#endif /* UTILS_H_ */

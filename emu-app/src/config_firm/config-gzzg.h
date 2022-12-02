/*
: * config-cysr.h
 *
 *  Created on: 2019-4-12
 *      Author: root
 */

#ifndef CONFIG_CYSR_H_
#define CONFIG_CYSR_H_

#ifdef __cplusplus

extern "C"{

#endif
#include "config-default.h"
#include "program.h"
//智光EMU配置文件

configPara  conf_tab[] = {
		{"firmid",TY_INT,"1"},

		{"lcd_server_ip1",TY_STR,"192.168.2.230"},
		{"lcd_server_port1",TY_INT,"1502"},//
		{"lcd_server_ip2",TY_STR,"192.168.2.230"},
		{"lcd_server_port2",TY_INT,"1503"},//
		{"lcd_server_ip3",TY_STR,"192.168.2.230"},
		{"lcd_server_port3",TY_INT,"1504"},//
		{"lcd_server_ip4",TY_STR,"192.168.2.230"},
		{"lcd_server_port4",TY_INT,"1505"},//
		{"lcd_server_ip5",TY_STR,"192.168.2.230"},
		{"lcd_server_port5",TY_INT,"1506"},//
		{"lcd_server_ip6",TY_STR,"192.168.2.230"},
		{"lcd_server_port6",TY_INT,"1507"},//
		{"lcd_num",TY_INT,"6"},
		{"pcs_num",TY_INT,"28"},
		{"plc_server_ip",TY_STR,"192.168.2.230"},
		{"plc_server_port",TY_INT,"2502"},
		{"balance_rate",TY_INT,"100"},		//均衡速度,精度0.01
		{"sys_max_pw",TY_INT,"18000"},		//系统定义最大功率,精度0.01


		{"LOG_DAYS",TY_INT,"30"},//日志记录天数
		{"LOGENABLE",TY_INT,"1"},//日志是否记录到Flash
		{"MAIN_VER",TY_STR,"220915"},
		{"START_LOG",TY_STR,"220915"},//日志开始记录日期
};
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_CYSR_H_ */

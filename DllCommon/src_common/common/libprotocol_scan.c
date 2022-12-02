/*
 ============================================================================
 Name        : libprotocol_scan.c
 Author      : liutianpeng
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "protocol_scan.h"
//#define DEBUG

#define GPMC_MISC_BANK_BASE_ADDR        0x10000000
#define MAP_LENGTH                      0x10

//全局变量定义
int mfd = -1;
unsigned char *cpld_base = NULL;

static char* get_hostname() {
	FILE* fp = NULL;
	char* name = NULL;
	int len = 0;

	fp = fopen("/etc/hostname", "r");
	if (fp == NULL) {
#ifdef DEBUG
		perror("fopen /etc/hostname fail:");
#endif
		goto FAIL;
	}

	if (getline(&name, (size_t *)&len, fp) == -1) {
#ifdef DEBUG
		perror("getline fail:");
#endif
		goto FAIL;
	}

	fclose(fp);
	return name;

	FAIL: fclose(fp);
	return NULL;
}

static char* get_hardware_version() {
	FILE* fp = NULL;
	char* line = NULL;
	char *version = NULL;
	char *p = NULL;
	int len = 0;

	fp = fopen("/etc/issue", "r");
	if (fp == NULL) {
#ifdef DEBUG
		perror("fopen /etc/issue fail: ");
#endif
		goto FAIL;
	}

	while (getline(&line, (size_t *)&len, fp) != -1) {
#ifdef DEBUG
		printf("%s\n", line);
#endif

		if ((p = strstr(line, "Hardware")) != NULL) {
			if ((p = strchr(line, '.')) != NULL) {
				p -= 2;
				version = p;
				break;
			}
		}

	}

	fclose(fp);
	return version;

	FAIL: fclose(fp);
	return NULL;
}

static int cpld_mmap_init(void) {
	//unsigned char ret = 0;

	mfd = open("/dev/mem", O_RDWR);
	if (mfd < 0) {
		printf("open memory failed.\n");
		return -1;
	}

	cpld_base = (unsigned char *) mmap(NULL, MAP_LENGTH, PROT_READ | PROT_WRITE,
			MAP_SHARED, mfd, GPMC_MISC_BANK_BASE_ADDR);
	if (NULL == cpld_base) {
		printf("mmap 0x%08x failed.\n", GPMC_MISC_BANK_BASE_ADDR);
		close(mfd);
		return -2;
	}

	return 0;
}

static void cpld_mmap_exit(void) {
	munmap(cpld_base, MAP_LENGTH);
	close(mfd);
}

static unsigned char cpld_reg_read(unsigned char offset) {
	return (*(cpld_base + offset));
}

void cpld_reg_write(unsigned char offset, unsigned char value) {
	*(cpld_base + offset) = value;
}

int dca600_v10_v13_check() {
	int ret = 0;

	ret = cpld_reg_read(0x0); //看门狗
	if (ret != 0) {
		return 1;
	}

	ret = cpld_reg_read(0xa); //4G电源控制
	if (ret != 5) {
		return 3;
	}

	ret = access("/etc/udev/rules.d/lteusb.rules", F_OK); //检查文件系统，避免北变的TCU(北变用的是mdev)
	if (ret != 0) {
		return 4;
	}

	ret = access("/etc/init.d/rc.user", F_OK); //检查文件系统
	if (ret != 0) {
		return 5;
	}

	return 0;
}

int dca600_v14_check() {
	int ret = 0;

	ret = cpld_reg_read(0x0); //看门狗
	if (ret != 0) {
		return 1;
	}

	ret = cpld_reg_read(0x8); //液晶屏背光和显示控制
	if (ret != 0x7) {
		return 2;
	}

	ret = cpld_reg_read(0xa); //4G电源控制
	if (ret != 1) {
		return 3;
	}

	ret = access("/etc/udev/rules.d/lteusb.rules", F_OK); //检查文件系统，避免北变的TCU(北变用的是mdev)
	if (ret != 0) {
		return 4;
	}

	ret = access("/etc/init.d/rc.user", F_OK); //检查文件系统
	if (ret != 0) {
		return 5;
	}

	return 0;
}

int dca600_v20_v21_check() {
	int ret = 0;

	ret = cpld_reg_read(0x0); //看门狗
	if (ret != 0) {
		return 1;
	}

	ret = cpld_reg_read(0xc); //液晶屏背光和显示控制
	if (ret != 0x7) {
		return 2;
	}

	ret = cpld_reg_read(0xe); //4G电源控制
	if (ret == 0) {
		return 3;
	}

	ret = access("/etc/udev/rules.d/lteusb.rules", F_OK); //检查文件系统，避免北变的TCU(北变用的是mdev)
	if (ret != 0) {
		return 4;
	}

	ret = access("/etc/init.d/rc.user", F_OK); //检查文件系统
	if (ret != 0) {
		return 5;
	}

	return 0;
}

int dca270_check() {
	int ret = 0;

	ret = cpld_reg_read(0x0); //看门狗
	if (ret != 0) {
		return 1;
	}

	ret = cpld_reg_read(0x6); //液晶屏背光和显示控制
	if (ret != 0x7) {
		return 2;
	}

	ret = cpld_reg_read(0x7); //4G电源控制
	if (ret != 1) {
		return 3;
	}

	ret = access("/etc/udev/rules.d/lteusb.rules", F_OK); //检查文件系统，避免北变的TCU(北变用的是mdev)
	if (ret != 0) {
		return 4;
	}

	ret = access("/etc/init.d/rc.user", F_OK); //检查文件系统
	if (ret != 0) {
		return 5;
	}

	return 0;
}

int dca210_v23_getID()
{
    int i = 0;
    unsigned char base = 0xc0;
    unsigned char cmp_data[] = {0x62,0x94,0x13,0x93,0x9a,0x52};
    unsigned char read_cmp_data[] = {0,0,0,0,0,0};
    printf("检测硬件编号 ID=");
    for(i=0; i<6; i++,base++){


    	read_cmp_data[i] = cpld_reg_read(base);
        printf("%x",read_cmp_data[i]);

    }
	printf("\n");


    for(i=0; i<6; i++)
    {
    	  if(read_cmp_data[i] != cmp_data[i])
             return 6;

    }

    return 0;
}

int dca210_check() {
	int ret = 0;

	ret = cpld_reg_read(0x0); //看门狗
	if (ret != 0) {
		return 1;
	}

	ret = cpld_reg_read(0x6); //液晶屏背光和显示控制
	if (ret != 0x7) {
		return 2;
	}

	ret = cpld_reg_read(0x7); //4G电源控制
	if (ret != 1) {
		return 3;
	}

	ret = access("/etc/udev/rules.d/lteusb.rules", F_OK); //检查文件系统，避免北变的TCU(北变用的是mdev)
	if (ret != 0) {
		return 4;
	}

	ret = access("/etc/init.d/rc.user", F_OK); //检查文件系统
	if (ret != 0) {
		return 5;
	}

	return 0;
}

/*平台检验接口，成功返回0 失败返回其他值*/
int protocol_platform_check() {
	char* hostname = NULL;
	char* version = NULL;
	char tmp[4] = { 0 };
	int ret = 0,ret1=0;
	char buf[256] = {0};


	hostname = get_hostname();
	if (hostname == NULL) {
		return -1;
	}

	version = get_hardware_version();
	if (version == NULL)
		version = tmp;

	ret = cpld_mmap_init();
	if (ret != 0) {
		printf("cpld init fail !!!");
		return -1;
	}

	if ((strncmp(hostname, "DCA600", 6) == 0)
			|| (strncmp(hostname, "ICE624", 6) == 0)
			|| (strncmp(hostname, "DCA-600", 7) == 0)
			|| (strncmp(hostname, "ICE-624", 7) == 0)) {
		if ((strncmp(version, "V2.0", 4) == 0)
				|| (strncmp(version, "V2.1", 4) == 0))
			ret = dca600_v20_v21_check();
		else if ((strncmp(version, "V1.4", 4) == 0))
			ret = dca600_v14_check();
		else
			ret = dca600_v10_v13_check();
		if(ret==0)
		{
			ret1=DCA600;
		}
		else
			ret1=-2;


	}
	else if((strncmp(hostname, "DCA700", 6) == 0)
			|| (strncmp(hostname, "DCA-700", 7) == 0) ){
//		if (strncmp(version, "V1.0", 4) == 0){
//			ret = dca600_v20_v21_check();
//			if(ret ==0){
//				TCU_Type=DCA700;
//			}
//		}
//		if(version != NULL)//因为目前DCA700只有一个版本，暂不进行版本号匹配，但是需要获取到版本号
			ret1 = DCA700;
	}

	else if ((strncmp(hostname, "DCA210", 6) == 0)
			|| (strncmp(hostname, "DCA-210", 7) == 0)) {
		ret = dca210_check();

        if(ret == 0){
        	 if (strncmp(version,"V2.3",4) ==0)
        	 {
                ret = dca210_v23_getID();

                if(ret==0)
                {
                	printf("小桔TCU DCA210");
                	ret1=DCA210;
                }
                else
                {
                	printf("非小桔TCU DCA210");
                	ret1=-3;
                }
        	 }
        	 else
        	 {
             	   printf("此版本 TCU DCA210");
                 	ret1=DCA210;

        	 }
        }
        else
        	ret1=-4;


	} else if ((strncmp(hostname, "DCA270", 6) == 0)
			|| (strncmp(hostname, "DCA-270", 7) == 0)) {
		ret1 = dca270_check();
	}
	else if ((strncmp(hostname, "DCA220", 6) == 0)
				|| (strncmp(hostname, "DCA-220", 7) == 0)) {
			ret1 = DCA220;
		} else
		ret1=-5 ;

	if(ret1 <0){
		sprintf(buf,"echo %d > /var/log/platform",ret);
		system(buf);
	}

	cpld_mmap_exit();
	return ret1;
}

// int main(void) {

// 	int ret = 0;
// 	ret = protocol_platform_check();
// 	if (ret == 0) {
// 		printf("platform check success!\n");
// 	} else {
// 		printf("platform check fail ,ret=%d!!!\n", ret);
// 	}

// 	return EXIT_SUCCESS;

// }

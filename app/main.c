#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "fcntl.h"
#include <linux/input.h>
#include <sys/ioctl.h>

//用户自定义头文件
#define TRACE_MODULE    "main.c"
#include <trace.h>
#include <user_util.h>


/**
 * 在挂载的文件系统中 /etc/init.d/rcS 设了开机自动启动
 */
int main(int argc , char *argv[])
{
	int fd;

    int read_len = 0;

    uint16_t read_buff[3];
    uint16_t ir = 0;
    uint16_t als = 0;
    uint16_t ps = 0;

	if( 2 != argc )
    {
        trace_errorln("Error usage !");
        return -1 ;
    }

	char *filename = argv[1];

	fd = open(filename , O_RDWR);
	if(fd < 0)
	{
		trace_errorln("Fail:file %s open \n",filename);
        return -1 ;
	}

    for(;;)
    {
        read_len = read(fd,read_buff,sizeof(read_buff));

        /**
         * 读取到数据
         */
        if(read_len > 0)
        {
            ir = read_buff[0];
            als = read_buff[1];
            ps = read_buff[2];

            trace_infoln("ir = %d, als = %d, ps = %d",ir,als,ps);
        }

        sleep(1);
    }

    close(fd);

    return 0;
}


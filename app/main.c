#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "fcntl.h"

//用户自定义头文件
#define TRACE_MODULE    "main.c"
#include <trace.h>
#include <user_util.h>


int main(int argc , char *argv[])
{
	int fd;
	int err_code = 0;

	uint8_t tx_buff[1];

	if( 3 != argc )
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

	tx_buff[0] = atoi( argv[2] );

	err_code = write(fd,tx_buff,sizeof(tx_buff));
    if( err_code < 0 )
    {
        trace_errorln("Fail : write data into drv file");
        close(fd);
        return -1 ;
    }

	err_code = close(fd);
    if( err_code < 0 )
    {
        trace_errorln("Fail: close to file %s",filename);
        return -1 ;
    }

    return 0 ;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <fcntl.h>

//用户自定义头文件
#define TRACE_MODULE    "main.c"
#include <trace.h>
#include <user_util.h>

#define MAX_EVENTS	10
static 	int fd;

static void  signal_handler(int sig)
{
	uint8_t read_buf[255];
	int read_sz = 0;

	read_sz = read(fd,read_buf,sizeof(read_buf));		//必须启动读函数，让底层的阻塞的条件复位为false
	if(read_sz > 0)
	{
		trace_debug("receive event queue %d data:",read_sz);
		trace_dump(read_buf,read_sz);
	}
	else
	{
		trace_debugln("no any data in event queue");
	}
}

int main(int argc , char *argv[])
{
	int flags = 0;

	if(argc != 2)
	{
		trace_errorln("Invalid parameter");
		return -1;
	}

	fd = open(argv[1],O_RDWR);		//阻塞式访问
	if(fd < 0)
	{
		trace_errorln("open %s file error",argv[1]);

		return -1;
	}

	fcntl(fd,F_SETOWN,getpid());		//获取文件描述符异步I/O所有权
	flags = fcntl(fd,F_GETFL);			//获得文件状态标记
	fcntl(fd,F_SETFL,flags | FASYNC);	//设置文件状态标记

	signal(SIGIO,signal_handler);		//设置回调函数

	while(1)
	{
		trace_infoln("i am alive");
		sleep(2);
	}

	close(fd);
	return 0;
}


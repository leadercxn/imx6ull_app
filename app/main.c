#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>

//用户自定义头文件
#define TRACE_MODULE    "main.c"
#include <trace.h>
#include <user_util.h>

int main(int argc , char *argv[])
{
	int err_code = 0;
	int fd;
	struct pollfd fds;

	uint8_t read_buf[255];
	int read_sz = 0;

	if(argc != 2)
	{
		return -1;
	}

	fd = open(argv[1],O_RDWR);		//阻塞式访问
	if(fd < 0)
	{
		trace_errorln("open %s file error",argv[1]);

		return -1;
	}

	fds.fd = fd;
	fds.events = POLLIN;		//poll关注的事件

	while(1)
	{
		/**
		 * -1 为一直等待
		 */
		err_code  = poll(&fds,sizeof(fds)/sizeof(struct pollfd),(-1));
		if(err_code)
		{
			trace_infoln("%s can be read",argv[1]);
			memset(read_buf,0,sizeof(read_buf));

			read_sz = read(fds.fd,read_buf,sizeof(read_buf));		//必须启动读函数，让底层的阻塞的条件复位为false
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
		else if(err_code == 0)
		{
			trace_warnln("wait timeout");
		}
		else if(err_code < 0)
		{
			trace_errorln("wait error");
		}
	}

	close(fd);
	return 0;
}


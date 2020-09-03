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

//用户自定义头文件
#define TRACE_MODULE    "main.c"
#include <trace.h>
#include <user_util.h>

#define MAX_EVENTS	10

int main(int argc , char *argv[])
{
	int err_code = 0;
	int fd;

	uint8_t read_buf[255];
	int read_sz = 0;

	int epoll_fd;
    epoll_fd = epoll_create(MAX_EVENTS);        //创建一个epoll句柄

	struct epoll_event e_event;					//用来装关注的文件描述符[in]
	struct epoll_event e_events[MAX_EVENTS];	//用来装底层输出的文件描述符集[out]

	int nfds = 0;								//装epoll返回的文件事件数

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

	e_event.events = EPOLLIN;	//关注的事件类型
	e_event.data.fd = fd;		//关注的文件描述符

	err_code =  epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&e_event);
	if(err_code != 0)
	{
		trace_errorln("epoll_ctl error");

		return -1;
	}

	while(1)
	{
		nfds = epoll_wait(epoll_fd,e_events,MAX_EVENTS,-1);   //-1永远等待
		if(nfds)
		{
			trace_infoln("%s can be read",argv[1]);
			memset(read_buf,0,sizeof(read_buf));

			int i;
            for(i = 0; i < nfds; i++)
			{
				if(e_events[i].data.fd == fd)
				{
#if 0
                    // 向epoll注册  other_fd 监听事件
                    ev.events=EPOLLIN;
                    ev.data.fd=connect_fd;
                    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,connect_fd,&ev)==-1)
                    {
                        perror("epoll_ctl:connect_fd register failed");
                        return -1;
                    }
#endif
					read_sz = read(e_events[i].data.fd,read_buf,sizeof(read_buf));		//必须启动读函数，让底层的阻塞的条件复位为false
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

#if 0
	struct pollfd fds;

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
#endif
	close(fd);
	return 0;
}


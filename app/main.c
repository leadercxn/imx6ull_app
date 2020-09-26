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

//用户自定义头文件
#define TRACE_MODULE    "main.c"
#include <trace.h>
#include <user_util.h>

static struct input_event m_inputevent;

int main(int argc , char *argv[])
{
	int fd;
	int err_code = 0;

	if( 2 != argc )
    {
        trace_errorln("Error usage !");
        return -1 ;
    }

	char *filename = argv[1];

    memset(&m_inputevent,0,sizeof(m_inputevent));

	fd = open(filename , O_RDWR);
	if(fd < 0)
	{
		trace_errorln("Fail:file %s open \n",filename);
        return -1 ;
	}

	while(1)
    {
        err_code = read(fd, &m_inputevent, sizeof(m_inputevent));
        trace_infoln("err_code = %d\n",err_code);
        if(err_code > 0)
        {
            switch(m_inputevent.type)
            {
                case EV_KEY:
                    if(m_inputevent.code == KEY_0)
                    {
                        trace_infoln("m_inputevent.value = %d\n",m_inputevent.value);
                    }
                break;

                default:

                break;
            }
        }
    }

    close(fd);

    return 0 ;
}


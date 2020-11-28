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


int main(int argc , char *argv[])
{
	int fd;

    int read_len = 0;

    int32_t read_buff[7];
    int32_t accel_x_adc = 0;
    int32_t accel_y_adc = 0;
    int32_t accel_z_adc = 0;
    int32_t temp_adc = 0;
    int32_t gyro_x_adc = 0;
    int32_t gyro_y_adc = 0;
    int32_t gyro_z_adc = 0;

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
        memset(read_buff,0,sizeof(read_buff));

        read_len = read(fd,read_buff,sizeof(read_buff));

        /**
         * 读取到数据
         */
        if(read_len > 0)
        {
            accel_x_adc = read_buff[0];
            accel_y_adc = read_buff[1];
            accel_z_adc = read_buff[2];
            temp_adc =    read_buff[3];
            gyro_x_adc =  read_buff[4];
            gyro_y_adc =  read_buff[5];
            gyro_z_adc =  read_buff[6];

            trace_infoln("icm20608 sensor data :");
            trace_infoln("accel_x_adc = %d",accel_x_adc);
            trace_infoln("accel_y_adc = %d",accel_y_adc);
            trace_infoln("accel_z_adc = %d",accel_z_adc);
            trace_infoln("temp_adc = %d",temp_adc);
            trace_infoln("gyro_x_adc = %d",gyro_x_adc);
            trace_infoln("gyro_y_adc = %d",gyro_y_adc);
            trace_infoln("gyro_z_adc = %d",gyro_z_adc);

            trace_infoln("");
        }

        sleep(1);
    }

    close(fd);

    return 0;
}


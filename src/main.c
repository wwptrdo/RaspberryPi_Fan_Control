/*
 * The C-language module that controls the fan on the Raspberry Pi.
 *
 * author: wwptrdudu <1348351139@qq.com>
 * github: https://github.com/wwptrdudu
 * QQ: 1348351139
 * data: 2018.09.22
 */


#include "fan.h"
#include <stdio.h>
#include <wiringPi.h>

int main(int argc, char* argv[])
{
	wiringPiSetup();         //wiringPi库初始化
	
	/*
	 *功能：初始化风扇管理
	 *参数：初始启动的模式、自定义模式下的温度阈值(0-100)、自定义模式下的风扇速度[25-100] */
	fan_init(CUSTOM,           37,                              52); 

	open_fan();    //开启风扇管理


	//测试
	while(1)
	{
		
		sleep(18);

		printf("切换到自动管理模式!\n");
		change_fan_mode(AUTOMATIC);
		
		sleep(29);
		
		printf("切换到自定义模式！\n");
		change_fan_mode(CUSTOM);
	}

	return 0;
}

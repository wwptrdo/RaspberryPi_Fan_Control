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
#include <signal.h>
#include <unistd.h>
#include <wiringPi.h>

static void signal_handler(int signo)
{
	if (signo == SIGCHLD)
	{
		wait(0); //释放僵尸进程 （保留）
	}
	else if (signo == SIGINT) //捕获Ctrl+C信号
	{
		sys_close_fan(); //关闭风扇管理
		printf("已经关闭风扇管理程序！\n");
		exit(1);
	}
}

void show_menu()
{
	printf("---------------------\n");
	printf("选择风扇运行模式：\n");
	printf("0 - 自动模式\n");
	printf("1 - 静音模式\n");
	printf("2 - 自定义模式\n");
	printf("3 - 强力模式\n\n");
}

int mode_choose()
{
	int mode = 0;

	show_menu();
	fflush(stdin);
	scanf("%d", &mode);

	return mode;
}

int main(int argc, char *argv[])
{
	if (signal(SIGCHLD, signal_handler) == SIG_ERR)
	{
		//show_sys_info("登记信号SIGCHLD出错\n");
		printf("登记信号SIGCHLD出错！\n");
		exit(1);
	}
	//登记Ctrl+C（第九个信号）
	if (signal(SIGINT, signal_handler) == SIG_ERR)
	{
		printf("登记信号SIGINT出错！\n");
		exit(1);
	}

	wiringPiSetup(); // wiringPi 库初始化

	/*
	 *功能：初始化风扇管理
	 *参数：初始启动的模式、风扇的启动温度阈值(0-100)、风扇的关闭温度阈值(0-100)，自定义模式下的风扇速度[25-100]
	*/
	fan_init(mode_choose());

	open_fan(); //开启风扇管理

	while (1)
	{
		printf("当前温度: %d °C\r", sys_cpu_temp());
		fflush(stdout);
		sleep(3);
	}

	return 0;
}

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
#include <string.h>

#define CMD_RESULT_LEN 1024


static Fan s_fan;



/*
 * 日志
 */
static void show_sys_info(char *str)
{
	printf("%s", str);
	/*
	 * 可将日志信息添加到文件中,代码在下方添加
	 */

}


/*
 * 功能：执行系统命令，并获取命令的返回数据
 * 参数：cmd 要执行的系统命令，result 为接收命令返回的数据
 * 返回值：成功返回0，失败返回-1
 * 注意：执行的命令和返回的数据最大为CMD_RESULT_LEN
 */
static int execute_cmd_result(const char *cmd, char *result)
{       
        if (cmd == NULL || result == NULL || strlen(cmd) <= 0)
                return -1;
        char buf_ps[CMD_RESULT_LEN] = {0};
        char ps[CMD_RESULT_LEN] = {0};
        FILE *ptr; 
        strcpy(ps, cmd);
        if((ptr=popen(ps, "r"))!=NULL)
        {       
                while(fgets(buf_ps, CMD_RESULT_LEN, ptr)!=NULL)
                {       
                        strcat(result, buf_ps);
                        if(strlen(result)>CMD_RESULT_LEN)
                                break;
                }
                pclose(ptr);
                ptr = NULL;
        }
        else
        {       
                printf("error: [sys.c]popen %s error\n", ps);
                return -1;
        }
        return 0;
}




/*
 * 功能：获取系统cpu温度
 * 参数：无
 * 返回值：成功返回0，失败返回-1
 */
static int sys_cpu_temp()
{
        char buff[CMD_RESULT_LEN] = {0};
        if (execute_cmd_result("cat /sys/class/thermal/thermal_zone0/temp", buff) < 0)
        {
                show_sys_info("错误：执行获取cpu温度命令失败！\n");
                return -1;
        }
        double ret = atof(buff)/1000.0;
        return (int)ret;
}


/*
 * 强力风扇模式
 */
static void powerful_mode()
{
	digitalWrite(FAN_PIN, HIGH);  //始终高电平
}

static int is_running_custom = FALSE;     //标识位：标识是否正在运行自定义模式
static int is_running_automatic = FALSE;  //标识位：标识是否正在运行自动模式


static void* custom_th(void *arg)
{
	is_running_custom = TRUE;
	int powerful = 100;  //风扇最大时,标识为100
	//PWM调速
	while(s_fan.mode == CUSTOM && s_fan.fan_switch)
	{
		int temp = sys_cpu_temp(); //获取系统CPU温度
		
		//如果温度达到了阈值
		if (temp >= s_fan.threshold)	
		{
			//发送50个脉冲
			int i = 0;
			for (; i < 50; i++)
			{
				//如果调节声音和风扇的大小，可以更改1000和800两个数字，做微调即可！
				digitalWrite(FAN_PIN, HIGH);
                        	delayMicroseconds(s_fan.fan_speed * 1000);
                        	digitalWrite(FAN_PIN, LOW);
                        	delayMicroseconds((powerful*800) - (s_fan.fan_speed*1000));			
			}
			 
		}

	}
	is_running_custom = FALSE;

	return (void*)0;
}


/*
 * 自定义模式: 创建一个线程，根据自定义的参数对风扇转速进行调整 
 */
static void custom_mode()
{
	if (s_fan.fan_switch && s_fan.mode == CUSTOM && !is_running_custom && !is_running_automatic)	
	{
		//设置线程分离属性，以分离状态启动的线程，在线程结束后会自动释放所占有的系统资源。
        	pthread_attr_t attr;
        	pthread_attr_init(&attr);
        	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_t th;
        	int err_h;
        	if (err_h = pthread_create(&th, &attr, custom_th, (void*)0) != 0)
        	{
                	perror("custom_th pthread_create error!");
                	pthread_attr_destroy(&attr); //销毁线程属性结构体
                	return ;
        	}

	        pthread_attr_destroy(&attr); //销毁线程属性结构体

	}
	return;
}

/*
 * 自动模式
 */
static void automatic_mode()
{

	if (s_fan.fan_switch && s_fan.mode == AUTOMATIC && !is_running_custom && !is_running_automatic)
	{
		/*
		 * 挖坑：自动模式还未实现
		 */

		printf("自动模式还未实现，暂时用强风模式代替!\n");
		powerful_mode();
	}

	return;
}


/*
 * 功能：风扇线程处理函数
 *       调度不同的运行模式
 */
static void* fan_server_th(void *arg)
{
	show_sys_info("风扇管理线程启动成功！\n");
	
	while(1)
	{
		//获取线程锁(未)


		//如果检测到风扇管理开关未打开，则此线程睡眠
		if (!s_fan.fan_switch)
		{	
			digitalWrite(FAN_PIN, LOW);
			sleep(1);
		}
		else if (s_fan.mode == CUSTOM)
		{
			custom_mode();
		}
		else if (s_fan.mode == AUTOMATIC)
		{
			automatic_mode();
		}
		else if (s_fan.mode == POWERFUL)
		{
			powerful_mode();
		}
		else 
		{
			show_sys_info("ERROR: 风扇运行模式调度出错！\n");
		}

		//释放线程锁(未)
		
		sleep(1);
	}


	return (void*)0;
}


/*
 * 功能：初始化风扇阵脚
 * 参数：风扇模式，自定义模式下的温度阈值[0-100]、风扇转速[1-100]
 * 返回值：成功返回0，失败返回-1
 */
int fan_init(int mode, int threshold, int speed)
{

	memset(&s_fan, 0, sizeof(Fan));
	
	pinMode(FAN_PIN, OUTPUT);
	digitalWrite(FAN_PIN, LOW);	
	
	//模式
	if (mode != AUTOMATIC && mode != CUSTOM && mode != POWERFUL)
	{
		s_fan.mode = AUTOMATIC;
		show_sys_info("选择风扇的模式有误，已更改为自动模式！\n");	
	} 
	else
	{
		s_fan.mode = mode;
	} 
	
	if (threshold < 0 || threshold > 100 || speed < 25 || speed > 100)
	{
		//传入参数有误，自定义模式下为默认值
		s_fan.threshold = 42;
		s_fan.fan_speed = 50;
		show_sys_info("自定义模式下设置风扇的参数有误，已更改为默认值！\n");
	}
	else
	{
		s_fan.threshold = threshold;
		s_fan.fan_speed = speed;      //自定义模式下的风扇转速  
	}

	s_fan.fan_switch = FALSE;              //风扇管理默认关闭

	printf("风扇参数设置成功！模式:%d, 自定义模式下温度阈值:%d 风扇转速:%d\n",
				s_fan.mode, s_fan.threshold, s_fan.fan_speed);
	/*
	 * 启动风扇控制线程，对风扇进行管理
	 */

        //设置线程分离属性，以分离状态启动的线程，在线程结束后会自动释放所占有的系统资源。
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	int err_h;
	if (err_h = pthread_create(&s_fan.th_fan, &attr, fan_server_th, (void*)0) != 0)
	{
		perror("fan_server_th pthread_create error!");
		pthread_attr_destroy(&attr); //销毁线程属性结构体
		return -1;
	}
	
	pthread_attr_destroy(&attr); //销毁线程属性结构体
	return 0;
}


/*
 * 功能：开启风扇管理
 */
void open_fan()
{
	s_fan.fan_switch = TRUE;
}



/*
 * 功能：关闭风扇管理 
 */
void close_fan()
{
	s_fan.fan_switch = FALSE;	
}


/*
 * 功能：更改风扇模式
 */
int change_fan_mode(int mode)
{
	if (mode != AUTOMATIC && mode != CUSTOM && mode != POWERFUL)
	{
		show_sys_info("ERROR: 风扇更改模式失败, mode参数有误！\n");
		return -1;
	}
	s_fan.mode = mode;
	return 0;
}


/*
 * 功能：系统关闭风扇
 * 注意：一般在程序退出之前调用
 */
void sys_close_fan()
{
	s_fan.fan_switch = FALSE;
	digitalWrite(FAN_PIN, LOW);  //风扇针脚低电平
}




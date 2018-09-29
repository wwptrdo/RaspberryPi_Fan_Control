/*
 * The C-program module that controls the fan on the Raspberry Pi.
 *
 * author: wwptrdudu <1348351139@qq.com>
 * github: https://github.com/wwptrdudu
 * QQ: 1348351139
 * data: 2018.09.22
 */


#ifndef _FAN_H_
#define _FAN_H_

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


#define FAN_PIN        2          //风扇的控制信号阵脚为GPIO2

#define AUTOMATIC      0
#define CUSTOM         1
#define POWERFUL       2

#define TRUE           1
#define FALSE          0

typedef struct {
	pthread_t th_fan;          //线程标识符，在此线程中处理风扇的速度控制
	int start_threshold;       //自定义模式下的温度阈值，达到此值风扇开启
	int stop_threshold;        //自定义模式下的温度阈值，达到此值风扇关闭（注意：开启阈值 > 关闭阈值）
	int fan_speed;             //风扇转速

	int  fan_switch;           //风扇开关
	int mode;                  //风扇的运行模式
}Fan;



extern int fan_init(int mode, int start_threshold, int stop_threshold, int speed);

extern void open_fan();

extern void close_fan();

extern void sys_close_fan();

#endif

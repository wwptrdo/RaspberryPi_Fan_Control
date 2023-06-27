# RaspberryPi Fan Control
树莓派上的CPU风扇温控模块。可提供三种风扇管理模式：自定义模式、强力模式、自动模式（正在更新中）。【RaspberryPi、WringPi、PWM调速】

自定义模式：可设置风扇的启动的温度阈值和关闭的温度阈值、风扇的转速等参数，根据此类参数对风扇进行控制。

强力模式：风扇以最大状态运行。

自动模式：

# 一、使用说明
## 1：准备前操作
（1）当前树莓派系统中包含GCC和wiringPI等开发工具。（官方的系统即可）

（2）需要的其他硬件设备：散热风扇（如图1所示），如果散热风扇不支持PWM调速，即风扇只有两个线，需要准备一个NPN型三极管，手动焊接出第三条信号线，具体见2小节风扇焊接部分；需要准备若干杜邦线等。

![Fan]( https://github.com/wwptrdudu/RaspberryPi_Fan_Control/blob/master/photos/fan.jpg )

图1：不带PWM调速的散热风扇


## 2：风扇连接
（1）如果购买的散热风扇为三根线，即支持PWM控制，则分别将正极线接5V、负极线接0V、信号线接GPIO-2针脚，如图2所示。针脚的定义在include文件夹中fan.h文件中，可自行修改。

![GPIO-Show]( https://github.com/wwptrdudu/RaspberryPi_Fan_Control/blob/master/photos/GPIO_Show.jpg )

图2：连接阵脚图


（2）如果购买的散热风扇为二根线，则不支持PWM控制，需要准备一个开关三极管，我这里从收音机中拆出的一个8050型号的三极管。用电烙铁依照图3、图4的说明进行焊接，最后可用热缩管对焊接的部分和三极管针脚进行保护一下，防止短路。焊接完成之后，此风扇则已经支持PWM调速，则可依照(1)的说明将风扇连接到树莓派上。关于三极管型号的科普，我这里推荐一个简单的基础视频（[三极管的应用基础介绍](https://www.bilibili.com/video/av8960413 "三极管的应用基础介绍")），其他的三极管知识可从Google上进行搜索。


![s8050]( https://github.com/wwptrdudu/RaspberryPi_Fan_Control/blob/master/photos/s8050.jpg )

图3：NPN开关三极管s8050



![connection]( https://github.com/wwptrdudu/RaspberryPi_Fan_Control/blob/master/photos/connection.jpg )

图4：焊接图示

## 2：编译
运行以下命令：

	git clone https://github.com/wwptrdudu/RaspberryPi_Fan_Control.git
	cd  RaspberryPi_Fan_Control
	make

## 3：运行

	sudo bin/fan


# 二、效果

连接好的树莓派如图5所示：


![RapsberryPi]( https://github.com/wwptrdudu/RaspberryPi_Fan_Control/blob/master/photos/raspberryPi.jpg )

图5：树莓派

程序运行效果如图6所示：


![CMD_RESULT]( https://github.com/wwptrdudu/RaspberryPi_Fan_Control/blob/master/photos/cmd_result2.jpg )

图6：程序运行结果

## 三、 更新日志

#### 【2018.09.29 中】
1、修复bug：关闭进程后，风扇为不确定状态运行，修改为关闭。

2、更新：自定义模式下，添加了风扇启动的温度阈值和关闭的温度阈值。解决了一个阈值风扇转速不定的问题。

#### 【2018.09.22.夜】
记得在几个月前已经把硬件连接好了，可惜在焊接的时候没有拍摄图片，所以缺少了好多焊接的过程图片。不过，焊接好后也并没有写这个驱动程序，以前一直打算以后写来着，终于拖到了现在。终于把以前挖好的坑填起来了，更新的话，应该会很快吧。



# Contact me 
#### If you have any questions, please contact me.
#### QQ: 1348351139
#### e-mail: 1348351139@qq.com

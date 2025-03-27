/*********************************************************************************************************
* 模块名称: Global.h
* 使用说明: 
* 摘    要: 宏定义
* 当前版本: 1.0.0
* 作    者: Leyutek(COPYRIGHT 2018 - 2021 Leyutek. All rights reserved.)
* 完成日期: 2021年01月22日
* 内    容:
* 注    意: none
**********************************************************************************************************
* 取代版本:
* 作    者:
* 完成日期:
* 修改内容:
* 修改文件:
*********************************************************************************************************/
#pragma once

/*********************************************************************************************************
*                                                宏定义
*********************************************************************************************************/
#define DK_SUCCESS          (0)  // 成功
#define ERR_DK_FAILURE      (-1) // 失败
#define ERR_COM_NO          (-2) // 端口号错误

const int MAXRECVBUF = 4096; // 串口接收缓冲区
const int MAXSENDBUF = 4096; // 串口发送缓冲区

typedef unsigned char  uchar;

#define TARGET_RESOLUTION 1

// 如果是为了使用自己对自己的调试功能, 可以使用2ms的间隔;
#define TIME_TICK  (20) // 单位: ms; 定义多媒体定时器的时间间隔;

// Acknowledge time 
#define ACK_TIME  (200) // 指令包响应时间，单位: ms;

#define CUFFPREMAXVALUE 200
#define PULSEMAXVALUE 4096

#define WM_SECOND_TIMER	      (WM_USER+101)
#define WM_DRAW_CUffPre       (WM_USER+102)
#define WM_DRAW_Pulse         (WM_USER+103)
#define WM_MODULE_RESET       (WM_USER+105)
#define WM_COMMAND_ACK        (WM_USER+106)
#define WM_NIBP_RESET	      (WM_USER+107)
#define WM_NIBP_FINISH 	      (WM_USER+108)
#define WM_COMM_ERROR 		  (WM_USER+109)
#define WM_COMMAND_NOACK      (WM_USER+110)
#define APPLY_NIBP_FACTORY_CAL // 应用NIBP厂家测量;


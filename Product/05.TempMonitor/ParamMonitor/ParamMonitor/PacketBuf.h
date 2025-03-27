/*********************************************************************************************************
* 模块名称: Packetbuf.h
* 摘    要: 类CPacketBuf用于保存符合通讯协议的数据包，并提供读、写的接口，相当于一个循环队列
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
#ifndef _PACKETBUF_H
#define _PACKETBUF_H

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "System.h"

/*********************************************************************************************************
*                                                宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                               类的定义
*********************************************************************************************************/
class CPacketBuf
{
	enum
	{ 
		MAX_PACK_NUM = 100 
	};
	
public:
	CPacketBuf(void);                  //构造函数
	
	void  Initialize( void );          //初始化函数
	bool  Get(PacketFrame * p);        //从缓冲区得到解包后的数据包
	bool  UnGet( void );               //把从缓冲区取得的数据包返回缓冲区内
	bool  Put( PacketFrame & packet ); //把未解包的数据包放入缓冲区
	
	bool  IsEmpty( void );             //判断缓冲区是否为空
	
private:
	int         mGetNdx;               //读取数据的序号
	int         mPutNdx;               //放入数据序号
	PacketFrame mBuffer[MAX_PACK_NUM]; //缓冲区
};

#endif

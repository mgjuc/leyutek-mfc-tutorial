/*********************************************************************************************************
* 模块名称: PackUnpack.h
* 使用说明: 打包解包模块
* 摘    要: 类头文件
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
*                                                包含头文件
*********************************************************************************************************/
#include "String.h"

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/
//定义最大分组长度，一个分组包括分组ID，分组实际长度，分组数据（包括byte1）和校验和
enum{MAX_PACKET_LEN = 10};

//包帧定义：一个包由一个字节（第一个字节）和8字节数据组成，如果数据包未打包，则只有7个字节的数据
typedef union
{  
	struct
	{
		UCHAR id;
		UCHAR id2;
		UCHAR data[MAX_PACKET_LEN - 2];
	};
	UCHAR buffer[MAX_PACKET_LEN];
}PacketFrame;
//枚举定义，定义ID
typedef enum
{
	DAT_SYS     = 0x01,          //系统信息
	DAT_ECG     = 0x10,          //心电信息
	DAT_RESP    = 0x11,          //呼吸信息
	DAT_TEMP    = 0x12,          //体温信息
	DAT_SPO2    = 0x13,          //血氧信息
	DAT_NBP     = 0x14,          //无创血压信息
	DAT_IBP     = 0x15,          //有创血压信息
		
	MAX_PACK_ID = 0x80
}EnumPackID;

typedef enum 
{
	DAT_RST         = 0x01,
	DAT_SYS_STS     = 0x02,
	DAT_SELF_CHECK  = 0x03,
	DAT_CMD_ACK     = 0x04,
		
	DAT_ECG_WAVE    = 0x02,
	DAT_ECG_LEAD    = 0x03,
	DAT_ECG_HR      = 0x04,
	DAT_ST          = 0x05,
	DAT_ST_PAT      = 0x06,
		
	DAT_RESP_WAVE   = 0x02,
	DAT_RESP_RR     = 0x03,
	DAT_RESP_APNEA  = 0x04,
	DAT_RESP_CVA    = 0x05,
		
	DAT_TEMP_DATA   = 0x02,
		
	DAT_SPO2_WAVE   = 0x02,
	DAT_SPO2_DATA   = 0x03,
	
	DAT_NBP_CUFPRE  = 0x02,
	DAT_NBP_END     = 0x03,
	DAT_NBP_RSLT1   = 0x04,
	DAT_NBP_RSLT2   = 0x05,
	DAT_NBP_STS     = 0x06,  
}EnumSecondPackID;

typedef enum DataID
{
	DATA_ECG1_WAVE,
	DATA_ECG2_WAVE,
	DATA_ECG_HEART,
	DATA_ECG_LEAD_V,
	DATA_ECG_LEAD_RA,
	DATA_ECG_LEAD_LA,
	DATA_ECG_LEAD_LL,
		
	DATA_RESP_WAVE,
	DATA_RESP_RR,
		
	DATA_TEMP_VAL1,
	DATA_TEMP_VAL2,
	DATA_TEMP_STS1,
	DATA_TEMP_STS2,
	
	DATA_SPO2_WAVE,     //DATA_SPO2只是用来判别数据类别，实际血氧5个数据是存储在s_arrUnpackData[DATA_SOP2_WAVE_0-5]里面
	DATA_SPO2_WAVE_0,
	DATA_SPO2_WAVE_1,
	DATA_SPO2_WAVE_2,
	DATA_SPO2_WAVE_3,
	DATA_SPO2_WAVE_4,
		
	DATA_SPO2,
	DATA_PULSE,
	DATA_SPO2_LEAD,
		
	DATA_NBP_CUFPRE,
	DATA_NBP_END,
	DATA_NBP_SYS,		//高压，收缩压
	DATA_NBP_DIA,		//低压，舒张压
	DATA_NBP_MAP,		//平均压
	DATA_NBP_PR,		//脉率
		
	MAX_DATA_ID
}EnumDataID;

/*********************************************************************************************************
*                                              类的定义
*********************************************************************************************************/
class CPackUnpack
{
public:
	//成员函数
	CPackUnpack();
	virtual ~CPackUnpack();
	void InitPackUnpack(void);
	void PackData(PacketFrame* pack);
	BOOL UnpackData(UCHAR* data);
	void GetUnpackRslt(PacketFrame* pack);

private:
	PacketFrame mPack;
	UCHAR mPackLen;
	UCHAR mGotPackId;
	UCHAR mRestByteNum;
	void PackWithCheckSum(UCHAR* pack);
	BOOL UnpackWithCheckSum(UCHAR* pack);
};
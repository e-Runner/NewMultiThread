/**
* @file ObsDetect.h
* @brief 模拟障碍物检测类的文件
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/

#pragma once
#include "DataProcessBase.h"
/**
@brief 模拟障碍物检测类

继承于CDataProcessBase
*/
class CObsDetect : public CDataProcessBase
{
public:
	/**
	@brief 构造函数，指出了自己线程的名字 obs

	*/
	CObsDetect();

	~CObsDetect();

	/**
	@brief 处理一帧数据的函数，简单模拟一下，将采集到的最新两帧的rs数据输出，并且等待123ms

	*/
	void doMain(void);

	/**
	@brief 初始化

	*/
	bool processInit();

	/**
	@brief 销毁

	*/
	bool processDeinit();
};


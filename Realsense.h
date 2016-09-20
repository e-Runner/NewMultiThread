/**
* @file Realsense.h
* @brief 模拟RS采集数据类的文件
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/

#pragma once
#include "DataAcquireBase.h"
/**
@brief 模拟RS采集数据类，这里简单处理一下，假设每一帧的数据为double = =！

继承于CDataAcquireBase<double>
*/
class CRealsense :
	public CDataAcquireBase<double>
{
public:
	CRealsense();
	~CRealsense();

	/**
	@brief 模拟RS采集一帧图像的函数
	
	这里简单模拟一下，有个double变量，每隔230ms比上一次自增1.1

	*/
	void doMain(void);

	/**
	@brief 模拟RS拷贝图像的函数

	*/
	void dataCpy(double s, double &d);
	/**
	@brief 模拟RS初始化函数

	*/
	bool init(void);

	/**
	@brief 模拟RS销毁函数

	*/
	bool deinit(void);
};


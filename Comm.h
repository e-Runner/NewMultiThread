/**
* @file Comm.h
* @brief 模拟串口读取类的文件
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/
#pragma once
#include "DataAcquireBase.h"
#include <vector>

/**
	@brief 模拟串口读取类，每一帧的数据为vector<float>

	继承于CDataAcquireBase< std::vector<float> > 
*/
class CComm :
	public CDataAcquireBase< std::vector<float> >
{
public:
	CComm();
	~CComm();
	/**
	@brief 模拟串口采集一帧数据的函数
	
	每一帧数据包含一个vector，其中有5个相同的float，从0.0开始，每一帧比上一帧多1.0,两帧之前间隔1s
	采集完成后记得把数据存放到dataTem变量中
	*/
	void doMain(void);

	/**
	@brief 模拟串口拷贝数据的函数 

	*/
	void dataCpy(std::vector<float> src, std::vector<float> &dst);

	/**
	@brief 模拟串口初始化函数

	*/
	bool init(void);

	/**
	@brief 模拟串口销毁函数

	*/
	bool deinit(void);
};


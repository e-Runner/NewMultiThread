/**
* @file FaceRecongnize.h
* @brief 模拟人脸检测类的文件
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/

#pragma once
#include "DataProcessBase.h"

/**
@brief 模拟人脸识别类

继承于CDataProcessBase
*/
class CFaceRecognize :
	public CDataProcessBase
{
public:
	/**
	@brief 构造函数，指出了自己线程的名字 face

	*/
	CFaceRecognize();

	~CFaceRecognize();

	/**
	@brief 处理一帧数据的函数，简单处理一下，将采集到的最新两帧的rs数据输出，并且等待200ms

	*/
	void doMain();

	/**
	@brief 初始化

	*/
	bool processInit();

	/**
	@brief 销毁

	*/
	bool processDeinit();
};


/**
* @file ObsDetect.h
* @brief ģ���ϰ���������ļ�
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/

#pragma once
#include "DataProcessBase.h"
/**
@brief ģ���ϰ�������

�̳���CDataProcessBase
*/
class CObsDetect : public CDataProcessBase
{
public:
	/**
	@brief ���캯����ָ�����Լ��̵߳����� obs

	*/
	CObsDetect();

	~CObsDetect();

	/**
	@brief ����һ֡���ݵĺ�������ģ��һ�£����ɼ�����������֡��rs������������ҵȴ�123ms

	*/
	void doMain(void);

	/**
	@brief ��ʼ��

	*/
	bool processInit();

	/**
	@brief ����

	*/
	bool processDeinit();
};


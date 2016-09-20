/**
* @file Realsense.h
* @brief ģ��RS�ɼ���������ļ�
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/

#pragma once
#include "DataAcquireBase.h"
/**
@brief ģ��RS�ɼ������࣬����򵥴���һ�£�����ÿһ֡������Ϊdouble = =��

�̳���CDataAcquireBase<double>
*/
class CRealsense :
	public CDataAcquireBase<double>
{
public:
	CRealsense();
	~CRealsense();

	/**
	@brief ģ��RS�ɼ�һ֡ͼ��ĺ���
	
	�����ģ��һ�£��и�double������ÿ��230ms����һ������1.1

	*/
	void doMain(void);

	/**
	@brief ģ��RS����ͼ��ĺ���

	*/
	void dataCpy(double s, double &d);
	/**
	@brief ģ��RS��ʼ������

	*/
	bool init(void);

	/**
	@brief ģ��RS���ٺ���

	*/
	bool deinit(void);
};


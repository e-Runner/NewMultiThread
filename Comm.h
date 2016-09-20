/**
* @file Comm.h
* @brief ģ�⴮�ڶ�ȡ����ļ�
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/
#pragma once
#include "DataAcquireBase.h"
#include <vector>

/**
	@brief ģ�⴮�ڶ�ȡ�࣬ÿһ֡������Ϊvector<float>

	�̳���CDataAcquireBase< std::vector<float> > 
*/
class CComm :
	public CDataAcquireBase< std::vector<float> >
{
public:
	CComm();
	~CComm();
	/**
	@brief ģ�⴮�ڲɼ�һ֡���ݵĺ���
	
	ÿһ֡���ݰ���һ��vector��������5����ͬ��float����0.0��ʼ��ÿһ֡����һ֡��1.0,��֮֡ǰ���1s
	�ɼ���ɺ�ǵð����ݴ�ŵ�dataTem������
	*/
	void doMain(void);

	/**
	@brief ģ�⴮�ڿ������ݵĺ��� 

	*/
	void dataCpy(std::vector<float> src, std::vector<float> &dst);

	/**
	@brief ģ�⴮�ڳ�ʼ������

	*/
	bool init(void);

	/**
	@brief ģ�⴮�����ٺ���

	*/
	bool deinit(void);
};


/**
* @file FaceRecongnize.h
* @brief ģ�������������ļ�
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/

#pragma once
#include "DataProcessBase.h"

/**
@brief ģ������ʶ����

�̳���CDataProcessBase
*/
class CFaceRecognize :
	public CDataProcessBase
{
public:
	/**
	@brief ���캯����ָ�����Լ��̵߳����� face

	*/
	CFaceRecognize();

	~CFaceRecognize();

	/**
	@brief ����һ֡���ݵĺ������򵥴���һ�£����ɼ�����������֡��rs������������ҵȴ�200ms

	*/
	void doMain();

	/**
	@brief ��ʼ��

	*/
	bool processInit();

	/**
	@brief ����

	*/
	bool processDeinit();
};


/**
* @file DataProcessBase.h
* @brief ���߳����ݴ���Ļ��������ļ�������ʵ�ֶ��߳����ݴ�����̵߳���
*
* ���ݴ������͵ĳ��������ڴ��࣬����ֽ�Ҽ�⣨�����ɫͼ�����ݣ����ϰ����⣨�������ͼ�����̬�����ݣ�
* ʹ�÷�����
*	1.�������̳�����ࣻ
*	2.ʵ��doMain(),processInit(),processDeinit()���������麯����doMainΪ��ѭ����processInit()��processDeinit()Ϊ��ѭ����ʼǰ��ĳ�ʼ��������
*	3.ʹ��addDataSource()�������һ������Դ���������˵��
*	4.��doMain()������ʹ��getData()��getDataBuffer()������ȡ��������ݣ��������˵��
*	5.ʹ��start() stop() �����߳��п���������ݴ����̣߳�ʹ��pause() resume()
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/

#pragma once
#include "DataAcquireBase.h"
#include <string>
#include <vector>
#include <assert.h>
#include <iostream>
#include <string>
#include <map>


#define DATA_PROCESS_THREAD_DEFAULT_NAME	std::string("default")

#undef THREAD_RESUME

/**
	@brief ���߳���ص������ռ䣬�ں�CDataProcessBase�� �� CDataAcquire��
*/
namespace thr {
	/**
	@brief ���̵߳��ȿ��Ƶ�ö������
	*/
	typedef enum {

		THREAD_PAUSE,	/**< ��ͣ */
		THREAD_RESUME,	/**< ���� */
		THREAD_TURN,	/**< ��ͣ-�����л� */
		THREAD_STOP		/**< ���� */
	} ThreadCtrl_t;

	/**
	@brief �߳��������ö������
	*/
	typedef enum {
		_RUNNING,	/**< ���� */
		_PAUSEING,	/**< ��ͣ */
		_STOPPING	/**< ���� */
	}ThreadStates_t;

	/**
	@brief �߳�����״̬������

	�����̵߳����ֺ�����״̬
	*/
	typedef struct _ThreadInfo_t
	{
		std::string name;
		ThreadStates_t state;
		_ThreadInfo_t(std::string n, ThreadStates_t s) :name(n), state(s) {};
		_ThreadInfo_t(std::string n) :name(n),state(_STOPPING) {};
		/**
		@brief ���ص���������

		ֻҪname��ȣ� ���ж���ȣ����ڲ��Һ���
		*/
		bool operator == (const _ThreadInfo_t &t) { return (name == t.name); }
	}ThreadInfo_t;


	/**
	 @brief ���߳����ݴ���Ļ���

	 ���Ҫʹ�ö��߳̿�ܣ���������ݴ�����̳������
	*/
	class CDataProcessBase
	{

	private:

		/**
		@brief �桤�߳����

		���Ȼ����processInit()�����Զ����ʼ�����Դ�while(1)ѭ������ͣ/����
		*/
		void _doMain(void);

		/**
		@brief ����Դ�б���Ҫ�̳���CDataAcquireBase<T>
		*/
		std::vector<_CDataAcquireBase*> _dataSourceList;

		/**
		@brief �߳������б����
		*/
		static std::mutex _runningListMutex;

	public:

		/**
		@brief ���캯�������߳�����
		@param threadName ���̵߳����� Ĭ��Ϊstring("default")
		*/
		CDataProcessBase(std::string threadName = DATA_PROCESS_THREAD_DEFAULT_NAME);

		/**
		@brief ��������
		*/
		virtual ~CDataProcessBase();

		/**
		@brief �߳̿�ʼ����

		����һ���̣߳�ִ��processInit���������߳���Ϣ�����̹߳����б�
		isRunningImmediately������������running״̬������pause״̬��
		����߳��������У�����false

		@param isRunningImmediately �Ƿ��������У�Ĭ��Ϊtrue
		@return �Ƿ񴴽��ɹ�
		*/
		bool start(bool isRunningImmediately = true);
		
		/**
		@brief ���һ������Դ

		@param T ����Դ�е�ÿһ֡���ݵ�����
		@param data �̳���CDataAcquire<T>������Դ��
		*/
		template<typename T>
		void addDataSource(CDataAcquireBase<T> &data);

		/**
		@brief �ӵ�n������Դ�л�ȡ���µ�һ֡����

		���û�����ݻ����[dataSource] data dataSourceIndex is empyt"

		@param T ����Դ�е�ÿһ֡���ݵ�����
		@param dataSourceIndex [in] ����Դ�ı��
		@param data [out] �������ݵı���
		*/

		template<typename T>
		unsigned long getData(unsigned int dataSourceIndex, T &data);
		
		/**
		@brief �ӵ�n������Դ�л�ȡ��begin��end������m֡����
		
		֡��Ŵ����µ���ɷֱ�Ϊ0~bufferSize-1(bufferSize������Դ����Ĵ�С)
		��ȡ��������vector�У�����֡����ǰ�档
		����ֵΪ֡��ţ���Ŵ�1~DATA_ACQUIRE_MAX_DATA_INDEX ���CDataAcquire��
		����Ҫ��end >= begin  end < bufferSize
		���ݲ������� [ProcessBase::GetData()] data "dataSourceIndex" is not enough"

		@param T ����Դ�е�ÿһ֡���ݵ�����
		@param dataSourceIndex [in] ����Դ�ı��		
		@param data [out] �������ݵı�����������vector<T>
		@param end [in] ��ȡ���ݵ����֡�ı��
		@param begin [in] ��ȡ���ݵ�����֡�ı�� Ĭ��Ϊ0
		@return ����֡�ı��
		*/
		template<typename T>
		unsigned long getData(unsigned int dataSourceIndex, std::vector<T> &data, unsigned int end, unsigned int begin = 0);

		/**
		@brief �ӵ�n������Դ�л�ȡȫ������
		
		��ȡ��������vector�У�����֡����ǰ��(��0λ)��
		���ݲ���ʱ���[dataSource] data buff dataSourceIndex is not enough
		@param T ����Դ�е�ÿһ֡���ݵ�����
		@param dataSourceIndex [in] ����Դ�ı��		
		@param data [out] �������ݵı�����������vector<T>
		*/
		template<typename T>
		unsigned long getDataBuffer(unsigned int dataSourceIndex, std::vector<T> &data);

		/**
		@brief ��ͣ���ָ�����ֹͣĳ���߳�
	
		@param ctrl ���̵߳Ĳ�������� ThreadCtrl_t ö������
		@param name �̵߳����֣����̹߳����ʱ���ƶ������CDataProcessBase���캯��
		*/
		static bool multiThreadManage(ThreadCtrl_t ctrl, std::string name);

		/**
		@brief ���̵߳�����
		*/
		std::string threadName;

	protected:
		/**
		@brief �߳���ͣ
		
		�߳̽����running����Pause״̬�������ʼ״̬����running ����false

		@return �Ƿ���ͣ�ɹ�
		*/
		bool pause();

		/**
		@brief �ָ̻߳�

		�߳̽����Pause����running״̬�������ʼ״̬����Pause ����false
		@return �Ƿ�ָ��ɹ�
		*/
		bool resume();

		/**
		@brief �߳�ֹͣ

		����һ���̣߳�ִ��processDeinit���������߳���Ϣ���̹߳����б�ɾ����
		����̲߳����ڣ�����false

		@return �Ƿ�ֹͣ�ɹ�
		*/
		bool stop();

		/**
		@brief ��ѭ������
		
		���麯����ǿ����д����_doMain()�е��ã��Դ�while(1)�������������_doMain()
		*/
		virtual void doMain() = 0;

		/**
		@brief �û��Զ����ʼ������
		
		���麯����ǿ����д����_doMain()�����ȵ��ã����_doMain()
		��ʼ��ʧ�ܻ���� **** [CDataProcessBase] init failed! Thread error! ****
		@return ��ʼ��ʧ�ܷ���false ���򷵻�true
		*/
		virtual bool processInit() = 0;

		/**
		@brief �û��Զ������ٺ���
		
		���麯����ǿ����д����_doMain()���߳̽���ǰ���ã����_doMain()
		@return ����ʧ�ܷ���false ���򷵻�true
		*/
		virtual bool processDeinit() = 0;

		/// TODO: �ƺ�runningThreadListȱ���̰߳�ȫ����
		/**
		@brief �߳������б��б��еı���Ϊ�������С���ͣ���߳�

		����Ϊ��̬�����������̴߳����๲��
		*/
		static std::vector<ThreadInfo_t> runningThreadList2;

		
		/**
		@brief �����̻߳���/��ͣ����������
		*/
		static std::condition_variable runningListCondVari;


	};

	template<typename T>
	void CDataProcessBase::addDataSource(CDataAcquireBase<T>& dat)
	{
		_dataSourceList.push_back((_CDataAcquireBase*)(&dat));
	}

	template<typename T>
	unsigned long CDataProcessBase::getData(unsigned int dataSourceIndex, T& data) {
		// �Ӵ�����Դ�б����ҵ�����Դ
		CDataAcquireBase<T>*  dataSource = (CDataAcquireBase<T>*) _dataSourceList[dataSourceIndex];

		// �ж�����Դ�Ƿ�������
		if (dataSource->data.empty()) {
			std::cout << "[dataSource] data \"" << dataSourceIndex << "\" is empyt" << std::endl;
			return 0;
		}
		else {
			// ��ȡ����
			{
				std::unique_lock<std::mutex> lck(dataSource->dataMutex);	
				dataSource->dataCpy(dataSource->data.back(), data);
			}
			return dataSource->dataIndex;
		}


	}

	template<typename T>
	unsigned long CDataProcessBase::getData(unsigned int dataSourceIndex, std::vector<T> &data, unsigned int end, unsigned int begin)
	{
		// �Ӵ�����Դ�б����ҵ�����Դ
		assert(end >= begin);
		CDataAcquireBase<T>*  dataSource = (CDataAcquireBase<T>*) _dataSourceList[dataSourceIndex];
		assert(end < dataSource->_bufferSize);

		// ���������ݲ���
		auto dataSize = dataSource->data.size();
		if (dataSize <= end) {
			std::cout << "[ProcessBase::GetData()] data \" " << dataSourceIndex << " \" is not enough" << std::endl;
			return 0;
		}
		// ��ȡ����������
		else {
			std::unique_lock<std::mutex> lck(dataSource->dataMutex);	// lock
			std::queue<T> dataBufferTemp(dataSource->data);

			// �����ݻ������ҵ���Ҫ������
			for (unsigned int i = 0; i < dataSize - 1 - end; i++)
				dataBufferTemp.pop();

			for (unsigned int i = 0; i < end - begin + 1; i++) {
				T dataTemp;
				dataSource->dataCpy(dataBufferTemp.front(), dataTemp);
				data.push_back(dataTemp);
				dataBufferTemp.pop();
			}
		}
		return dataSource->dataIndex;
	}

	template<typename T>
	unsigned long CDataProcessBase::getDataBuffer(unsigned int dataSourceIndex, std::vector<T>& data)
	{
		CDataAcquireBase<T>*  dataSource = (CDataAcquireBase<T>*) _dataSourceList[dataSourceIndex];
		if (dataSource->data.empty()) {
			std::cout << "[dataSource] data buff \"" << dataSourceIndex << "\" is not enough" << std::endl;
			return 0;
		}
		else {
			{
				std::unique_lock<std::mutex> lck(dataSource->dataMutex);	// lock
				std::queue<T> dataBufferTemp(dataSource->data);
				while (dataBufferTemp.size() != 0) {
					T dataTemp;
					dataSource->dataCpy(dataBufferTemp.front(), dataTemp);
					data.push_back(dataTemp);
					dataBufferTemp.pop();
				}
			}
			return dataSource->data.size();
		}
	}

}

using namespace thr;
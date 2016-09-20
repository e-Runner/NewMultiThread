/**
* @file DataAcquireBase.h
* @brief ���߳����ݻ�ȡ�Ļ��������ļ�������ʵ�ֶ��߳����ݻ�ȡ
*
* �������ȡ���ݵ����͵ĳ��������ڴ��࣬RealSense�࣬ IMU�࣬ ���ײ��״��ȡ��
* ʹ�÷�����
*	1.�������̳�����ࣻ
*	2.ʵ��doMain(),init(),deinit(),dataCpy()���ĸ����麯����doMain()Ϊ��ѭ����init()��deinit()Ϊ��ѭ����ʼǰ��ĳ�ʼ�������٣�dataCpy()Ϊ���ݿ�������
*	3.��doMain�����н���ȡ�����������ݿ�����dataTem�������ܻ��Զ���dataTem�е�������ӵ����ݻ������
*
* @author huweijian
* @version 1.0
* @date 2016.9.7
*/

#pragma once
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <atomic>

#define DATA_ACQUIRE_DEFAULE_BUFFER_SIZE	3
#define DATA_ACQUIRE_MAX_DATA_INDEX			100000000

/**
	@brief ���߳���ص������ռ䣬�ں�CDataProcessBase�� �� CDataAcquire��
*/
namespace thr{

	/**
	 @brief ���߳����ݻ�ȡ�Ļ���Ļ���
	*/
	class _CDataAcquireBase {
	public:
		_CDataAcquireBase() {};
		~_CDataAcquireBase() {};
	};
	


	/**
	@brief ���߳����ݻ�ȡ�Ļ���

	���Ҫʹ�ö��߳̿�ܣ���������ݻ�ȡ��̳������

	@param T ÿһ֡���ݵ�����

	*/
	template<typename T>
	class CDataAcquireBase: public _CDataAcquireBase
	{
	public:
		/**
		@brief ���캯��

		@param buffSize����Ҫ���ݻ���Ĵ�С Ĭ��Ϊ 3
		*/
		CDataAcquireBase(unsigned int buffSize = DATA_ACQUIRE_DEFAULE_BUFFER_SIZE);

		/**
		@brief ��������
		*/
		virtual ~CDataAcquireBase();

		/**
		@brief ��ʼ�ɼ�����
		*/
		void start();

		/**
		@brief ������
		*/
		std::mutex dataMutex;

		/**
		@brief �ɼ��������ݻ�����У��������ڶ���ǰ�ˣ��������ڶ��к��
		*/
		std::queue<T> data;

		/**
		@brief �û��Զ�������ݿ�������

		��ͬ�������в�ͬ�Ŀ���������ֻҪ���߳�����ΰ�dataSrc������dataDest������ 
		����������飬memcpy(dataSrc, dataDest); 
		����Mat: dataDest = dataSrc.clone();
		
		@param dataSrc �������
		@param dataDest �����յ�
		*/
		virtual void dataCpy(T dataSrc, T &dataDest) = 0;

		/**
		@brief �������ݵı�ţ�������Ϊ��unsigned long ���ͣ���1~DATA_ACQUIRE_DEFAULE_BUFFER_SIZEѭ�����𽥵���
		*/
		std::atomic<unsigned long> dataIndex;

		/**
		@brief ���ݻ���Ĵ�С
		*/
		unsigned int _bufferSize;

	private:// ����private�ľͱ��˼��� >_<!!!
		// ���߳���ں�����ʵ�����������û����麯��doMain
		/**
		@brief �桤���߳���ں������Դ�while(1)����������
		ִ���û���doMain���� �������ݴ�dataTem�п��������뻺�����
		*/
		void _doMain(void);

		/**
		@brief ���ĳ�ʼ�����
		*/
		bool _dataMutexIsInit;

	protected:
		/**
		@brief �ղɼ������������ݣ���doMain�ɼ������ݺ������������У�ע��Ҫ������������
		*/
		T dataTem;

		/**
		@brief �û��Զ������ѭ����

		�ɼ�һ֡���ݵĹ��̣��ɼ���ɺ�������ݴ�ŵ�dataTem���������
		*/
		virtual void doMain(void) = 0;

		/**
		@brief �û��Զ���ĳ�ʼ������

		����ѭ����֮ǰִ�У�
		��ʼ��ʧ����� **** [CDataAcquireBase] init failed! thread error ****

		@return ��ʼ���Ľ��
		*/
		virtual bool init(void) = 0;


		/**
		@brief �û��Զ�������ٺ���
		
		��ʱ���ã��Ժ�϶��õ��� = =�������ڴ�
		*/
		virtual bool deinit(void) = 0;

	};


	template<typename T>
	CDataAcquireBase<T>::CDataAcquireBase(unsigned int buffSize):
		_bufferSize(buffSize), 
		_dataMutexIsInit(false),
		dataIndex(0)
	{
	}

	template<typename T>
	CDataAcquireBase<T>::~CDataAcquireBase()
	{
	}

	template<typename T>
	void  CDataAcquireBase<T>::start() {
		thread *t = new thread(&CDataAcquireBase::_doMain, this);
		return ;
	}

	// �桤���߳���ں���
	template<typename T>
	void CDataAcquireBase<T>::_doMain()
	{
		// ��ʼ��
		if (!init()) {
			std::cout << "**** [CDataAcquireBase] init failed! thread error ****" << std::endl;
			return;
		}
		// ��������ʼ��
		dataMutex.lock();

		while (1) {
			doMain();
			{
				// �����
				std::unique_lock<std::mutex> lck;
				if (_dataMutexIsInit)
					lck = std::unique_lock<std::mutex>(dataMutex);
				else {
					lck = std::unique_lock<std::mutex>(dataMutex, std::adopt_lock);
					_dataMutexIsInit = true;
				}

				// ������������ӵ�����
				if (data.size() >= _bufferSize)
					data.pop();
				T dataTemCpy;
				dataCpy(dataTem, dataTemCpy);
				data.push(dataTemCpy);

				// ���ݱ�Ÿ���
				if (dataIndex++ >= DATA_ACQUIRE_MAX_DATA_INDEX)
					dataIndex = 1;


			}
		}

	}

}
using namespace thr;

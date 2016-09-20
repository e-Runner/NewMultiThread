/**
* @file DataAcquireBase.h
* @brief 多线程数据获取的基类所在文件，用于实现多线程数据获取
*
* 从外设获取数据的类型的程序适用于此类，RealSense类， IMU类， 毫米波雷达获取类
* 使用方法：
*	1.将你的类继承这个类；
*	2.实现doMain(),init(),deinit(),dataCpy()这四个纯虚函数，doMain()为主循环，init()和deinit()为主循环开始前后的初始化和销毁，dataCpy()为数据拷贝函数
*	3.在doMain函数中将获取到的最新数据拷贝到dataTem变量里，框架会自动将dataTem中的数据添加到数据缓存队列
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
	@brief 多线程相关的命名空间，内涵CDataProcessBase类 和 CDataAcquire类
*/
namespace thr{

	/**
	 @brief 多线程数据获取的基类的基类
	*/
	class _CDataAcquireBase {
	public:
		_CDataAcquireBase() {};
		~_CDataAcquireBase() {};
	};
	


	/**
	@brief 多线程数据获取的基类

	如果要使用多线程框架，把你的数据获取类继承这个类

	@param T 每一帧数据的类型

	*/
	template<typename T>
	class CDataAcquireBase: public _CDataAcquireBase
	{
	public:
		/**
		@brief 构造函数

		@param buffSize　需要数据缓存的大小 默认为 3
		*/
		CDataAcquireBase(unsigned int buffSize = DATA_ACQUIRE_DEFAULE_BUFFER_SIZE);

		/**
		@brief 析构函数
		*/
		virtual ~CDataAcquireBase();

		/**
		@brief 开始采集数据
		*/
		void start();

		/**
		@brief 数据锁
		*/
		std::mutex dataMutex;

		/**
		@brief 采集到的数据缓存队列，旧数据在队列前端，新数据在队列后端
		*/
		std::queue<T> data;

		/**
		@brief 用户自定义的数据拷贝方法

		不同的数据有不同的拷贝方法，只要告诉程序如何把dataSrc拷贝到dataDest就行了 
		比如对于数组，memcpy(dataSrc, dataDest); 
		对于Mat: dataDest = dataSrc.clone();
		
		@param dataSrc 拷贝起点
		@param dataDest 拷贝终点
		*/
		virtual void dataCpy(T dataSrc, T &dataDest) = 0;

		/**
		@brief 最新数据的编号，可以认为是unsigned long 类型，从1~DATA_ACQUIRE_DEFAULE_BUFFER_SIZE循环，逐渐递增
		*/
		std::atomic<unsigned long> dataIndex;

		/**
		@brief 数据缓存的大小
		*/
		unsigned int _bufferSize;

	private:// 都是private的就别看人家啦 >_<!!!
		// 多线程入口函数，实现锁，调用用户纯虚函数doMain
		/**
		@brief 真·多线程入口函数，自带while(1)和数据锁，
		执行用户的doMain函数 并将数据从dataTem中拷出，加入缓存队列
		*/
		void _doMain(void);

		/**
		@brief 锁的初始化标记
		*/
		bool _dataMutexIsInit;

	protected:
		/**
		@brief 刚采集到的新鲜数据，在doMain采集到数据后放入这个变量中，注意要深拷贝入这个变量
		*/
		T dataTem;

		/**
		@brief 用户自定义的主循环体

		采集一帧数据的过程，采集完成后请把数据存放到dataTem这个变量中
		*/
		virtual void doMain(void) = 0;

		/**
		@brief 用户自定义的初始化函数

		在主循环体之前执行，
		初始化失败输出 **** [CDataAcquireBase] init failed! thread error ****

		@return 初始化的结果
		*/
		virtual bool init(void) = 0;


		/**
		@brief 用户自定义的销毁函数
		
		暂时无用，以后肯定用得上 = =！敬请期待
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

	// 真·多线程入口函数
	template<typename T>
	void CDataAcquireBase<T>::_doMain()
	{
		// 初始化
		if (!init()) {
			std::cout << "**** [CDataAcquireBase] init failed! thread error ****" << std::endl;
			return;
		}
		// 数据锁初始化
		dataMutex.lock();

		while (1) {
			doMain();
			{
				// 检查锁
				std::unique_lock<std::mutex> lck;
				if (_dataMutexIsInit)
					lck = std::unique_lock<std::mutex>(dataMutex);
				else {
					lck = std::unique_lock<std::mutex>(dataMutex, std::adopt_lock);
					_dataMutexIsInit = true;
				}

				// 把新鲜数据添加到队列
				if (data.size() >= _bufferSize)
					data.pop();
				T dataTemCpy;
				dataCpy(dataTem, dataTemCpy);
				data.push(dataTemCpy);

				// 数据标号更新
				if (dataIndex++ >= DATA_ACQUIRE_MAX_DATA_INDEX)
					dataIndex = 1;


			}
		}

	}

}
using namespace thr;

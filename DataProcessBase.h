/**
* @file DataProcessBase.h
* @brief 多线程数据处理的基类所在文件，用于实现多线程数据处理和线程调度
*
* 数据处理类型的程序适用于此类，比如纸币检测（处理彩色图像数据）、障碍物检测（处理深度图像和姿态角数据）
* 使用方法：
*	1.将你的类继承这个类；
*	2.实现doMain(),processInit(),processDeinit()这三个纯虚函数，doMain为主循环，processInit()和processDeinit()为主循环开始前后的初始化和销毁
*	3.使用addDataSource()函数添加一个数据源，详见函数说明
*	4.在doMain()函数中使用getData()和getDataBuffer()函数获取具体的数据，详见函数说明
*	5.使用start() stop() 在主线程中开启你的数据处理线程，使用pause() resume()
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
	@brief 多线程相关的命名空间，内涵CDataProcessBase类 和 CDataAcquire类
*/
namespace thr {
	/**
	@brief 多线程调度控制的枚举类型
	*/
	typedef enum {

		THREAD_PAUSE,	/**< 暂停 */
		THREAD_RESUME,	/**< 继续 */
		THREAD_TURN,	/**< 暂停-继续切换 */
		THREAD_STOP		/**< 结束 */
	} ThreadCtrl_t;

	/**
	@brief 线程自身控制枚举类型
	*/
	typedef enum {
		_RUNNING,	/**< 运行 */
		_PAUSEING,	/**< 暂停 */
		_STOPPING	/**< 结束 */
	}ThreadStates_t;

	/**
	@brief 线程运行状态的类型

	包括线程的名字和运行状态
	*/
	typedef struct _ThreadInfo_t
	{
		std::string name;
		ThreadStates_t state;
		_ThreadInfo_t(std::string n, ThreadStates_t s) :name(n), state(s) {};
		_ThreadInfo_t(std::string n) :name(n),state(_STOPPING) {};
		/**
		@brief 重载的相等运算符

		只要name相等， 即判定相等，用于查找函数
		*/
		bool operator == (const _ThreadInfo_t &t) { return (name == t.name); }
	}ThreadInfo_t;


	/**
	 @brief 多线程数据处理的基类

	 如果要使用多线程框架，把你的数据处理类继承这个类
	*/
	class CDataProcessBase
	{

	private:

		/**
		@brief 真·线程入口

		首先会调用processInit()进行自定义初始化，自带while(1)循环和暂停/唤醒
		*/
		void _doMain(void);

		/**
		@brief 数据源列表，需要继承于CDataAcquireBase<T>
		*/
		std::vector<_CDataAcquireBase*> _dataSourceList;

		/**
		@brief 线程运行列表的锁
		*/
		static std::mutex _runningListMutex;

	public:

		/**
		@brief 构造函数，给线程命名
		@param threadName 本线程的名字 默认为string("default")
		*/
		CDataProcessBase(std::string threadName = DATA_PROCESS_THREAD_DEFAULT_NAME);

		/**
		@brief 析构函数
		*/
		virtual ~CDataProcessBase();

		/**
		@brief 线程开始运行

		创建一个线程，执行processInit函数，将线程信息加入线程管理列表，
		isRunningImmediately参数决定进入running状态或者是pause状态，
		如果线程正在运行，返回false

		@param isRunningImmediately 是否立即运行，默认为true
		@return 是否创建成功
		*/
		bool start(bool isRunningImmediately = true);
		
		/**
		@brief 添加一个数据源

		@param T 数据源中的每一帧数据的类型
		@param data 继承于CDataAcquire<T>的数据源类
		*/
		template<typename T>
		void addDataSource(CDataAcquireBase<T> &data);

		/**
		@brief 从第n个数据源中获取最新的一帧数据

		如果没有数据会输出[dataSource] data dataSourceIndex is empyt"

		@param T 数据源中的每一帧数据的类型
		@param dataSourceIndex [in] 数据源的编号
		@param data [out] 接收数据的变量
		*/

		template<typename T>
		unsigned long getData(unsigned int dataSourceIndex, T &data);
		
		/**
		@brief 从第n个数据源中获取从begin到end的连续m帧数据
		
		帧编号从最新到最旧分别为0~bufferSize-1(bufferSize是数据源缓存的大小)
		获取到的数据vector中，最新帧在最前面。
		返回值为帧编号，编号从1~DATA_ACQUIRE_MAX_DATA_INDEX 详见CDataAcquire类
		断言要求end >= begin  end < bufferSize
		数据不足会输出 [ProcessBase::GetData()] data "dataSourceIndex" is not enough"

		@param T 数据源中的每一帧数据的类型
		@param dataSourceIndex [in] 数据源的编号		
		@param data [out] 接收数据的变量，类型是vector<T>
		@param end [in] 获取数据的最旧帧的编号
		@param begin [in] 获取数据的最新帧的编号 默认为0
		@return 最新帧的编号
		*/
		template<typename T>
		unsigned long getData(unsigned int dataSourceIndex, std::vector<T> &data, unsigned int end, unsigned int begin = 0);

		/**
		@brief 从第n个数据源中获取全部数据
		
		获取到的数据vector中，最新帧在最前面(第0位)。
		数据不足时输出[dataSource] data buff dataSourceIndex is not enough
		@param T 数据源中的每一帧数据的类型
		@param dataSourceIndex [in] 数据源的编号		
		@param data [out] 接收数据的变量，类型是vector<T>
		*/
		template<typename T>
		unsigned long getDataBuffer(unsigned int dataSourceIndex, std::vector<T> &data);

		/**
		@brief 暂停、恢复或者停止某个线程
	
		@param ctrl 对线程的操作，详见 ThreadCtrl_t 枚举类型
		@param name 线程的名字，在线程构造的时候制定，详见CDataProcessBase构造函数
		*/
		static bool multiThreadManage(ThreadCtrl_t ctrl, std::string name);

		/**
		@brief 本线程的名字
		*/
		std::string threadName;

	protected:
		/**
		@brief 线程暂停
		
		线程进入从running进入Pause状态，如果初始状态不是running 返回false

		@return 是否暂停成功
		*/
		bool pause();

		/**
		@brief 线程恢复

		线程进入从Pause进入running状态，如果初始状态不是Pause 返回false
		@return 是否恢复成功
		*/
		bool resume();

		/**
		@brief 线程停止

		结束一个线程，执行processDeinit函数，将线程信息从线程管理列表删除，
		如果线程不存在，返回false

		@return 是否停止成功
		*/
		bool stop();

		/**
		@brief 主循环主体
		
		纯虚函数，强制重写，在_doMain()中调用，自带while(1)和数据锁，详见_doMain()
		*/
		virtual void doMain() = 0;

		/**
		@brief 用户自定义初始化函数
		
		纯虚函数，强制重写，在_doMain()中首先调用，详见_doMain()
		初始化失败会输出 **** [CDataProcessBase] init failed! Thread error! ****
		@return 初始化失败返回false 否则返回true
		*/
		virtual bool processInit() = 0;

		/**
		@brief 用户自定义销毁函数
		
		纯虚函数，强制重写，在_doMain()中线程结束前调用，详见_doMain()
		@return 销毁失败返回false 否则返回true
		*/
		virtual bool processDeinit() = 0;

		/// TODO: 似乎runningThreadList缺乏线程安全机制
		/**
		@brief 线程运行列表，列表中的变量为正在运行、暂停的线程

		变量为静态变量，所有线程处理类共享
		*/
		static std::vector<ThreadInfo_t> runningThreadList2;

		
		/**
		@brief 用于线程唤醒/暂停的条件变量
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
		// 从从数据源列表中找到数据源
		CDataAcquireBase<T>*  dataSource = (CDataAcquireBase<T>*) _dataSourceList[dataSourceIndex];

		// 判断数据源是否有数据
		if (dataSource->data.empty()) {
			std::cout << "[dataSource] data \"" << dataSourceIndex << "\" is empyt" << std::endl;
			return 0;
		}
		else {
			// 获取数据
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
		// 从从数据源列表中找到数据源
		assert(end >= begin);
		CDataAcquireBase<T>*  dataSource = (CDataAcquireBase<T>*) _dataSourceList[dataSourceIndex];
		assert(end < dataSource->_bufferSize);

		// 缓冲区数据不足
		auto dataSize = dataSource->data.size();
		if (dataSize <= end) {
			std::cout << "[ProcessBase::GetData()] data \" " << dataSourceIndex << " \" is not enough" << std::endl;
			return 0;
		}
		// 获取缓冲区数据
		else {
			std::unique_lock<std::mutex> lck(dataSource->dataMutex);	// lock
			std::queue<T> dataBufferTemp(dataSource->data);

			// 从数据缓存中找到需要的数据
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
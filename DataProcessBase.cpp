#include "DataProcessBase.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <condition_variable>	
#include <algorithm>
#include <assert.h>

// 运行线程表
std::vector<ThreadInfo_t> CDataProcessBase::runningThreadList2;
// 线程运行列表的锁
std::mutex CDataProcessBase::_runningListMutex;
// 模式判断条件变量
std::condition_variable CDataProcessBase::runningListCondVari;

CDataProcessBase::CDataProcessBase(std::string threadName)
	:threadName(threadName)
{

}

CDataProcessBase::~CDataProcessBase()
{
}

bool CDataProcessBase::start(bool isRunningImmediately ) {

	auto it = std::find(runningThreadList2.begin(), runningThreadList2.end(), threadName);

	if (it != runningThreadList2.end())
		return false;

	if (isRunningImmediately) {
		// 把当前线程加入runningList
		runningThreadList2.push_back(ThreadInfo_t(threadName, _RUNNING));
	}
	else {
		runningThreadList2.push_back(ThreadInfo_t(threadName, _PAUSEING));
	}

	std::thread *t = new std::thread(&CDataProcessBase::_doMain, this);
	return true;

}

bool CDataProcessBase::pause()
{
	auto it = std::find(runningThreadList2.begin(), runningThreadList2.end(), threadName);

	if (it == runningThreadList2.end() || (*it).state != _RUNNING)
		return false;
	
	(*it).state = _PAUSEING;

	runningListCondVari.notify_all();
	return true;
}

bool CDataProcessBase::resume()
{
	auto it = std::find(runningThreadList2.begin(), runningThreadList2.end(), (threadName));

	if (it == runningThreadList2.end() || (*it).state != _PAUSEING)
		return false;

	(*it).state = _RUNNING;

	runningListCondVari.notify_all();
	return true;
}

bool CDataProcessBase::stop()
{
	auto it = std::find(runningThreadList2.begin(), runningThreadList2.end(), (threadName));

	if (it == runningThreadList2.end() )
		return false;
	(*it).state = _STOPPING;
	runningListCondVari.notify_all();

	return true;
}

void CDataProcessBase::_doMain(void) {
	// 执行初始化函数
	if (!processInit()) {
		std::cout << "**** [CDataProcessBase] init failed! Thread error! ****" << std::endl;
		return;
	}

	// 线程主循环
	while (1) {

		// 检查当前线程是否可以运行(线程存在，并且RUNNING)
		{
			std::unique_lock<std::mutex> lck(_runningListMutex);
			
			while(1){
				auto it = std::find(runningThreadList2.begin(), runningThreadList2.end(), (threadName));
				// 线程标记为STOP
				if ( (*it).state == _STOPPING) {
					runningThreadList2.erase(it);
					processDeinit();
					return;
				}
				// 线程被标记为PAUSE
				if ((*it).state == _PAUSEING)
					runningListCondVari.wait(lck);
				else
					break;

			}
		}
		
		// 用户自定义的循环体
		doMain();
	}
	
}

bool CDataProcessBase::multiThreadManage(ThreadCtrl_t ctrl, std::string name)
{
	using namespace std;

	auto it = find(runningThreadList2.begin(), runningThreadList2.end(), (name));
	if (it == runningThreadList2.end())
		return false;

	switch (ctrl)
	{
	case THREAD_PAUSE:
		
		if ((*it).state != _RUNNING)	// 暂停一个没有运行的线程
			return false;
		else
			(*it).state = _PAUSEING;
		break;

	case THREAD_RESUME:
		if ( (*it).state != _PAUSEING)	// 继续一个正在运行的线程
			return false;
		else
			(*it).state = _RUNNING;
		break;

	case THREAD_TURN:

			if ((*it).state == _RUNNING)
				(*it).state = _PAUSEING;
			else if ((*it).state == _PAUSEING)
				(*it).state = _RUNNING;

		break;
	case THREAD_STOP:
		runningThreadList2.erase(it);
		break;
	default:
		return false;
	}
	runningListCondVari.notify_all();


	cout << endl << "Running Thread List : ";
	for (auto &i : runningThreadList2) {
		cout << i.name << " ";
	}
	cout << endl;

	return true;
}
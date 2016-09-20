#include "RunningSelect.h"
#include <conio.h>	
#include <iostream>
#include <algorithm>
#include <cstring>


CRunningSelect::CRunningSelect():
	CDataProcessBase("sel")
{
}


CRunningSelect::~CRunningSelect()
{
}

void CRunningSelect::doMain()
{
	using namespace std;
	if (_kbhit()) {
		char key = _getch();
		vector<std::string>::iterator it;
		switch (key){
		case 'o':
			multiThreadManage(THREAD_TURN, "obs");
			
			break;
		case 'f':
			multiThreadManage(THREAD_TURN, "face");
			break;

		default:
			break;
		}
	}
	
}

bool CRunningSelect::processInit()
{
	return true;
}

bool CRunningSelect::processDeinit()
{
	return false;
}

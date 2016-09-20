#include "ObsDetect.h"

#include <vector>
#include <iostream>
#include <Windows.h>

CObsDetect::CObsDetect():
	CDataProcessBase("obs")
{
}


CObsDetect::~CObsDetect()
{
}

void CObsDetect::doMain(void)
{
	using namespace std;

	vector<float> cmData;
	double rsData;
	cout << "[ " << threadName;
	if (auto index = getData<vector<float> >(1,cmData )) {
		cout << index << " ] ";
		for (auto i = cmData.begin(); i != cmData.end(); i++) {
			cout << *i << " ";
		}

		if (index == 15) {
			stop();
		}

		
	}
	
	cout << endl;
	Sleep(123);



}

bool CObsDetect::processInit(void)
{
	return true;
}

bool CObsDetect::processDeinit()
{
	return false;
}

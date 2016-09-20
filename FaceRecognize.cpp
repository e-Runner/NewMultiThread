#include "FaceRecognize.h"

#include <vector>
#include <iostream>
#include <Windows.h>



CFaceRecognize::CFaceRecognize() :
	CDataProcessBase("face")
{
}


CFaceRecognize::~CFaceRecognize()
{
}

void CFaceRecognize::doMain()
{
	using namespace std;

	vector<double> rsDataBuf;
	double rsData;
	cout <<"[ " << threadName;
	if (auto index = getData<double>(0, rsDataBuf, 1)) {
		cout << index << " ] ";
		for (auto i = rsDataBuf.begin(); i != rsDataBuf.end(); i++) {
			cout << *i << " ";
		}
	}
	cout << endl;
	Sleep(200);

}

bool CFaceRecognize::processInit()
{
	return true;
}

bool CFaceRecognize::processDeinit()
{
	return false;
}

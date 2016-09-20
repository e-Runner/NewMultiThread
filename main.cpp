#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex>

#include "Comm.h"
#include "Realsense.h"
#include "ObsDetect.h"
#include "FaceRecognize.h"
#include "RunningSelect.h"
using namespace std;

int main(void) {

	CComm cm;
	cm.start();

	CRealsense rs;
	rs.start();

	CObsDetect ob;
	ob.addDataSource<double>(rs);
	ob.addDataSource< vector<float> >(cm);
	ob.start();

	//CFaceRecognize fr;
	//fr.addDataSource<double>(rs);
	//fr.start();

	CRunningSelect sel;
	sel.start();

	while (1) {
		using std::cout;
		Sleep(1000);
		cout << ".";
	
	}
	return 0;
}
#include "Comm.h"
#include <iostream>
#include <Windows.h>


CComm::CComm()
{
}


CComm::~CComm()
{
}
 
void CComm::doMain(void) {
	static float i = 0;
	i++;
	std::vector<float> a(8, i);
	dataTem = a;
	Sleep(1000);


}

void CComm::dataCpy(std::vector<float> src, std::vector<float>& dst)
{
	for (auto i = src.begin(); i != src.end(); i++) {
		dst.push_back(*i);
	}
}

bool CComm::init(void)
{
	return true;
}

bool CComm::deinit(void)
{
	return true;
}





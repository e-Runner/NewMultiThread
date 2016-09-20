#include "Realsense.h"
#include <iostream>
#include <Windows.h>

CRealsense::CRealsense()
{
	
}


CRealsense::~CRealsense()
{
}

void CRealsense::doMain(void)
{
	static double i = 0 ;
	i += 1.1;
	dataTem = i;
	Sleep(230);

	
}

void CRealsense::dataCpy(double s, double & d)
{
	d = s;
}

bool CRealsense::init(void)
{
	return true;
}

bool CRealsense::deinit(void)
{
	return true;
}



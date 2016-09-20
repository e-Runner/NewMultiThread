#pragma once
#include "DataProcessBase.h"
#include <string>
class CRunningSelect :
	public CDataProcessBase
{
public:
	CRunningSelect();
	~CRunningSelect();
	void doMain();
	bool processInit();
	bool processDeinit();
};


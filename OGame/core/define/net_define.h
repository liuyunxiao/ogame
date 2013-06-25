#pragma once

#include "type_define.h"
const INT GT_NET_BLOCK_TIME = 1000*150;	// 150 ms

struct msgBase
{
	DWORD	dwID;
	union
	{
		DWORD	dwMsgInfo;///dwSize;
		struct
		{
			DWORD	dwSize : 24;
			DWORD	bSec : 2;
			DWORD	bCompress : 3;
			DWORD	bReserved : 3;			
		};
	};

	//DWORD Crc32(LPCSTR lpData);dwID = Crc32(#name); 
};

#ifndef NMDECAL_START
#define NMDECAL_START(name)	\
struct msg##name : public msgBase{	\
	msg##name(){ ZeroMemory(this,sizeof(*this));  \
	dwSize = sizeof(*this); }
#define NMDECAL_END		};
#endif


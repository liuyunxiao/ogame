//
//  filter_define.h
//  RiddleGame
//
//  Created by quwei on 13-4-25.
//
//

#pragma once

#include "type_define.h"

enum EFM_FILTER	
{
	FMF_Null,
    
	FMF_Changeless,
	FMF_MiniLzo,
	FMF_ZLib,
	FMF_ZLibDeflated,
    
	FMF_Des,
	FMF_Ras,
	FMF_Md5,
	FMF_Sha1,
	FMF_Sha512,
    
	FMF_End
};

struct tagFilterParam
{
	EFM_FILTER	eFilter;			//
	bool		bCompress;			//
	LPVOID		lpMemIn;			//
	LPVOID		lpMemOut;			//
	DWORD		dwInBufferSize;		//
	DWORD		dwOutBufferSize;	//
	DWORD		dwInDataSize;		//
	DWORD		dwQueryPart;		//
    
	DWORD		dwOutDataSize;
	bool		bSuccess;
};

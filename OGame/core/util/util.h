//-----------------------------------------------------------------------------
// File: uUtil.h
// Desc: Game System uUtil 2.0
// Auth:
// Date: 2003/11/13
//-----------------------------------------------------------------------------
#pragma once

#include "../define/type_define.h"
#include "globe_table.h"

/*DWORD Crc32(LPCSTR lpData)
{
	LPBYTE pData = (LPBYTE)lpData;
	if( 0 == pData[0] ) 
		return (DWORD)GT_INVALID;

	UINT unResult = *pData++ << 24;
	if( *pData )
	{
		unResult |= *pData++ << 16;
		if( *pData )
		{
			unResult |= *pData++ << 8;
			if( *pData ) unResult |= *pData++;
		}
	}
	unResult = ~ unResult;
	while( *pData )
	{
		unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
		pData++;
	}
	return ~unResult;
}

DWORD Crc32(LPBYTE pData, INT nByteCount)
{
	if( nByteCount <= 0 )
		return (DWORD)GT_INVALID;

	UINT unResult = *pData++ << 24;
	if( --nByteCount > 0 )
	{
		unResult |= *pData++ << 16;
		if( --nByteCount > 0 )
		{
			unResult |= *pData++ << 8;
			if( --nByteCount > 0 ) unResult |= *pData++;
		}
	}
	unResult = ~ unResult;
	while( --nByteCount > 0 )
	{
		unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
		pData++;
	}

	return ~unResult;
}*/




#include "stdafx.h"
#include ".\npchelper.h"
#include <tchar.h>

NPCHelper* NPCHelper::_Instance = nullptr; 

BOOL NPCHelper::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	NPCData *pInfo = (NPCData*)pRawDataInfo;

	int m = 0, n = 0;
	std::string strList[8];
	std::string strLine;


    _tcsncpy(pInfo->szName, pInfo->szDataName, NPC_MAXSIZE_NAME);
	pInfo->szName[NPC_MAXSIZE_NAME - 1] = _TEXT('\0');

	_tcsncpy(pInfo->szArea, ParamList[m++].c_str(), NPC_MAXSIZE_NAME);
	pInfo->szMapName[NPC_MAXSIZE_NAME - 1] = _TEXT('\0');

	Util_ResolveTextLine( ParamList[m++].c_str(), strList, 8, ',' ); 
	pInfo->dwxPos0 = Str2Int(strList[0]);
	pInfo->dwyPos0 = Str2Int(strList[1]);

	_tcsncpy(pInfo->szMapName, ParamList[m++].c_str(),  NPC_MAXSIZE_NAME );
	pInfo->szMapName[NPC_MAXSIZE_NAME - 1] = _TEXT('\0');
	pInfo->NPCType = Str2Int(ParamList[m++]); // NPC Type

	return TRUE;
}


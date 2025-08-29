#include "stdafx.h"
#include "Mountinfo.h"
#include <tchar.h>

MountHelper* MountHelper::_Instance = nullptr;

BOOL MountHelper::_ReadRawDataInfo(CRawDataInfo* pRawDataInfo, vector<string>& ParamList)
{
	if (ParamList.size() == 0) return FALSE;

	MountInfo* pInfo = (MountInfo*)pRawDataInfo;

	int m = 0, n = 0;
	string strList[5];
	string strLine;

	_tcsncpy(pInfo->szName, pInfo->szDataName, Mount_MAXSIZE_NAME);
	pInfo->szName[Mount_MAXSIZE_NAME - 1] = _TEXT('\0');

    pInfo->CharID = Str2Int(ParamList[m++]);
    pInfo->ItemID = Str2Int(ParamList[m++]);

    pInfo->Scale = Str2Int(ParamList[m++]);

    for (int i = 0; i < MaxChatrcterx; i++)
    {
        Util_ResolveTextLine(ParamList[m++].c_str(), strList, 5, ',');
        pInfo->BoneID   [i] = Str2Int(strList[0]);
        pInfo->PoseID   [i] = Str2Int(strList[1]);
        pInfo->OffsetX  [i] = Str2Int(strList[2]);
        pInfo->OffsetY  [i] = Str2Int(strList[3]);
        pInfo->Height   [i] = Str2Int(strList[4]);        
    }

	return TRUE;
}

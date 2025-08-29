#pragma once
#include "util.h"
#include "TableData.h"

#define Mount_MAXSIZE_NAME			32 // mount name size
#define MaxChatrcterx	4
class MountInfo : public CRawDataInfo
{
public:
	char szName[Mount_MAXSIZE_NAME];
    int CharID;
    int ItemID;
    int Scale; // percent
	int BoneID  [MaxChatrcterx];
    int PoseID  [MaxChatrcterx];
    int OffsetX [MaxChatrcterx];
    int OffsetY [MaxChatrcterx];
	int Height  [MaxChatrcterx];
};


class MountHelper : public CRawDataSet
{
public:

	static MountHelper* I() { return _Instance; }

	MountHelper(int nIDStart, int nIDCnt, int nCol = 128)
		: CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static MountHelper* _Instance; // �൱�ڵ���, ���Լ���ס


	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new MountInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[](MountInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(MountInfo);
	}

	virtual void* _CreateNewRawData(CRawDataInfo* pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo* pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo* pRawDataInfo, vector<string>& ParamList);
};

inline MountInfo* GetMountInfoFromMountID(int ID)
{
    return (MountInfo*)MountHelper::I()->GetRawDataInfo(ID);
}

inline MountInfo* GetMountInfoFromItemID(int ItemID)
{
    MountHelper* helper = MountHelper::I();
    for (int i = 0; i < helper->GetIDCnt(); i++)
        if(MountInfo* pInfo = (MountInfo*)helper->GetRawDataInfo(helper->GetIDStart() + i))
            if (pInfo->ItemID == ItemID)
                return pInfo;
    return nullptr;
}

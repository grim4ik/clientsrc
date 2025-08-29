#pragma once

class CStallData
{
public:
	enum eType{selling,buying};
	enum eState{normal=0,money=1,member=2,repute=4};
	struct sStallData
	{
		DWORD	stall_id;
		string	stall_name;
		string	stall_motto_name;
		CStallData::eType	stall_type;
		string	stall_master_name;
		__int64	stall_money;
		__int64	stall_experience;
		DWORD	stall_member_count;
		DWORD	stall_member_max;
		eState	stall_state;
		DWORD	stall_remain_time;
	};
	CStallData(void);
	~CStallData(void);

	static void LoadData(const sStallData data);
	static void	SetStallID(const DWORD dwID)	{ m_dwID=dwID; }
	static DWORD	GetStallID()	{ return m_dwID; }
	static void	SetStallName(const string strName)	{ m_strName=strName; }
	static string	GetStallName()	{ return m_strName; }
	static void	SetStallMottoName(const string strName)	{ m_strMottoName=strName; }
	static string	GetStallMottoName()	{ return m_strMottoName; }
	static void	SetStallType(const eType type)	{ m_eType=type; }
	static eType	GetStallType()	{ return m_eType; }
	static void	SetStallState(const eState state)	{ m_eState=state; }
	static eState	GetStallState()	{ return m_eState; }
	static void	SetStallMasterName(const string strMasterName)	{ m_strMasterName=strMasterName; }
	static string	GetStallMasterName()	{ return m_strMasterName; }
	static void	SetExperence(const __int64 i64Exp)	{ m_i64Exp=i64Exp; }
	static __int64	GetExperence()	{ return m_i64Exp; }
	static void	SetMemberCount(const DWORD dwCount)	{ m_dwMemberCount=dwCount; }
	static DWORD	GetMemberCount()	{ return m_dwMemberCount; }
	static void	SetMaxMembers(const DWORD dwCount)	{ m_dwMaxMembers=dwCount; }
	static DWORD	GetMaxMembers()	{ return m_dwMaxMembers; }
	static void	SetMoney(const __int64 i64Money)	{ m_i64Money=i64Money; }
	static __int64	GetMoney()	{ return m_i64Money; }
	static void	SetRemainTime(const __int64 i64RemainTime)	{ m_i64RemainTime=i64RemainTime; }
	static __int64	GetRemainTime()	{ return m_i64RemainTime; }
	static void	Reset();

private:
	static DWORD	m_dwID;
	static string	m_strName;
	static string	m_strMottoName;
	static eType	m_eType;
	static eState	m_eState;
	static DWORD	m_dwLv;
	static DWORD	m_dwMasterID;
	static string	m_strMasterName;
	static __int64	m_i64Exp;
	static DWORD	m_dwMemberCount;
	static DWORD	m_dwMaxMembers;
	static __int64	m_i64Money;
	static __int64	m_i64RemainTime;
};

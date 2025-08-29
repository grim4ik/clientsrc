//------------------------------------------------------------------------
//	2005.5.8	Arcol	create this file
//------------------------------------------------------------------------

#include "stdafx.h"
#include "RecruitMemberData.h"
#include ".\recruitmembersmgr.h"

std::vector<CRecruitMemberData*> CRecruitMembersMgr::m_pRecruitMembers;

CRecruitMembersMgr::CRecruitMembersMgr(void)
{
}

CRecruitMembersMgr::~CRecruitMembersMgr(void)
{
}

void CRecruitMembersMgr::AddRecruitMember(CRecruitMemberData* pRecruitMember)
{
	m_pRecruitMembers.push_back(pRecruitMember);
}

bool CRecruitMembersMgr::DelRecruitMember(CRecruitMemberData* pRecruitMember)
{
	std::vector <CRecruitMemberData*>::iterator Iter;
	for (Iter=m_pRecruitMembers.begin();Iter!=m_pRecruitMembers.end();Iter++)
	{
		if ((*Iter)==pRecruitMember)
		{
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI��������
			m_pRecruitMembers.erase(Iter);
			return true;
		}
	}
	return false;
}

bool CRecruitMembersMgr::DelRecruitMemberByID(DWORD dwID)
{
	std::vector <CRecruitMemberData*>::iterator Iter;
	for (Iter=m_pRecruitMembers.begin();Iter!=m_pRecruitMembers.end();Iter++)
	{
		if ((*Iter)->GetID()==dwID)
		{
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI��������
			m_pRecruitMembers.erase(Iter);
			return true;
		}
	}
	return false;
}

bool CRecruitMembersMgr::DelRecruitMemberByName(std::string strName)
{
	std::vector <CRecruitMemberData*>::iterator Iter;
	for (Iter=m_pRecruitMembers.begin();Iter!=m_pRecruitMembers.end();Iter++)
	{
		if ((*Iter)->GetName()==strName)
		{
			//delete (*Iter);
			SAFE_DELETE(*Iter); // UI��������
			m_pRecruitMembers.erase(Iter);
			return true;
		}
	}
	return false;
}

CRecruitMemberData* CRecruitMembersMgr::FindRecruitMemberByID(DWORD dwID)
{
	std::vector <CRecruitMemberData*>::iterator Iter;
	for (Iter=m_pRecruitMembers.begin();Iter!=m_pRecruitMembers.end();Iter++)
	{
		if ((*Iter)->GetID()==dwID)
		{
			return (*Iter);
		}
	}
	return nullptr;
}

CRecruitMemberData* CRecruitMembersMgr::FindRecruitMemberByName(std::string strName)
{
	std::vector <CRecruitMemberData*>::iterator Iter;
	for (Iter=m_pRecruitMembers.begin();Iter!=m_pRecruitMembers.end();Iter++)
	{
		if ((*Iter)->GetName()==strName)
		{
			return (*Iter);
		}
	}
	return nullptr;
}

CRecruitMemberData* CRecruitMembersMgr::FindRecruitMemberByIndex(DWORD dwIndex)
{
	if (dwIndex>=GetTotalRecruitMembers()) return nullptr;
	return m_pRecruitMembers[dwIndex];
}

DWORD CRecruitMembersMgr::GetTotalRecruitMembers()
{
	return static_cast<DWORD>(m_pRecruitMembers.size());
}

void CRecruitMembersMgr::ResetAll()
{
	std::vector <CRecruitMemberData*>::iterator Iter;
	while (m_pRecruitMembers.size()>0)
	{
		Iter=m_pRecruitMembers.begin();
		CRecruitMemberData* pNode=*Iter;
		//delete pNode;
		SAFE_DELETE(pNode); // UI��������
		m_pRecruitMembers.erase(Iter);
	}
}

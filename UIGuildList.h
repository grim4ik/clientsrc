#pragma once
#include "uiglobalvar.h"

namespace GUI
{
class CUIGuildList :
	public CUIInterface
{
public:
	CUIGuildList(void);
	~CUIGuildList(void);
	static void ShowGuildList();
	static void OnClickSortByGuildName(CGuiData *pSender, int x, int y, DWORD key);
	static void OnClickSortByGuildMemberCount(CGuiData *pSender, int x, int y, DWORD key);
	static void OnClickSortByGuildExperience(CGuiData *pSender, int x, int y, DWORD key);
	static void OnSelectChange(CGuiData *pSender);
	static void OnClickApply(CGuiData *pSender, int x, int y, DWORD key);
	static void OnMsgReplaceApply(std::string strOldGuildName);
	static void OnMsgReplaceApplySelectClick(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void ResetGuildList();
	static void ResetOrder();

protected:
	virtual bool Init();
	//virtual bool InitRank();

private:
	static CForm*	m_pGuildListForm;
	//static CForm*	m_pRankListForm;
	static CListView*	m_pGuildList;
	//static CListView*	m_pRankList;
	static CTextButton*	m_pGuildApplyBtn;
	static CLabelEx*	m_pGuildNavyLab;
	static CImage*	m_pGuildNavyImg;
	static CImage*	m_pRankFirstImg;
	//static CLabelEx*	m_pRankLab;
	//static CImage*	m_pGuildNavyImg1;
	//static CImage*	m_pGuildNavyImg2;	// �޸ĳ� Label ��
	static CImage*	m_pGuildNavyImg3;
	//static CImage*	m_pRankImg1;
	//static CImage*	m_pRankImg2;
	//static CImage*	m_pRankImg3;

	static CImage*	m_pGuildNavyImg4;
	static CImage*	m_pGuildPirateImg;
	//static CImage*	m_pGuildPirateImg1;
	//static CImage*	m_pGuildPirateImg2;	// �޸ĳ� Label ��
	static CImage*	m_pGuildPirateImg3;
	static CImage*	m_pGuildPirateImg4;

	static CLabelEx*	m_lblGuildNavyTitle;
	static CLabelEx*	m_lblGuildPirateTitle;
	//static CLabelEx*	m_lblGuildRankTitle;

	static bool	m_bSortIncByName;
	static bool	m_bSortIncByMemberCount;
	static bool	m_bSortIncByExperience;

};
}
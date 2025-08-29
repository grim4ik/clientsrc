//------------------------------------------------------------------------
//	2005.4.25	Arcol	create this file
//------------------------------------------------------------------------

#include "stdafx.h"
#include "netguild.h"
#include "UIFormMgr.h"
#include "UITextButton.h"
#include "UIBoxForm.h"
#include "UIListView.h"
#include "GuildListData.h"
#include "GuildListMgr.h"
#include "UILabel.h"
#include ".\uiguildlist.h"


CForm* CUIGuildList::m_pGuildListForm=nullptr;
//CForm* CUIGuildList::m_pRankListForm=nullptr; //rank
CListView* CUIGuildList::m_pGuildList=nullptr;
//CListView*	CUIGuildList::m_pRankList=nullptr; //rank
CTextButton* CUIGuildList::m_pGuildApplyBtn=nullptr;
CLabelEx* CUIGuildList::m_pGuildNavyLab=nullptr;
//CLabelEx* CUIGuildList::m_pRankLab=nullptr; //rank

CImage*	CUIGuildList::m_pGuildNavyImg=nullptr;
//CImage*	CUIGuildList::m_pRankFirstImg=nullptr; //rank
//CImage*	CUIGuildList::m_pRankImg1=nullptr; //rank
//CImage*	CUIGuildList::m_pRankImg2=nullptr; //rank
//CImage*	CUIGuildList::m_pRankImg3=nullptr; //rank
CImage*	CUIGuildList::m_pGuildNavyImg3=nullptr;
CImage*	CUIGuildList::m_pGuildNavyImg4=nullptr;
CImage*	CUIGuildList::m_pGuildPirateImg=nullptr;
CImage*	CUIGuildList::m_pGuildPirateImg3=nullptr;
CImage*	CUIGuildList::m_pGuildPirateImg4=nullptr;
CLabelEx* CUIGuildList::m_lblGuildNavyTitle   = nullptr;
CLabelEx* CUIGuildList::m_lblGuildPirateTitle = nullptr;
//CLabelEx* CUIGuildList::m_lblGuildRankTitle = nullptr;


bool CUIGuildList::m_bSortIncByName=true;
bool CUIGuildList::m_bSortIncByMemberCount=true;
bool CUIGuildList::m_bSortIncByExperience=true;

CUIGuildList::CUIGuildList(void)
{
}

CUIGuildList::~CUIGuildList(void)
{
}

bool CUIGuildList::Init()
{
	FORM_LOADING_CHECK(m_pGuildListForm,"npc.clu","frmConAsk");

	FORM_CONTROL_LOADING_CHECK(m_pGuildList,m_pGuildListForm,CListView,"npc.clu","lstAsk");
	m_pGuildList->GetList()->evtSelectChange=OnSelectChange;
	CTextButton *btn;
	FORM_CONTROL_LOADING_CHECK(btn,m_pGuildListForm,CTextButton,"npc.clu","btnName");
	btn->evtMouseClick=OnClickSortByGuildName;
	FORM_CONTROL_LOADING_CHECK(btn,m_pGuildListForm,CTextButton,"npc.clu","btnNum");
	btn->evtMouseClick=OnClickSortByGuildMemberCount;
	FORM_CONTROL_LOADING_CHECK(btn,m_pGuildListForm,CTextButton,"npc.clu","btnExp");
	btn->evtMouseClick=OnClickSortByGuildExperience;
	FORM_CONTROL_LOADING_CHECK(m_pGuildApplyBtn,m_pGuildListForm,CTextButton,"npc.clu","btnApply");
	m_pGuildApplyBtn->SetIsEnabled(false);
	m_pGuildApplyBtn->evtMouseClick=OnClickApply;
	
	FORM_CONTROL_LOADING_CHECK(m_pGuildNavyLab,m_pGuildListForm,CLabelEx,"npc.clu","labInfoJ");
	FORM_CONTROL_LOADING_CHECK(m_pGuildNavyImg,m_pGuildListForm,CImage,"npc.clu","imgName1");
	//FORM_CONTROL_LOADING_CHECK(m_pGuildNavyImg1,m_pGuildListForm,CImage,"npc.clu","imgbackJ");
	//FORM_CONTROL_LOADING_CHECK(m_pGuildNavyImg2,m_pGuildListForm,CImage,"npc.clu","imgTitleJ");	// �޸ĳ� Label ��
	FORM_CONTROL_LOADING_CHECK(m_pGuildNavyImg3,m_pGuildListForm,CImage,"npc.clu","imgBakAskH");
	FORM_CONTROL_LOADING_CHECK(m_pGuildNavyImg4,m_pGuildListForm,CImage,"npc.clu","imgBakAskH1");
	FORM_CONTROL_LOADING_CHECK(m_pGuildPirateImg,m_pGuildListForm,CImage,"npc.clu","imgName2");
	//FORM_CONTROL_LOADING_CHECK(m_pGuildPirateImg1,m_pGuildListForm,CImage,"npc.clu","imgbackD");
	//FORM_CONTROL_LOADING_CHECK(m_pGuildPirateImg2,m_pGuildListForm,CImage,"npc.clu","imgTitleD");	// �޸ĳ� Label ��
	FORM_CONTROL_LOADING_CHECK(m_pGuildPirateImg3,m_pGuildListForm,CImage,"npc.clu","imgBakAsk");
	FORM_CONTROL_LOADING_CHECK(m_pGuildPirateImg4,m_pGuildListForm,CImage,"npc.clu","imgBakAsk1");

	FORM_CONTROL_LOADING_CHECK(m_lblGuildNavyTitle,   m_pGuildListForm, CLabelEx, "npc.clu", "labTitleJ");
	FORM_CONTROL_LOADING_CHECK(m_lblGuildPirateTitle, m_pGuildListForm, CLabelEx, "npc.clu", "labTitleD");

	//InitRank();


	return true;
}

/*bool CUIGuildList::InitRank()
{

	FORM_LOADING_CHECK(m_pRankListForm,"ranksystem.clu","frmRankAsk");

	FORM_CONTROL_LOADING_CHECK(m_pRankList,m_pRankListForm,CListView,"ranksystem.clu","PstAsk");
	m_pRankList->GetList()->evtSelectChange=OnSelectChange;
	CTextButton *btn;
	FORM_CONTROL_LOADING_CHECK(btn,m_pRankListForm,CTextButton,"ranksystem.clu","btnNameR");
	btn->evtMouseClick=OnClickSortByGuildName;
	FORM_CONTROL_LOADING_CHECK(btn,m_pRankListForm,CTextButton,"ranksystem.clu","btnNumR");
	btn->evtMouseClick=OnClickSortByGuildMemberCount;
	FORM_CONTROL_LOADING_CHECK(btn,m_pRankListForm,CTextButton,"ranksystem.clu","btnExpR");
	btn->evtMouseClick=OnClickSortByGuildExperience;
	
	FORM_CONTROL_LOADING_CHECK(m_pRankLab,m_pRankListForm,CLabelEx,"ranksystem.clu","labInfoK");
	FORM_CONTROL_LOADING_CHECK(m_pRankImg1,m_pRankListForm,CImage,"ranksystem.clu","imgName3");
	FORM_CONTROL_LOADING_CHECK(m_pRankImg2,m_pRankListForm,CImage,"ranksystem.clu","imgBakAskH2");
	FORM_CONTROL_LOADING_CHECK(m_pRankImg3,m_pRankListForm,CImage,"ranksystem.clu","imgBakAskH3");

	FORM_CONTROL_LOADING_CHECK(m_lblGuildRankTitle, m_pRankListForm, CLabelEx, "ranksystem.clu", "labTitleP");

	return true;
}*/

void CUIGuildList::ShowGuildList()
{
	char buf[120];
	ResetGuildList();
	m_pGuildList->GetList()->GetItems()->Clear();
	for (DWORD i=0;i<CGuildListMgr::GetTotalGuilds();i++)
	{
		CGuildListData* pListData=CGuildListMgr::FindGuildByIndex(i);
		if (!pListData) continue;
		CItemRow* pRow=m_pGuildList->GetList()->NewItem(); //m_pRankList		

		CItem*	pGuildNameItem=new CItem(pListData->GetGuildName().c_str(),COLOR_BLACK);
		CItem*	pGuildMottoNameItem=new CItem(pListData->GetGuildMottoName().c_str(),COLOR_BLACK);
		CItem*	pGuildMemberCountItem=new CItem(itoa(pListData->GetMembers(),buf,10),COLOR_BLACK);
		CItem*	pGuildExperienceItem=new CItem(_i64toa(pListData->GetExperence(),buf,10),COLOR_BLACK);
		CItem*	pGuildMasterItem=new CItem(pListData->GetGuildMasterName().c_str(),COLOR_BLACK);
		pRow->SetPointer(pListData);
		pRow->SetIndex(0,pGuildNameItem);
		pRow->SetIndex(1,pGuildMottoNameItem);
		pRow->SetIndex(2,pGuildMemberCountItem);
		pRow->SetIndex(3,pGuildExperienceItem);
		pRow->SetIndex(4,pGuildMasterItem);
	}

	/*m_pRankList->GetList()->GetItems()->Clear();
	for (DWORD i=0;i<CGuildListMgr::GetTotalGuilds();i++)
	{
		CGuildListData* pListData=CGuildListMgr::FindGuildByIndex(i);
		if (!pListData) continue;
		CItemRow* pRow=m_pRankList->GetList()->NewItem(); //m_pRankList		

		CItem*	pGuildNameItem=new CItem(pListData->GetGuildName().c_str(),COLOR_BLACK);
		CItem*	pGuildMottoNameItem=new CItem(pListData->GetGuildMottoName().c_str(),COLOR_BLACK);
		CItem*	pGuildMemberCountItem=new CItem(itoa(pListData->GetMembers(),buf,10),COLOR_BLACK);
		CItem*	pGuildExperienceItem=new CItem(_i64toa(pListData->GetExperence(),buf,10),COLOR_BLACK);
		CItem*	pGuildMasterItem=new CItem(pListData->GetGuildMasterName().c_str(),COLOR_BLACK);
		pRow->SetPointer(pListData);
		pRow->SetIndex(0,pGuildNameItem);
		pRow->SetIndex(1,pGuildMottoNameItem);
		pRow->SetIndex(2,pGuildMemberCountItem);
		pRow->SetIndex(3,pGuildExperienceItem);
		pRow->SetIndex(4,pGuildMasterItem);

	}*/

	if (CGuildListMgr::GetType()==CGuildData::navy)
	{
		m_pGuildPirateImg->SetIsShow(false);
		//m_pGuildPirateImg1->SetIsShow(false);
		//m_pGuildPirateImg2->SetIsShow(false);	// �޸ĳ� Label ��
		m_pGuildPirateImg3->SetIsShow(false);
		m_pGuildPirateImg4->SetIsShow(false);
		m_pGuildNavyLab->SetIsShow(true);
		m_pGuildNavyImg->SetIsShow(true);
		//m_pGuildNavyImg1->SetIsShow(true);
		//m_pGuildNavyImg2->SetIsShow(true);	// �޸ĳ� Label ��
		m_pGuildNavyImg3->SetIsShow(true);
		m_pGuildNavyImg4->SetIsShow(true);
		//m_pRankLab->SetIsShow(false);
		//m_pRankImg1->SetIsShow(false);
		////m_pRankImg2->SetIsShow(false);
		//m_pRankImg3->SetIsShow(false);

		m_lblGuildNavyTitle->SetIsShow(true);
		m_lblGuildPirateTitle->SetIsShow(false);
		//m_lblGuildRankTitle->SetIsShow(false);

		const char* strNavy   = m_lblGuildNavyTitle->GetCaption();
		const char* strPriate = m_lblGuildPirateTitle->GetCaption();
		//const char* strRank = m_lblGuildRankTitle->GetCaption();

		int jampe = 0;;

		m_pGuildListForm->Refresh();
		m_pGuildListForm->Show();
	}

	else if(CGuildListMgr::GetType()==CGuildData::pirate)
	{
		m_pGuildPirateImg->SetIsShow(true);
		m_pGuildPirateImg3->SetIsShow(true);
		m_pGuildPirateImg4->SetIsShow(true);
		m_pGuildNavyLab->SetIsShow(false);
		m_pGuildNavyImg->SetIsShow(false);
		m_pGuildNavyImg3->SetIsShow(false);
		m_pGuildNavyImg4->SetIsShow(false);
		//m_pRankLab->SetIsShow(false);
		//m_pRankImg1->SetIsShow(false);
		//m_pRankImg2->SetIsShow(false);
		//m_pRankImg3->SetIsShow(false);

		m_lblGuildNavyTitle->SetIsShow(false);
		m_lblGuildPirateTitle->SetIsShow(true);
		//m_lblGuildRankTitle->SetIsShow(false);

		m_pGuildListForm->Refresh();
		m_pGuildListForm->Show();
	}
	/*else if (CGuildListMgr::GetType() == CGuildData::ranktype)
	{
		m_pGuildPirateImg->SetIsShow(false);
		m_pGuildPirateImg3->SetIsShow(false);
		m_pGuildPirateImg4->SetIsShow(false);
		m_pGuildNavyLab->SetIsShow(false);
		m_pGuildNavyImg->SetIsShow(false);
		m_pGuildNavyImg3->SetIsShow(false);
		m_pGuildNavyImg4->SetIsShow(false);
		m_pRankLab->SetIsShow(true);
		m_pRankImg1->SetIsShow(true);
		m_pRankImg2->SetIsShow(true);
		m_pRankImg3->SetIsShow(true);

		m_lblGuildNavyTitle->SetIsShow(false);
		m_lblGuildPirateTitle->SetIsShow(false);
		m_lblGuildRankTitle->SetIsShow(true);
	//	m_pGuildListForm->Show();
		m_pRankListForm->Refresh();
		m_pRankListForm->Show();

	}*/
}

void CUIGuildList::OnClickSortByGuildName(CGuiData *pSender, int x, int y, DWORD key)
{
	m_bSortIncByName=!m_bSortIncByName;
	CGuildListMgr::SortGuildsByName(m_bSortIncByName);
	ShowGuildList();
}

void CUIGuildList::OnClickSortByGuildMemberCount(CGuiData *pSender, int x, int y, DWORD key)
{
	m_bSortIncByMemberCount=!m_bSortIncByMemberCount;
	CGuildListMgr::SortGuildsByMemberCount(m_bSortIncByMemberCount);
	ShowGuildList();
}

void CUIGuildList::OnClickSortByGuildExperience(CGuiData *pSender, int x, int y, DWORD key)
{
	m_bSortIncByExperience=!m_bSortIncByExperience;
	CGuildListMgr::SortGuildsByExperience(m_bSortIncByExperience);
	ShowGuildList();
}

void CUIGuildList::ResetGuildList()
{
	m_pGuildApplyBtn->SetIsEnabled(false);
	m_pGuildList->GetList()->GetItems()->Clear(); //m_pGuildList
	//m_pRankList->GetList()->GetItems()->Clear();
//	m_pRankList->GetList()->GetItems()->Clear();
}

void CUIGuildList::ResetOrder()
{
	m_bSortIncByName=true;
	m_bSortIncByMemberCount=true;
	m_bSortIncByExperience=true;
}

void CUIGuildList::OnSelectChange(CGuiData *pSender)
{
	CList* pList=dynamic_cast<CList*>(pSender);
	if (!pList) return;
	CItemRow* pItemRow=pList->GetSelectItem();
	if (!pItemRow) return;
	m_pGuildApplyBtn->SetIsEnabled(true);

}

void CUIGuildList::OnClickApply(CGuiData *pSender, int x, int y, DWORD key)
{
	CItemRow* pItemRow=m_pGuildList->GetList()->GetSelectItem(); //m_pGuildList  m_pRankList
	//pItemRow = m_pGuildList->GetList()->GetSelectItem();
	if (!pItemRow) return;
	CGuildListData* pListData=static_cast<CGuildListData*>(pItemRow->GetPointer());
	if (!pListData) return;
    CM_GUILD_TRYFOR(pListData->GetGuildID());
}

void CUIGuildList::OnMsgReplaceApply(std::string strOldGuildName)
{
	std::string str=g_oLangRec.GetString(593)+strOldGuildName;
	CBoxMgr::ShowSelectBox( OnMsgReplaceApplySelectClick, str.c_str(), true );
}

void CUIGuildList::OnMsgReplaceApplySelectClick(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	CTextButton* pButton = dynamic_cast<CTextButton*>(pSender);
	if( pButton->GetFormModal()==CForm::mrYes )
	{
		CM_GUILD_TRYFORCFM(true);
	}
	else
	{
		CM_GUILD_TRYFORCFM(false);
	}
}

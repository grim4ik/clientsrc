#pragma once
#include "UIGlobalVar.h"


//2006-1-8 By Arcol:�µĿ��������Ϣ���CCharMsg��ɣ������δʹ��Ҳδ�����ԣ���ԭ�ṹ�Ƚϻ��ң�����ʹ�ô������
class CCharMsg
{
public:
	enum eChannel
	{
		CHANNEL_NONE			= 0,
		CHANNEL_ALL				= 1,
		CHANNEL_SIGHT			= 2,
		CHANNEL_PRIVATE			= 4,
		CHANNEL_WORLD			= 8,
		CHANNEL_TRADE			= 16,
		CHANNEL_TEAM			= 32,
		CHANNEL_GUILD			= 64,
		CHANNEL_SYSTEM			= 128,
		CHANNEL_PUBLISH			= 256,
		CHANNEL_SIDE			= 512,	// ��Ӫ
		//CHANNEL_VIP				= 1024,	
		//CHANNEL_DISCORD			= 2048,	
	};

	struct sTextInfo
	{
		eChannel eTextChannel;
		std::string	 strWho;
		std::string	 strText;
		std::string	 strShowText;
		bool	 bSendTo;
		DWORD	dwColour;
	};

private:
	typedef list<sTextInfo> lstTextInfoType;
	struct sChannelInfo
	{
		eChannel					enumChannel;
		std::string						strChannelName;
		DWORD						dwChannelColor;
		std::string						strChannelCommand;
		DWORD						dwTotalMsg;
		lstTextInfoType::iterator	itFirstMsgPos;
	};


public:
	CCharMsg();
	~CCharMsg();

	static void		AddMsg(eChannel channel, std::string strWho, std::string strText, bool bSendTo = false,DWORD dwColour = 0xFFFFFFFF);
	static void		SetChannelColor(eChannel channel, DWORD dwColor);
	static DWORD	GetChannelColor(eChannel channel);
	static std::string	GetChannelCommand(WORD wChannelIndex);
	static std::string	GetChannelName(eChannel channel);
	static std::string	GetChannelShowName(eChannel channel);
	static WORD		GetChannelIndex(eChannel channel);
	static eChannel	GetChannelByIndex(WORD wChannelIndex);
	static WORD		GetTotalChannelCount();
	static void		ClearAllMsg();

	void			SetShowChannels(DWORD ecboShowChannels);
	bool			ModifyShowChannel(eChannel eShowChannel, bool bAddOrRemove = true, bool bShowTips = true);
	bool			IsShowChannel(eChannel channel);
	DWORD			GetShowChannels();

	bool			MoveToFirstMsg();
	bool			MoveToNextMsg();
	sTextInfo		GetMsgInfo();


private:


private:

	static const WORD			m_wTotalChannelsCount = 13;	// ������Ӫ�޸� 10 -> 11
	static const DWORD			m_dwChannelBufferSize = 100;
	static sChannelInfo			m_sChannelInfo[m_wTotalChannelsCount];
	static lstTextInfoType		m_lstMsgLink;
	typedef list<CCharMsg*>		lstInstanceType;
	static lstInstanceType		m_lstThisInstanceLink;
	
	bool						m_bCurMsgAvailable;
	DWORD						m_ecboShowChannels;
	lstTextInfoType::iterator	m_itCurrentMsgPos;
};

class CCardCase
{
public:
	CCardCase(WORD wMaxLimit=10);
	~CCardCase();
	void	AddCard(std::string str);
	bool	RemoveCard(std::string str);
	void	ClearAll();
	int		GetTotalCount();
	bool	MoveToFirstCard();
	bool	MoveToNextCard();
	bool	MoveToLastCard();
	bool	MoveToPrevCard();
	std::string	GetCardInfo();

private:
	typedef std::list<std::string>	lstCardType;
	lstCardType				m_lstCardData;
	lstCardType::iterator	m_itCurrentCardPos;
	int						m_wMaxLimit;
};

namespace GUI
{

class CChannelSwitchForm : public CUIInterface
{
public:

	CChannelSwitchForm();
	static CChannelSwitchForm* GetInstance();
	~CChannelSwitchForm();
	void SwitchCheck();

protected:
	
	bool Init();
	static void EventLostFocus(CGuiData *pSender);
	static void EventPrivateCheckChange(CGuiData *pSender);
	static void EventSightCheckChange(CGuiData *pSender);
	static void EventSystemCheckChange(CGuiData *pSender);
	static void EventTeamCheckChange(CGuiData *pSender);
	static void EventGuildCheckChange(CGuiData *pSender);
	static void EventWorldCheckChange(CGuiData *pSender);
	static void EventTradeCheckChange(CGuiData *pSender);
	//static void EventDiscordCheckChange(CGuiData *pSender);
	//static void EventVIPCheckChange(CGuiData *pSender);

private:
	CForm		*m_frmChannelSwitch;
	CCheckBox	*m_chkPrivate;
	CCheckBox	*m_chkSight;
	CCheckBox	*m_chkSystem;
	CCheckBox	*m_chkTeam;
	CCheckBox	*m_chkGuild;
	CCheckBox	*m_chkWorld;
	CCheckBox	*m_chkTrade;
	//CCheckBox	*m_chkVIP;
	//CCheckBox	*m_chkDiscord;
};



class CCozeForm : public CUIInterface
{
	friend CChannelSwitchForm;
public:
	CCozeForm();
	~CCozeForm();
	static CCozeForm* GetInstance();

	//void OnVIPMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	//void OnDiscordMsg(string strName, string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnSightMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnSightMsg(CCharacter* pChar, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnPrivateMsg(std::string strFromName, std::string strToName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnWorldMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTradeMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTeamMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnTeamMsg(DWORD dwCharID, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnGuildMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnSystemMsg(std::string strMsg);
	void OnSideMsg(std::string strName, std::string strMsg,DWORD dwColour=0xFFFFFFFF);
	void OnPublishMsg(std::string strName, std::string strMsg);
	void OnPublishMsg1(std::string strMsg,int setnum);//Add by sunny.sun20080804
	void OnPrivateNameSet(std::string strName);
	void OnResetAll();
	bool IsMouseOnList(int x, int y);
	void AddToEdit(std::string strData);

protected:
	bool Init();

	void SendMsg();
	void UpdatePages();
	void ResetPages();
	void ChangePrivatePlayerName(std::string strName);

	CCharMsg::eChannel	GetCmdFromMsg(std::string strMsg);

protected:
	static void EventPublishShowForm(CForm* pForm, bool& IsShow);
	static void EventPublishSendMsg(CGuiData *pSender);
	static bool EventGlobalKeyDownHandle(int& key);
	static void EventSendMsg(CGuiData *pSender);
	static bool EventEditMsg(CGuiData *pSender, int& key);
	static void EventMainListKeyDown(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSendChannelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSendChannelChange(CGuiData *pSender);
	static void EventMainPageDragBegin(CGuiData *pSender, int x, int y, DWORD key);
	static void EventMainPageDragging(CGuiData *pSender, int x, int y, DWORD key);
	static void EventMainPageDragEnd(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragBegin(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragging(CGuiData *pSender, int x, int y, DWORD key);
	static void EventSystemPageDragEnd(CGuiData *pSender, int x, int y, DWORD key);
	
	//static void EventDiscordListKeyDown(CGuiData *pSender, int x, int y, DWORD key);

	//static void EventDiscordPageDragBegin(CGuiData *pSender, int x, int y, DWORD key);
	//static void EventDiscordPageDragging(CGuiData *pSender, int x, int y, DWORD key);
	//static void EventDiscordPageDragEnd(CGuiData *pSender, int x, int y, DWORD key);
	//
	static void EventChannelSwitchCheck(CGuiData *pSender);
	static void EventCallingCardSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	//static void EventCallingCardLostFocus(CGuiData *pSender);
	static void EventCardSelected(CGuiData *pSender);
	static void EventFacePanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	//static void EventFacePanelLostFocus(CGuiData *pSender);
	static void EventFaceSelected(CGuiData *pSender);
	static void EventBrowPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventBrowSelected(CGuiData *pSender);
	static void EventActionPanelSwitchClick(CGuiData *pSender, int x, int y, DWORD key);
	static void EventActionSelected(CGuiData *pSender);
    void ReplaceSpecialFace( std::string &strMsg ,const std::string & strReplace ,const std::string & strFace );//Add by sunny.sun 20080902

private:
	CForm			*m_frmMainChat;
	CForm			*m_frmPublish;

	CCombo			*m_cmbChannel;
	CEdit			*m_edtPublishMsg;
	CEdit			*m_edtMsg;

	CList			*m_lstMainPage;
	CList			*m_lstSystemPage;
//	CList			*m_lstDiscordPage;
	CDragTitle		*m_drgMainPage;
	CDragTitle 		*m_drgSystemPage;
//	CDragTitle 		*m_drgDiscordPage;
	CCheckBox		*m_chkChannelSwitch;
	CTextButton		*m_btnCallingCardSwitch;
	CList			*m_lstCallingCard;
	CTextButton		*m_btnFaceSwitch;
	CGrid			*m_grdFacePanel;
	CTextButton		*m_btnBrowSwitch;
	CGrid			*m_grdBrowPanel;
	CTextButton		*m_btnActionSwitch;
	CGrid			*m_grdActionPanel;

	CCharMsg::eChannel	m_eCurSelChannel;
	CCharMsg			m_cMainMsg;
	CCharMsg			m_cSystemMsg;
	//CCharMsg			m_cDiscordMsg;
	CCardCase			m_cCallingCard;
	CCardCase			m_cSendMsgCard;

	bool				m_bSendMsgCardSwitch;
	int					m_nHeightBeforeDrag;
	int					m_nTopBeforeDrag;
	static const int	m_nMainPageMinHeight;
	static const int	m_nMainPageMaxHeight;
	static const int	m_nSystemPageMinHeight;
	static const int	m_nSystemPageMaxHeight;
	DWORD	EmojiesTicks = 0;
};





















#if 0



// ����Ϸ�е�����,�����½���Ϸ�е�����
class CCozeMgr : public CUIInterface
{
	enum 
	{
		MAX_PAGE_SAY	= 10,		// ��up,down����ܷ���������
	};

	static const int DATA_RECORDS_SIZE = 200;
	static const int CHANNELS_COUNT = 9;	// ��ӪƵ������, 8 -> 9

public:
	struct stChatColor
	{
		stChatColor() : road(0xFFFFFFFF), person(0xFFFFFFFF), team(0xFFFFFFFF), guild(0xFFFFFFFF), world(0xFFFFFFFF), system(0xFFFFFFFF), trade(0xFFFFFFFF), gm(0xFFFFFFFF, side(0xFFFFFFFF)) {}

		DWORD	road;		//a��	����
		DWORD	person;		//b��	˽��
		DWORD	team;		//c��	����
		DWORD	guild;		//d��	����
		DWORD   world;		//e��	�㲥
		DWORD   system;		//f��	ϵͳ
		DWORD   trade;		//g��	����
		DWORD	gm;			//h��	GM
		DWORD	side;
	};

	enum eChannel
	{
		CHANNEL_ROAD,
		CHANNEL_PRIVATE,
		CHANNEL_TEAM,
		CHANNEL_GUILD,
		CHANNEL_WORLD,
		CHANNEL_SYSTEM,
		CHANNEL_TRADE,
		CHANNEL_SIDE		// ��ӪƵ������
	};

	struct sTextInfo
	{
		eChannel eTextChannel;
		string stName;
		string stText;
		DWORD dwColor;
	};


	static bool		IsMouseOnSayList( int x, int y );

	static void		OnGmSay( const char* name, const char* say );
	static void     OnSystemSay( const char* say );							// ϵͳ��Ϣ
	static void		OnRoadSay( CCharacter* p, const char* say );
	static void		OnWorldSay( const char* name, const char* say );
	static void		OnTradeSay( const char* name, const char* say );
	static void		OnTeamSay(DWORD cha_id, const char* say );
	static void		OnGuildSay( const char* name, const char* say );
	static void		OnPrivateSay( const char* srcName, const char* dstName,const char* say );
	static void		SetPrivateName(const char* srcName);
	static void		ResetAll();

	static CGrid*   GetFaceGrid()		{ return	grdFace;	}

	void		AddInput( const char* str );
	static void AddNameToCard(string strName);
	static bool IsForbidChannel(eChannel channel);

	stChatColor		chatColor;

protected:
	bool Init();
    void End();
	void CloseForm();

private:
	void		NextChatState();

	static void _evtGmForm( CForm* pForm, bool& IsShow );
	static void _evtSay(CGuiData *pSender);			// ��������
	static void _evtGmSay(CGuiData *pSender);
	static bool _evtSayKeyDown(CGuiData *pSender, int& key);
	static void _evtChannelSelectChange(CGuiData *pSender);
	static void _evtCardSelectChange(CGuiData *pSender);
	static void _evtShowChannelList(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtForbidCheck(CGuiData *pSender);
	static void _evtCardClick(CGuiData *pSender, int x, int y, DWORD key);
	
	static void _MainOnLstKeyDownEvent(CGuiData *pSender, int x, int y, DWORD key);
	static bool _MainOnSayKeyDownEvent( int& key );

	static void _DragBeginOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragMoveOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragEndOnSystemTitle(CGuiData *pSender, int x, int y, DWORD key);

	//static void _DragBeginOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key);
	//static void _DragMoveOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key);
	//static void _DragEndOnDiscordTitle(CGuiData *pSender, int x, int y, DWORD key);

	
	static void _DragBeginOnSay(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragMoveOnSay(CGuiData *pSender, int x, int y, DWORD key);
	static void _DragEndOnSay(CGuiData *pSender, int x, int y, DWORD key);

	static void _evtSelectBudChange(CGuiData *pSender) ;       //����ѡ�����仯
	static void _evtHideSelf(CGuiData *pSender);	// �����Լ�

	static void _evtForbidFormLost(CGuiData *pSender);
	static void _evtForbidSystemChange(CGuiData *pSender);



	//static void ShowLstOnSay(string str , DWORD color);
	static void PrintToList(eChannel channel, string name, string str, DWORD color);

private:
	CForm*			frmMainChat; 
	CForm*			frmGM; 
	CForm*			frmForbid; 

	static CCheckBox* m_pPrivateCheck;
	static CCheckBox* m_pSightCheck;
	static CCheckBox* m_pSystemCheck;
	static CCheckBox* m_pTeamCheck;
	static CCheckBox* m_pGuildCheck;
	static CCheckBox* m_pWorldCheck;
	static CCheckBox* m_pTradeCheck;

	
	//static CCheckBox*	m_p

	static CList*	lstOnSay;
	static CList*	lstOnSystemSay;
	//static CList*	lstOnDiscordSay;
	CList*			lstChannel;
	static CGrid*	grdFace;

	CDragTitle*		drpTitle;
	CDragTitle*		drpTitleSystem;
	//CDragTitle*		drpTitleDiscord;
	static CEdit*	edtSay;
	static CEdit*	edtGmSay;
	static CCheckBox* m_pForbidCheck;
	static CTextButton* m_pbtnCard;
	static CList* m_pCardList;


	static CCombo*	m_cmbChannel;

private:
	string		oldSay[MAX_PAGE_SAY];			// ��ǰ˵�Ļ�

	int		_nOldSayRow;		// ��һ��˵��ʱ��¼���к�,�������¼�ֱ����ʾ��ǰ����Ļ�
	int		_nSelectSayRow;

	int		_nBeginOnSayY;		// �϶�ǰlstOnSay��Yλ��
	int		_nBeginOnSayHeight;	// �϶�ǰlstOnSay�ĸ�
	int		_nDragSystemMinY, _nDragSystemMaxY;
	int		_nDragMinY, _nDragMaxY;
	//int		_nDragDiscordMinY, _nDragDiscordMaxY;

	static eChannel m_CurSelectChannel;
	static string m_CurCommand;
	static int m_TextPoint[CHANNELS_COUNT][2];
	static sTextInfo m_TextRecords[CHANNELS_COUNT][DATA_RECORDS_SIZE];
	static vector<string> m_strCardListData;
};
#endif

}


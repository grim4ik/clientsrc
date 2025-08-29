
#pragma once

#include "uiglobalvar.h"
#include "uiguidata.h"
#include "uiform.h"
#include "uilabel.h"
#include "uimemo.h"
#include "uiboxform.h"
#include "uifastcommand.h"
#include "uitreeview.h"
#include "uiprogressbar.h"
#include "UI3DCompent.h"
#include "NetProtocol.h"

#include <map>


namespace GUI
{

	// ������Ʒ�ﵥ��������Ϣ
	struct stItemInfo
	{
		short       itemID;         //	���߱��
		int         itemNum;        //	���߸���
		int         itemFlute;      //	������
		short       itemAttrID[5];  //	������Ա��
		short       itemAttrVal[5]; //	�������ֵ

		void Clear() { memset(this, 0, sizeof(stItemInfo)); }
	};

	// ������Ʒ��Ϣ
	struct stStoreInfo
	{
		long        comID;          //	��Ʒ���
		char        comName[21];    //	��Ʒ����
		long        comClass;       //	��Ʒ����
		long        comTime;        //	��Ʒ�ϼ�ʱ��
		long        comPrice;       //	�۸�
		char        comRemark[129]; //	��Ʒ��ע
		int         comNumber;      //  ��Ʒʣ�������-1�����ƣ�0�¼�
		time_t      comExpire;      //  ��Ʒ����ʱ�䡣
		int         itemNum;        //  ���߸���
		int         isHot;          //  �Ƿ�����
		stItemInfo  comItemInfo[6]; //  �����������

		stStoreInfo() { memset(this, 0, sizeof(stStoreInfo)); }
		void Clear()  { memset(this, 0, sizeof(stStoreInfo)); }
	};

	//  ������ƷGUI
	struct stStoreGui
	{
		CLabelEx*		labName;			// ��Ʒ��
		CLabelEx*		labPrice;			// ��Ʒ�۸�
		CLabelEx*		labLeftTime;		// ��Ʒʣ��ʱ��
		CLabelEx*		labLeftNum;			// ��Ʒʣ�����
		CLabelEx*		labRemark;			// ��Ʒ����
		CLabelEx*		labRightClickView;	// �Ҽ��鿴��ϸ
		CImage*			imgCutLine;			// �ָ���
		CImage*			imgHot;				// ����ͼ
		CImage*			imgNew;				// ��Ʒͼ
		CImage*			imgBlue;			// ѡ����Ʒ����
		CImage*			imgSquare;			// ���߸���
		COneCommand*	cmdStore;			// ��Ʒ����
		CTextButton*	btnBlue;			// ������ť
		
		CImage*			imgMoney;

		void SetIsShow(bool v)
		{
			labName->SetIsShow(v);
			labPrice->SetIsShow(v);
			labRemark->SetIsShow(v);
			labLeftTime->SetIsShow(v);
			labLeftNum->SetIsShow(v);
			labRightClickView->SetIsShow(v);
			imgCutLine->SetIsShow(v);
			imgHot->SetIsShow(v);
			imgNew->SetIsShow(v);
			imgBlue->SetIsShow(v);
			imgSquare->SetIsShow(v);
			cmdStore->SetIsShow(v);
			btnBlue->SetIsShow(v);
			imgMoney->SetIsShow(v);

			if(!v) cmdStore->DelCommand();
		}
	};


	class CStoreMgr : public CUIInterface
	{
	public:
		CStoreMgr();

		//void ShowTempKitbag(bool bShow = true);
		void ShowStoreForm(bool bShow = true);
		void ShowStoreLoad(bool bShow = true);
		void ShowViewAllForm(bool bShow = true);
		void ShowTryonForm(bool bShow = true);
		void ShowAlphaMatteForm(bool bShow = true);
		void ShowStoreWebPage();

	//	CGoodsGrid* GetBagOfHoldingGrid()		{  return grdBagOfHolding;	}
		CGoodsGrid* GetTempKitbagGrid()		{  return grdTempBag;	}
		CForm*		GetStoreForm()			{  return frmStore;		}
		static int	GetPageSize()			{  return STORE_PAGE_SIZE;  }
		
	//	CForm*			frmBagOfHolding;		

		// �������
		void DarkScene(bool bDark);

		// ����ʱ����ȡ��
		bool PopFromTempKitbag(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);

		
		
		//bool		PushToBagOfHolding(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);
		//bool		PopFromBagOfHolding(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem); 
		//bool		BagOfHoldingToBagOfHolding(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem); 
		
		// ���̳�ѯ��
		void OpenStoreAsk();

		// �����̳�����㣨�����Ϊ���ʾ����㣩
		void AddStoreTreeNode(long nParentID, long nID, const char* szCaption);

		// �����̳���Ʒ��Ϣ
		void AddStoreItemInfo(long nSeq, long nID, const char* szName, long nPrice, const char* szRemark, bool isHot, long nTime, long nRemainNum, long nRemainTime);

		// �����̳���Ʒϸ����Ϣ
		void AddStoreItemDetail(long nSeq, long nSubSeq, short sItemID, short sItemNum, short sFlute, short pItemAttrID[], short pItemAttrVal[]);

		// �����û����ý��
		void AddStoreUserTreeNode(void);

		// �����̳��б�ҳ
		void SetStoreItemPage(long nCurPage, long nMaxPage);

		// ����ħ������
		void SetStoreMoney(long nMoBean, long nRplMoney);

		// ����VIPֵ
		void SetStoreVip(long nVip);

		// �����̳ǹ���
		//void AddStoreCommunique(long nID, const char* szTitle, 

		// ˢ���̳���
		void StoreTreeRefresh();

		// ���ȫ�����̳������
		void ClearStoreTreeNode();

		// ���ȫ���̳���Ʒ�б�
		void ClearStoreItemList();

		// �����̳ǰ�ť�Ƿ����
		void SetStoreBuyButtonEnable(bool b);

		// ��õ�ǰѡ��ID
		int GetCurSelItemID();

		// ����������ʱ�䣬10 ��������
		bool ResetLastOperate(bool bSilent = false);

		// �û���Ϣ���
		static const long USER_NODEID = 0x7FFFFFFE;
		static const long HELP_NODEID = 0x7FFFFFFF;

	protected:
		virtual bool Init();
		virtual void CloseForm();
		virtual void SwitchMap();
		virtual void FrameMove(DWORD dwTime);

	private:

		// ��ʱ����
		//CForm*			frmTempBag;			// ��ʱ��������
		CGoodsGrid*		grdTempBag;			// ��ʱ��������
		
		
		//CGoodsGrid*		grdBagOfHolding;			// ��ʱ��������
		stNumBox*		m_pkNumberBox;		// ѯ�ʸ���
		stNetTempKitbag m_NetTempKitbag;	// ��ʱ��������Э������

		static void _evtDragItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		//static void _MoveItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		//static void _evtBagToBag(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap); 
		//static void _RenameBagOfHolding(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	private:
		//stNetBank		m_kNetBagOfHolding;
		// �̳�
		CForm*			frmStore;			// �̳ǽ���
		CTreeView*		trvStore;			// ���б�
		CLabelEx*		labMoneyLeft;		// ��Ǯʣ��
		CLabelEx*		labBeanLeft;		// ����ʣ��
		CLabelEx*		labMemberStyle;		// ��Ա����

		CLabelEx*		labNameTitle;
		CLabelEx*		labPriceTitle;
		CLabelEx*		labLeftTimeTitle;
		CLabelEx*		labLeftNumTitle;

		CLabelEx*		labListPage;		// ҳ��ǩ
		CTextButton*	btnLeftPage;		// ��ť
		CTextButton*	btnRightPage;		// �Ұ�ť
		CTextButton*	btnTrade;			// ���װ�ť
		CTextButton*	btnTryon;			// �Դ���ť
		CTextButton*	btnViewAll;			// �鿴ȫ����Ʒ��ť
		CLabelEx*		labTrade;			// ���װ�ť��
		CLabelEx*		labTryon;			// �Դ���ť��
		CLabelEx*		labViewAll;			// �鿴ȫ����Ʒ��ť��
		CMemo*			memStoreHelp;		// ������Ϣ����
		CImage*			imgBackGround10;	// ��ɫ���壨��Ʒ����ʱ��ʾ���Զ�����Ϣʱ����ʾ��
		stNumBox*		m_pkExchangeNum;	// �һ������������

		static const int STORE_PAGE_SIZE = 9;		  // ��Ʒ�б�һҳ��� 9 ��
		stStoreGui		m_stStoreGui [STORE_PAGE_SIZE];
		stStoreInfo		m_stStoreInfo[STORE_PAGE_SIZE];

		long			m_nCurClass;		// ��ǰ����
		long			m_nCurSel;			// ��ǰѡ��
		long			m_nCurPage;			// ��ǰҳ
		long			m_nMaxPage;			// ���ҳ
		long			m_nExchangeNum;		// �һ���
		long			m_nVip;				// �Ƿ���VIP


		typedef std::map<long, std::string> MapNode;
		typedef std::map<long, std::string>::iterator MapNodeIter;
		MapNode			m_mapNode;		// �����

		void _SetIsShowUserInfo(bool bShow);
		void _SetIsShowHelpInfo(bool bShow);
		void _SetIsShowCozeForm(bool bShow);
		void _RefreshStoreListHighLight();
		void _ShowTradeSelectBox();
		void _LoadStoreHelpInfo();
		void _ChangeChaPart(stNetLookInfo& SLookInfo);
		bool _IsCurSelVipNode(void);

		static void	_evtStoreTreeMouseClick(CGuiData *pSender, int x, int y, DWORD key);
		static void	_evtStoreOpenCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtStoreToVipEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtExchangeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtExchangeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtTradeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtStoreFormClose(CForm* pForm, bool& IsClose);

		static void _evtStoreFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtStoreListMouseRClick(CGuiData *pSender, int x, int y, DWORD key);
		static void	_evtStoreListMouseDBClick(CGuiData *pSender, int x, int y, DWORD key);

	private:

		// �̳Ƕ�ȡ����
		CForm*			frmStoreLoad;
		CProgressBar*	proStoreLoad;
		static void _evtStoreLoadFormClose(CForm* pForm, bool& IsClose);
		static void _evtProTimeArriveEvt(CGuiData *pSender);
		DWORD			_dwDarkScene;

		static const int STORE_OPEN_TIMEOUT = 15;	// �̳Ǵ򿪳�ʱ����λ���룩

	private:

		// �鿴��ϸ
		static const int STORE_ITEM_COUNT = 6;	// ��Ʒ�����������
		CForm*			frmViewAll;
		CMemo*			memViewAll;
		CImage*			imgSquareViewAll[STORE_ITEM_COUNT];
		COneCommand*	cmdSquareIcon[STORE_ITEM_COUNT];

		static void _evtStoreViewAllMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtStoreViewAllLostEvent(CGuiData *pSender);

	private:

		// �Դ�(Avata)
		CForm*			frmTryon;		// �Դ�����
		C3DCompent*		ui3dplayer;		// 3D�Դ�

		int				m_nChaRotate;	// ��ɫ��ת
		CCharacter*		m_pCurrMainCha;
		bool			m_isFight;
		stNetLookInfo	m_sLookInfo;
		stNetLookInfo	m_sCurLookInfo;

		void			ChaEquipClearAll();
		void			ChaLeftRotate();
		void			ChaRightRotate();
		void			RenderChaTryon(int x, int y);	// ���� 3D ��ɫ

		static void	_evtChaTryonRenderEvent(C3DCompent *pSender, int x, int y);
		static void _evtChaEquipClearAll(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaLeftRotate(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaRightRotate(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaLeftContinueRotate(CGuiData *sender);
		static void _evtChaRightContinueRotate(CGuiData *sender);
		static void _evtTryonFormClose(CForm* pForm, bool& IsClose);

	};

}


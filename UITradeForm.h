#pragma once
#include "UIGlobalVar.h"
#include "gamecommon.h"

struct NET_CHARTRADE_BOATDATA;
namespace GUI
{
// ����ҽ���
class CTradeMgr : public CUIInterface
{
public:
	void	ShowCharTradeRequest( BYTE byType, DWORD dwRequestID );				// ��ʾһ�������
	void	ShowCharTrade( BYTE byType, DWORD dwAcceptID,DWORD dwRequestID );	// ��ʾ���׽���
	void	CancelCharTrade(  DWORD dwCharID );						// ȡ������
	void	ShowCharTradeMoney(DWORD dwCharID, DWORD dwMoney);   // ��ʾ���׵Ľ��
	void	ShowCharTradeIMP(DWORD dwCharID, DWORD dwMoney);   // ��ʾ���׵Ľ��

	void	ValidateTradeData( DWORD dwCharID );
	void	ValidateTrade(  DWORD dwCharID );
	void	ShowTradeSuccess(); // ��ʾ���׳ɹ�
	void	ShowTradeFailed();  // ��ʾ����ʧ��

	void    CloseAllForm();		// �رմ���

	// �϶������������
	void	DragTradeToItem(DWORD dwCharID,BYTE byIndex,BYTE byItemIndex );

	//sItem: ���׵ĵ����ڵ��߱��е����У� byIndex : �ڽ��ױ����е����� byCount�����׵�����
	//byItemIndex: ���������ڵ�����������
	void	DragItemToTrade(DWORD dwCharID,USHORT sItemID,BYTE byIndex,BYTE byCount,BYTE byItemIndex, SItemGrid* pGrid, const NET_CHARTRADE_BOATDATA* const pBoat );

	CGoodsGrid*     GetPlayertradeSaleGrid()    { return  grdSale;		} 
	CGoodsGrid*		GetRequestGrid()			{ return  grdRequest;	} //

	void			LocalSaleItem( CGoodsGrid* pSaleGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem );  // ���׵ĵ���
	void			LocalCancelItem( CGoodsGrid* pSaleGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem );// ȡ�����׵ĵ���

	// �Ƿ����ڽ�����
	bool			IsTrading();

	CForm*			GetForm()	{ return frmPlayertrade; } //

protected:
	bool Init();
    void End();
    void FrameMove(DWORD dwTime);

	void Clear();

protected:
	static void _evtSelectYesNoEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
    static void _evtThrowItemEvent(CGuiData *pSender,int id,bool& isThrow);     // �ӵ��������ӳ�����

	static void _evtIMPFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtGoldFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _MainMousePlayerTradeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtSelfRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID);		// ���׽����鿴�Լ�����Ʒ����
	static void _evtOtherRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID);		// ���׽����鿴�Է�����Ʒ����

private:
	CGoodsGrid* GetPlayertradeBuyGrid()     { return  grdBuy;	}
	static void _evtLocalSaleEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void	_OnDragTrade(CGuiData *pSender, int x, int y, DWORD key);

private:
	CForm*		frmPlayertrade;
	CForm*		frmRequest;				//������ߵĵ��߱���(�򴬲�)
	CGoodsGrid* grdRequest;
	CGoodsGrid*	grdBuy;
	CGoodsGrid*	grdSale;
	CLabelEx*	labOtherGold;
	CLabelEx*	labSelfGold;

	CLabelEx*	labOtherIMP;
	CLabelEx*	labSelfIMP;
	
	CTextButton*	btnTrade;
	CTextButton*	btnYes;
	CTextButton*	btnGold;
	CTextButton*	btnIMP;

	CCheckBox*		chkTrade;
	CCheckBox*		chkYes;

	DWORD			m_dwAcceptID;         // ���׽��ܷ�
	DWORD			m_dwRequestID;        // �������뷽
	BYTE			m_bTradeType;		  // ��������,�ο�RoleCommand.h TRADE_CHAR_TYPE

	DWORD			m_dwMainID;

private:
	struct stSale
	{
		stSale() : dwSaleID(0), nGridID(0), nDragID(0) {}

		DWORD   dwSaleID;
		int		nGridID;
		int		nDragID;
	};
	stSale		_sSale;

};

}


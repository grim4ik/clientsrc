#pragma once
#include "UIGlobalVar.h"
#include "uicloneform.h"
#include "uiform.h"

namespace GUI
{
struct stTradeBox
{
	stTradeBox();
	void Refresh();
	int GetTradeNum();

	CForm*  frmDialog;
	CLabel*	labPrice;
	CEdit*	edtNumber;
	CLabel* labName;

	// �û�����
	float	fUnitMoney;
	int		nTotalNum;

};

struct stSelectBox
{
	stSelectBox();

	CForm*  frmDialog;
	CLabel* labInfo;

	// �û�����
	DWORD	dwTag;
	DWORD	dwParam;
	void*	pointer;
};

struct stNumBox
{
	stNumBox();
	void Refresh();
	int GetNumber();
	void GetString(const char*& out);

	CForm*  frmDialog;
	CEdit*	edtNumber;
	CLabel* labInfo;

	// �û�����
	int		nTotalNum;
	void*	pointer;
};

struct stPasswordBox
{
	stPasswordBox():frmDialog(nullptr),edtPassword(nullptr),labInfo(nullptr){}
	CForm*  frmDialog;
	CEdit*	edtPassword;
	CLabel* labInfo;
};

struct stMsgBox
{
	CForm*  frmDialog;
	CLabel* labInfo;
	
	// �û�����
};

struct stMsgTimeBox
{
	CForm*  frmDialog;
	CLabel* labInfo;

	// �û�����
	short teamID;
};

// һЩͨ�öԻ���
class CBoxMgr : public CUIInterface
{
public:
	// �����õĶԻ���
	static stTradeBox*		ShowTradeBox( FormMouseEvent evtForm=nullptr, float fUnitMoney=0, int nTotalNum=-1, const char* szName=nullptr );

	// ѡ��ȷ����ȡ���ĶԻ���
	static stSelectBox*		ShowSelectBox( FormMouseEvent evtForm=nullptr, const char* szTitle=nullptr, bool bModal=false );

	// ���������Ի���
	static stNumBox*		ShowNumberBox( FormMouseEvent evtForm=nullptr, int nTotalNum=-1, const char* szTitle=nullptr, bool IsMax=true, bool bModal=true );

	// ��������Ի���
	static stPasswordBox*	ShowPasswordBox( FormMouseEvent evtForm=nullptr, const char* szTitle=nullptr, const char* szPassword=nullptr, bool bModal=true );

	// ��ʾһ���Ի���
	static stMsgBox*		ShowMsgBox( FormMouseEvent evtForm=nullptr, const char* szTitle=nullptr, bool bModal=true );

	static stMsgTimeBox*	ShowMsgTime(  FormMouseEvent evtForm=nullptr, const char* szTitle=nullptr, int iSeconds = 60 );

	static void				CloseAllBox();

protected:
	virtual bool Init();
    virtual void End();
	virtual void FrameMove(DWORD dwTime);

private:
	static void _ClearBox( CHideForm& list );

	CForm*		_FindForm(const char * frmName);

private:
	// ���������Ի���:����,�ܼ�
	static CHideForm	_cTrade;	

	// ѡ����,����ȡ��
	static CHideForm	_cSelect;	

	// ���������Ի���
	static CHideForm	_cNumber;

	// ��������Ի���
	static CHideForm	_cPassword;

	// ��ʾһ���Ի���
	static CHideForm	_cMsg;

	// ��ʱ��Tick
	static CForm*	_cfrmError;
	static DWORD    _cTick;
	static int		_cSeconds;
	static bool		_cStart;
	
};

}

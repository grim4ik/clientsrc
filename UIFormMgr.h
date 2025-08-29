//--------------------------------------------------------------
// ����:����������
// ����:lh 2004-07-08
// ���˼��:�������б���
// ��;:��ʾ,����,���ұ���,�л�����ģ��,�ṩȫ�ֹ��ӻص��ӿ�
// ����޸�����:2004-10-09
//--------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uiform.h"

namespace GUI
{
class CFormMgr;
typedef bool (*FormMgrEvent) ( CFormMgr* pSender );

typedef bool (*KeyDownEvent) (int& key);
typedef bool (*KeyCharEvent) (char& key);
typedef bool (*MouseEvent) (int& x, int& y, DWORD& mouse);
typedef bool (*MouseScrollEvent) (int& nScroll);
typedef bool (*HotKeyEvent) ( char& key, int& control );	// ����true�ж�


class CFormMgr
{
	friend class CForm;
public:	
	CFormMgr();
	~CFormMgr();

	static void		SetDebugMode(bool v) { _IsDebugMode=v;}		//�����Ƿ���������ģʽ���ɿ���̨������ã�-added by Arcol
	void			ShowDebugInfo();							//��ʾ������Ϣ -added by Arcol
	static void		SetDrawFrameInDebugMode(bool v)	{_IsDrawFrameInDebugMode=v;}	//�����ڵ���ģʽ���Ƿ���ʾ���ڱ߿� -added by Arcol
	static void		SetDrawBackGroundInDebugMode(bool v) {_IsDrawBackgroundInDebugMode=v;}	//���ڵ���ģʽ���Ƿ���ʾ��ɫ -added by Arcol

	bool			AddForm( CForm* form, int templete=0 );

	bool			SetFormTempleteMax(int n);	
	unsigned int	GetFormTempleteMax()			{ return (unsigned int)_showforms.size();	}
	int				GetFormTempletetNum()			{ return _nTempleteNo;						}

	bool			SwitchTemplete( int n );		// �л���ǰģ��,���Ϊ-1,���л���Ĭ��ģ��

public:
	bool			Init(HWND hWnd);
	void			Clear();

    void            FrameMove( int x, int y, DWORD dwMouseKey, DWORD dwTime );
	void			Render();
	void            RenderHint( int x, int y );
	bool			HandleWindowMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);

	bool			OnKeyDown(int key);
	bool			OnKeyChar(char key);
	bool			OnHotKey( char key, int control );

	bool			MouseScroll( int nScroll );
	void			Refresh();

    void            MouseReset();

	CForm*			GetHitForm( int x, int y );

	CForm*			Find( const char * );			// �ڵ�ǰģ���У�����Form
	CForm*			Find( const char *str, int no );// ��noģ���У�����Form
	CForm*			FindAll( const char* );			// �����б����У�����Form

	CForm*			FindESCForm();					// ���ҿ���ESC�رյ�Form
	int				ModalFormNum()					{ return (int)_modal.size();		}

	typedef void (*FormFun)(CForm* pSender);
	void			ForEach( FormFun pFun );		// ����Form��ִ��һ��pFun����

public:
	void			SetEnabled( bool v )			{ _bEnabled = v;	}
	bool			GetEnabled()					{ return _bEnabled; }
    void            ResetAllForm();
    void            SetScreen();

	void			SetEnableHotKey(int flag, bool v);		// �����ĵ��޸�
	bool			GetEnableHotKey()				{ return _nEnableHotKey == 0xFFFFFFFF; }// �����ĵ��޸�

	static bool		IsMouseInGui()					{ return _eMouseAction==enumMA_Gui;	}
	static eMouseAction	GetMouseAction()			{ return _eMouseAction;	}

public:	// ȫ�ֹ���
	bool			AddFormInit( FormMgrEvent pInitFun );	// ����ű���,��ʼ��ʱ�¼�

	bool			AddKeyDownEvent( KeyDownEvent event );
	bool			DelKeyDownEvent( KeyDownEvent event );

	bool			AddKeyCharEvent( KeyCharEvent event );
	bool			DelKeyCharEvent( KeyCharEvent event );

	bool			AddMouseEvent( MouseEvent event );
	bool			DelMouseEvent( MouseEvent event );

	bool			AddMouseScrollEvent( MouseScrollEvent event );
	bool			DelMouseScrollEvent( MouseScrollEvent event );

	bool			AddHotKeyEvent( HotKeyEvent event );
	bool			DelHotKeyEvent( HotKeyEvent event );

public:
	static CFormMgr	s_Mgr;

private:
	bool			_AddMemory( CForm* form );
	bool			_DelMemory( CForm* form );

	bool			_MouseRun( int x, int y, DWORD mouse );

	void			_ShowModal( CForm* form );
    void            _ModalClose( CForm* form );

	void			_SetNewActiveForm();		// ��ǰ����form��ʧ��ҪѰ��һ���µ�form����

	void			_InitFormID();

    void            _DelShowForm( CForm* frm );
    void            _AddShowForm( CForm* frm );
    void            _UpdataShowForm( CForm* frm );

    void            _ActiveCompent();

private:
	typedef std::list<CForm*> vfrm;
	vfrm			_allForms;
	vfrm*			_forms;						// ��ģ̬��ʾ������Դ,��һ��Ϊ��ǰ����ĵı���
	vfrm			_modal;						// ������ʾ��ģ̬��
    vfrm            _show;                      // ������ʾ�ķ�ģ̬��

	typedef std::vector<vfrm*> frmtemplete;
	frmtemplete		_showforms;
	vfrm			_defaulttemplete;

	bool			_bEnabled;		
	bool			_bInit;
	int				_nEnableHotKey;		// �����ĵ��޸�

	typedef std::vector<KeyDownEvent>	vkeydowns;
	vkeydowns		_OnKeyDown;

	typedef std::vector<KeyCharEvent>	vkeychars;
	vkeychars		_OnKeyChar;

	typedef std::vector<MouseEvent>		vmouses;
	vmouses			_OnMouseRun;
	
	typedef std::vector<MouseScrollEvent> vscrolls;
	vscrolls		_OnMouseScroll;

	typedef std::vector<FormMgrEvent>	vinits;
	vinits			_vinits;

	typedef std::vector<HotKeyEvent>		vhotkey;
	vhotkey			_vhotkey;

	int _nTempleteNo{ -1 };			// ��ǰ��ģ���

	int _nMouseHover{ 0 };              // ����ڲ����κβ����Ѿ�����ʱ��
    
    CGuiData*       _pHintGui;

	static eMouseAction		_eMouseAction;
	static bool		_IsDebugMode;				// �Ƿ��ڵ���ģʽ -added by Arcol
	static bool		_IsDrawFrameInDebugMode;	// �����ڵ���ģʽ -added by Arcol
	static bool		_IsDrawBackgroundInDebugMode;	// �����ڵ���ģʽ -added by Arcol

private:
	void			_DelForm( vfrm& list, CForm* frm );

};

inline void CFormMgr::MouseReset()                    
{ 
    _nMouseHover=0;     
}

}


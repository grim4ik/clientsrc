//----------------------------------------------------------------------
// ����:��ť
// ����:lh 2004-07-19
// ��;:����ʾ����
// ����޸�����:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "UIForm.h"

namespace GUI
{

class CTextButton : public CCompent
{
public:
	enum eButtonState	// ��ť״̬����������긡�������棬�Ѿ����£�����
	{
		csNormal = 0,
		csHover,
		csDown,
		csDisable,
		csEnd,
	};

public:
	CTextButton(CForm& frmOwn);
	CTextButton(const CTextButton& rhs);
	CTextButton& operator=(const CTextButton& rhs);
	~CTextButton(void);
	GUI_CLONE(CTextButton)

	virtual void		Render();
	virtual void		Refresh();
	virtual void		SetAlpha( BYTE alpha )  { _pImage->SetAlpha(alpha); }

	virtual bool		IsHandleMouse()         { return true;              }
	virtual bool		MouseRun( int x, int y, DWORD key );
    virtual void		FrameMove( DWORD dwTime );
	virtual bool		IsFrameMove()			{ return true;				}

	virtual CGuiPic*	GetImage()              { return _pImage;           }

	const char*			GetCaption()            {return _strCaption.c_str();}
	void				SetCaption( const char * str ) { _strCaption = str; }

	virtual	void		SetIsEnabled( bool v );

	// �ļ��д洢��ʽ�����ΰ�csNormal,csHover,csDown,csDisable���У�w��hΪ��Ԫ����
	bool				LoadImage( const char* file, int w=32, int h=32, int tx=0, int ty=0, bool isHorizontal=true );

	DWORD				GetTextColor()						{ return _textColor;		}
	void				SetTextColor(DWORD color )			{ _textColor=color;			}
	bool                GetState()                          { return _isDown;           }

    void				SetFormModal( CForm::eModalResult v );
	CForm::eModalResult GetFormModal()						{ return _eFormModal;		}

	void				DoClick(eMouseState state = Mouse_LUp);		// �ⲿģ�����¼�

	// ������˸���ڣ����룩���� 0 ����˸
	void				SetFlashCycle( DWORD dwCycle = 1000 )	{ _dwFlashCycle = dwCycle;  }
	DWORD				GetFlashCycle()			{ return _dwFlashCycle;		}

public:	// �¼�
	GuiMouseEvent		evtMouseClick;			// ����������¼�
	GuiMouseEvent		evtMouseRClick;			// ����Ҽ�����¼�  add by Philip.Wu 2006/01/23
	GuiMouseEvent		evtMouseDBClick;		// ������˫���¼�  add by Philip.Wu 2006/01/23
	GuiEvent			evtMouseDownContinue;	// ���������ڰ�ť

protected:
	void				_SetState( eButtonState v );
	void				_ClearOldState();

protected:
	std::string				_strCaption;	// ��ʾ�ڱ��������
	DWORD				_textColor;		// ������ɫ

	CGuiPic*			_pImage;		// ����ͼ,����֡���ֱ��Ӧ��bsNormal,bsHover,bsDown,bsDisable
	bool				_isDown;		// ��ǰ�Ѿ��ڰ�ť�ϰ��£������ɿ�ִ��evtMouseClick�¼�

    CForm::eModalResult _eFormModal;    // ������������Ϊģ̬��ʾʱ�����ڱ����ķ���ֵ

	DWORD				_dwFlashCycle;	// ��˸���ڣ�=0 ����˸
	DWORD				_dwLastClick;	// ��һ�ε����ʱ��

	static CTextButton*	m_pCurButton;
};

inline void CTextButton::SetFormModal( CForm::eModalResult v )              
{ 
    if( v>=CForm::mrNone && v<CForm::mrEnd )
        _eFormModal=v;            
}

}

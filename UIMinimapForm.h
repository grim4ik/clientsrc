#pragma once
#include "UIGlobalVar.h"
#include "UILabel.h"
#include "FindPath.h"
#include "UICheckBox.h"
namespace GUI
{
// С��ͼ
class CMiniMapMgr : public CUIInterface
{
public:
	CMiniMapMgr() : 
	  frmMinimap(0), MinimatRect(0), labMapPos(0), labMapName(0), frmBigmap(0), labBigMapCoord(0), labBigMapMapName(0){}


	// ����С��ͼ����ʱ�õ�Form��С
	CCompent*	GetMinimapRect()			{ return MinimatRect;	}

	void		RefreshChaPos( int x, int y );
	void        RefreshMapName (const char * name );

	C3DCompent*	GetBigmapRect()				{ return d3dBigmap;		}

	bool		IsShowBigmap();

	CForm*		GetBigmapForm()				{ return frmBigmap;		}

	void		ClearRadar();

	void        CloseRadar();

	const char* GetMapName()				{ return labMapName->GetCaption(); }

	const char* GetBigMapName()				{ return labBigMapMapName->GetCaption(); }

	bool		IsPKSilver();
	bool		IsGuildWar();
	void		ShowRadar(const char * szX,const char * szY);//add by alfred.shi 20080710

	void		SetCiPanel(DWORD type,const char* data);//@mothannakh
	void		SetClockStringClientSide();	//if we want set clock as client side only @mothannakh
	CCheckBox*		chkID;


	CLabelEx*		GetLabMapPos(){return labMapPos;}

	CLabelEx*		GetBigMapPos(){return labBigMapCoord;}

protected:
	virtual bool Init();
    virtual void End();
	virtual void SwitchMap();

private:
	static void _MiniFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _RadarFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	static void _RenderBigmapEvent(C3DCompent *pSender, int x, int y);
	static void _evtShowbigmap(CGuiData *pSender);
	static void _evtHidebigmap(CGuiData *pSender);

	//�״�����enter�¼�
	static void _evtRadarEnter(CGuiData *pSender);
	static void _RadarFormBeforeShowEvent(CForm* pForm, bool& IsShow);

	static void _evtShowNPCList(CGuiData *pSender,int x,int y ,DWORD key);
	static void _evtShowQQ(CGuiData *pSender,int x, int y, DWORD key );

	static void ShowRadar();

	// �������������Ƿ��зǷ��ַ�(ֻ֧������) ��ȷ����0,�������ݶ�Ϊ-1
	static int CheckCoordinateEdit(const char* input);

	// ���ƴ��ͼ��ʾ
    void _RenderBigMapHint(void);//Add by sunny.sun 20080903

	//С��ͼ����
	CForm*		frmMinimap;
	CCompent*	MinimatRect;

	enum 
	{
		MAP_POS_MAX = 10,
	};

	enum BigMapCoords 
	{
		BIGMAP_POS_MAX = 10,
	};
    //С��ͼ�ؼ�
	CLabelEx	*labMapPosRand[MAP_POS_MAX];
	CLabelEx	*labMapPos;
	CLabelEx    *labMapName;


	CLabelEx	*labBigMapCoordRand[BIGMAP_POS_MAX];
	CLabelEx	*labBigMapCoord;
	CLabelEx    *labBigMapMapName;
	
	CTextButton	*btnQQ;
	CTextButton	*btnPosSearch;	
	CTextButton *btnSeach;
	
	CForm*		frmBigmap;
	C3DCompent*	d3dBigmap;

	static CForm*		frmRadar;
	static CEdit*		edtX;
	static CEdit*		edtY;
	static CCombo*		cboAddr;
	//ci panel @mothanankh
	CForm* CiPanel;
	CLabelEx* RedTeamPoints;
	CLabelEx* GreenTeamPoints;
	CLabelEx* ServerClock;
	CLabelEx* CiTimer;
	//CMPFont				g_CFont;

	static void	_OnDragRadar(CGuiData *pSender, int x, int y, DWORD key);
};

}


#include "StdAfx.h"

#include "Character.h"
#include "chastate.h"
#include "GameApp.h"
#include "MapSet.h"
#include "SkillStateRecord.h"
#include "uiboatform.h"
#include "uiequipform.h"
#include "uiheadsay.h"
#include "uiitem.h"
#include "UILabel.h"
#include "uiMiniMapForm.h"
#include "UIProgressBar.h"
#include "uistartForm.h"
#include <unordered_set>

//using namespace GUI;

bool g_IsShowShop = true;
bool g_IsShowShop2 = true;
bool g_IsShowHpSp = false;
bool g_IsShowNames = false;
bool g_IsShowStates = false;
extern bool CanRender;
extern std::unordered_set<DWORD> visitedStalls;
//---------------------------------------------------------------------------
// calss CHeadSay
//---------------------------------------------------------------------------
int CHeadSay::_nMaxShowTime = 100;
CGuiPic* CHeadSay::_pImgLife = new CGuiPic(nullptr, 2);
CGuiPic* CHeadSay::_pImgMana = new CGuiPic(nullptr, 2);

DWORD CHeadSay::_dwBkgColor = 0x90000000;

unsigned int	CHeadSay::_nFaceFrequency=3;
CGuiPic* CHeadSay::_pImgFace = new CGuiPic[FACE_MAX];
CGuiPic* CHeadSay::_pImgEvil = new CGuiPic[EVIL_MAX];

CGuiPic* CHeadSay::_pImgTeamLeaderFlag = new CGuiPic;
CGuiPic* CHeadSay::_pImgGuildLeaderFlag = new CGuiPic;

CGuiPic CHeadSay::_ImgShop[3];
int		CHeadSay::_nShopFrameWidth = 0; 
int		CHeadSay::_nShopFontYOff = 0;

//������ʾ��ʽ (ǰ׺) �� ��ɫ�� + (������) �� [����]
char CHeadSay::s_sNamePart[NAME_PART_NUM][64] = { 0 };


char CHeadSay::s_szName[1024] = {0};

DWORD CHeadSay::s_dwNamePartsColors[NAME_PART_NUM][2] =			//	ǰһ����ǰ��ɫ,��һ������Ӱ��ɫ
	{
		COLOR_WHITE,			COLOR_BLACK,		//	(
		0,						COLOR_BLACK,		//	ǰ׺��
		COLOR_WHITE,			COLOR_BLACK,		//	)
		0,						COLOR_BLACK,		//	��ɫ��
		COLOR_BLACK,			0,		//	(
		COLOR_BLACK,			0,		//	������
		COLOR_BLACK,			0,		//	)
		COLOR_WHITE,			COLOR_BLACK,		//	[
		COLOR_SHIP_NAME,		COLOR_BLACK,		//	����
		COLOR_WHITE,			COLOR_BLACK,		//	]
	};

char CHeadSay::s_szConsortiaNamePart[4][64] = { 0 };


char CHeadSay::s_szConsortiaName[256] = {0};


// Member Methods---------------------------------------------------------

bool CHeadSay::Init()
{
	_pImgTeamLeaderFlag->LoadImage( "texture/ui/flag.tga", 12, 12, 0, 0, 0, 1.0, 1.0 );
	_pImgGuildLeaderFlag->LoadImage( "texture/ui/guildflag.tga", 16, 16, 0, 0, 0, 0.8, 0.8 );

	_nShopFrameWidth = _ImgShop[0].GetWidth() - 1;
	_nShopFontYOff = (_ImgShop[0].GetHeight() - CGuiFont::s_Font.GetHeight( g_oLangRec.GetString(489) ))/2;

	for (int i = 0; i < EVIL_MAX; i++)
	{
		char buffer[64] = {0};
		sprintf(buffer, "texture/icon/rank%d.png", i );

		if (!_pImgEvil)
			break;

		_pImgEvil[i].LoadImage(buffer, 16, 16, 0, 0, 0, 0.0, 0.0);
	}
	return true;
}

bool CHeadSay::Clear()
{
    if( _pImgLife )
    {
        delete _pImgLife;
        _pImgLife = nullptr;
    }
	
	if( _pImgMana )
    {
        delete _pImgMana;
        _pImgMana = nullptr;
    }

    if( _pImgFace )
    {
        delete [] _pImgFace;
        _pImgFace = nullptr;
    }

	if( _pImgTeamLeaderFlag )
	{
		delete _pImgTeamLeaderFlag;
		_pImgTeamLeaderFlag = nullptr;
	}
	
	if( _pImgGuildLeaderFlag )
	{
		delete _pImgGuildLeaderFlag;
		_pImgGuildLeaderFlag = nullptr;
	}

	if (_pImgEvil)
	{
		delete[] _pImgEvil;
	}

	return true;
}

CHeadSay::CHeadSay(CCharacter* p) 
: _pOwn(p), _nShowTime(0), _pObj(nullptr), _fLifeW(1.0f ), _nFaceTime(0), _pCurFace(nullptr), _dwFaceTime(0), _IsShowLife(false), _IsShowMana(false)
,_IsShowName(false) , _nChaNameOffX(0) , _fScale( 0.1f), _dwNameColor(COLOR_WHITE), _sEvilLevel(0), _IsShowEvil(false),isGuildMember(false),isTeamMember(false)
{
}

void CHeadSay::AddItem( CItemEx* obj ) 
{ 
    if( _pObj ) delete _pObj;

    _pObj = obj;

    _nShowTime = _nMaxShowTime;
    _fScale    = 0.1f ;
}

void CHeadSay::SetName( const char* name )
{
    _nChaNameOffX = 0 - CGuiFont::s_Font.GetWidth( name )/2;
}

void CHeadSay::RenderStateIcons(CCharacter* cha,int x,int y,float scale,float spacing,int rowSize,bool Rendertimer){
	if (!cha->IsMainCha()) return;
	CGuiPic stateIcon;
	int stateCount = 0;
	int nTotalState = CSkillStateRecordSet::I()->GetLastID() + 1;
	bool IsStatesPerLevel = false;
	bool RenderIcon = false;
	CSkillStateRecord* pState;
	for( int i=0; i<nTotalState; i++ ){
		if(cha->GetStateMgr()->HasSkillState( i )){
			pState = GetCSkillStateRecordInfo( i );
			if( pState){
				if (0 != stricmp(pState->szIcon[0], "0"))
				{
					if (0 == stricmp(pState->szIcon[cha->GetStateMgr()->GetStateLv(i) - 1], "0"))
					{
						IsStatesPerLevel = false;
					}else  IsStatesPerLevel = true;
					RenderIcon = true;
				}
				if (RenderIcon)
				{
					char buf[64];

					sprintf(buf, "texture/icon/%s.png", (IsStatesPerLevel? pState->szIcon[cha->GetStateMgr()->GetStateLv(i) - 1]: pState->szIcon[0]));
					stateIcon.LoadImage( buf, 33, 33, 0, 0, 0, scale, scale );
					
					int yspace = 0;
					if (Rendertimer) yspace = 22;
					
					int xi = x +((stateCount%rowSize)*spacing);
					int yi = y +((stateCount/rowSize)*(spacing+yspace));
					if(Rendertimer) g_pGameApp->RenderStateHint(xi,yi,cha->GetStateMgr()->GetStateData(i));
					stateIcon.Render(xi,yi);
					stateCount++;
					int xm = g_pGameApp->GetMouseX();
					int ym = g_pGameApp->GetMouseY();
					//check if we need to render hint.
					if(xm>=xi && xm<=xi+(33*scale)){
						if(ym>=yi && ym<=yi+(33*scale)){
							char desc[300];
							CChaStateMgr::stChaState stateData = cha->GetStateMgr()->GetStateData(i);
							g_pGameApp->ShowStateHint( xm, ym, stateData);
						}
					}
				}
			}
		}
	}
	//hide states form if has 0 active states 
	if(stateCount > 0 && g_IsShowStates)
	{
		int w = 33* stateCount;
		int h = stateCount <= 9 ? 45: stateCount >=18 ? 120:94;
		if (w > 250) w = 250;
		g_stUIEquip.stateDrags->SetSize(w,h);
		g_stUIEquip.stateDrags->Refresh();
		g_stUIEquip.stateDrags->SetIsShow(true)	;
	}else{
		g_stUIEquip.stateDrags->SetIsShow(false)	;
	}
	//	
}


void CHeadSay::Render( D3DXVECTOR3& pos )
{
    static int x=0, y=0;
    static int nSayTotalWidth =  32 * CGuiFont::s_Font.GetWidth("a") ; //32���ַ��ĳ���
    g_Render.WorldToScreen( pos.x, pos.y, pos.z + _pOwn->GetDefaultChaInfo()->fHeight, &x, &y );   

	
	if(_pOwn->getHumanID() == g_stUIStart.targetInfoID && g_stUIStart.frmTargetInfo->GetIsShow()){
		float scale = 0.45;
		float spacing = scale * 32 + 2;
		int height = 40 + g_stUIStart.frmTargetInfo->GetY();
		if (!CanRender) height = 60 + g_stUIStart.frmTargetInfo->GetY();
		RenderStateIcons(_pOwn, g_stUIStart.frmTargetInfo->GetX(), height, scale, spacing, 10,false);
	}	
	
	if( _IsShowLife && _pOwn->getChaCtrlType() != enumCHACTRL_Mount)
    {
		static int x1 = 0, y1 =0;
		g_Render.WorldToScreen( pos.x, pos.y, pos.z , &x1, &y1 );
		static char szBuf[32] = { 0 };

        int nLifeWidth   = _pImgLife->GetWidth();
        _pImgLife->SetScaleW(1,_fLifeW);	
		int nOffset =  (int)(( x - g_Render.GetScrWidth()/2 ) * 0.02f );
		
		DWORD hpcolour = 0xA0FF0000;
			if(_fLifeW > 0){
				int red = _fLifeW * 255;
				hpcolour = 0xA0000000 + (65536 * red);  // Update color with new red value, keeping only the red channel
			}
	
		
		if (  _pOwn->IsPlayer() || _pOwn == CGameScene::GetMainCha() || !_pOwn->IsNPC() )
		{
			if ( _pOwn->IsPlayer() || _pOwn == CGameScene::GetMainCha() ) {
				sprintf(szBuf, "%.f%%", _fLifeW * 100);
				int iStartPosX = 0 - CGuiFont::s_Font.GetWidth(szBuf)/2;
				CGuiFont::s_Font.BRender(szBuf, x + iStartPosX , y1 + 8, (DWORD)hpcolour , COLOR_BLACK );	
			} else {
				sprintf(szBuf, "%d\/%d", _fCurHp, _fMxHp);
				int iStartPosX = 0 - CGuiFont::s_Font.GetWidth(szBuf)/2;
				CGuiFont::s_Font.BRender(szBuf, x + iStartPosX, y1 + 8, (DWORD)hpcolour , COLOR_BLACK );
			}
		}

		_pImgLife->RenderAll( x - nLifeWidth/2 - nOffset , y1 + 20, hpcolour );


		if(_pOwn->GetStateMgr()->HasSkillState( 83 ) ){
			static int x1 = 0, y1 =0;
			g_Render.WorldToScreen( pos.x, pos.y, pos.z , &x1, &y1 );
			int nManaWidth   = _pImgMana->GetWidth();
			_pImgMana->SetScaleW(1,_fManaW);	
			int nOffset =  (int)(( x - g_Render.GetScrWidth()/2 ) * 0.02f );
			DWORD spcolour = 0xA00000FF; // Initial color with full blue and alpha
			if (_fManaW > 0) {
				int blue = _fManaW * 255; // Calculate the blue value based on _fManaW
				spcolour = 0xA0000000 + blue; // Update color with new blue value, keeping only the blue channel
			}
			_pImgMana->RenderAll( x - nManaWidth/2 - nOffset , y1 + 28, spcolour );
			
			char spInfo[32];
			sprintf(spInfo,"%d/%d",_fCurSp,_fMxSp);
			CGuiFont::s_Font.BRender( spInfo, x  - nOffset - (CGuiFont::s_Font.GetWidth(spInfo)/2), y1 + 32, spcolour, COLOR_BLACK );
		}
		
		if(g_IsShowStates && !g_stUIMap.IsPKSilver() ){	//states icons dragable @mothannakh
			float scale = 0.67;	//.045
			float spacing = scale * 32;
			//new code 
			int picX = 0;
			int picY = 0;
			int iconqty = 5;
			if (_pOwn == CGameScene::GetMainCha() )
			{
				picX = g_stUIEquip.stateDrags->GetLeft();	   
				picY = 	g_stUIEquip.stateDrags->GetTop();
			}else
			{
				scale = 0.45;
				spacing = scale * 32 + 2;
				picX = x-(_pImgMana->GetWidth()/2)-nOffset-1;
				picY = y1+28;
				iconqty = 4;
				if(g_IsShowHpSp){
					picY += 12;
				}				
			}
			//end of new code 
			RenderStateIcons(_pOwn,picX,picY, scale, spacing,iconqty,true);
		}

		
    }

	
	
	
	
	
	
	
	if(g_stUIMap.IsPKSilver())
	{
		// ���Ҷ������������ͷ�ϲ���ʾ�κ����ݣ�����ʾ��һ�ֵ��?��
		return;
	}

	if(_pOwn->GetIsFly())
		y -= 30;


	if( _pOwn->IsTeamLeader() )
	{
		y -= 20;
		GetLeaderPic()->Render( x - 6, y );
	}
	//new code for new rank icons //mothannakh
	if (_pOwn->_Attr.get(ATTR_CHAOS_POINT) > 0 && _pOwn->IsPlayer())
	{
		SetIsShowEvil(1);
		SetEvilLevel(_pOwn->_Attr.get(ATTR_CHAOS_POINT));

	}
	else
	{
		SetIsShowEvil(0);
	}
	//end of the code 
	
	if( g_IsShowShop2 && _pOwn->IsShop() )
	{
		_RenderShop( _pOwn->getShopName(), x, y );

		static int nImageHeight = _ImgShop[0].GetHeight();
		y -= nImageHeight;
	}

    if ( _nShowTime <= 0 )
    {
#ifdef _LOG_NAME_                     //��ɫ����
		const int LINE_HEIGHT_STEP = 14;		//һ�еĸ߶�
		int iNameHeightStep = LINE_HEIGHT_STEP;	//��ɫ�������ĸ߶�

		// ������־��
        if( CCharacter::IsShowLogName )
        {
            int  nNameLength =  0 - CGuiFont::s_Font.GetWidth( _pOwn->getLogName() )/2;
            CGuiFont::s_Font.BRender( _pOwn->getLogName(), x +  nNameLength  , y - iNameHeightStep, _dwNameColor, COLOR_BLACK );
        }
        else if( _IsShowName )
        {
			// ����������
			if ( *(_pOwn->getGuildName()) )
			{
				iNameHeightStep += LINE_HEIGHT_STEP;	//��������һ��
				int iGuildNameHeightStep = LINE_HEIGHT_STEP;

				//������
				if ( *(_pOwn->getGuildName()) )
				{
					strncpy( s_szConsortiaNamePart[0], _pOwn->getGuildName(), NAME_LENGTH );					
				}
				else
				{
					strncpy( s_szConsortiaNamePart[0], "", NAME_LENGTH );					
				}
				//����������
				if ( *(_pOwn->getGuildMotto()) )
				{
					strncpy( s_szConsortiaNamePart[1], "(", strlen("(") );
					strncpy( s_szConsortiaNamePart[2], _pOwn->getGuildMotto(), NAME_LENGTH );
					strncpy( s_szConsortiaNamePart[3], ")", strlen(")") );
				}
				else
				{
					strncpy( s_szConsortiaNamePart[1], "", NAME_LENGTH );					
					strncpy( s_szConsortiaNamePart[2], "", NAME_LENGTH );					
					strncpy( s_szConsortiaNamePart[3], "", NAME_LENGTH );					
				}

				//ZeroMemory(s_szConsortiaName, sizeof(s_szConsortiaName));
				s_szConsortiaName[0] = '\0';
				for (int i(0); i<4; i++)
				{
					strncat( s_szConsortiaName, s_szConsortiaNamePart[i], NAME_LENGTH );
				}
				int nNameLength = 0 - CGuiFont::s_Font.GetWidth(s_szConsortiaName)/2;
				int iStartPosX = 0 - CGuiFont::s_Font.GetWidth(s_szConsortiaName)/2;

				// renderÿһ������
				int perm = (_pOwn->getGuildPermission()&emGldPermLeader);
				if( perm == emGldPermLeader){
					//GetGuildLeaderPic()->TintColour( 255, 0, 0 );
					GetGuildLeaderPic()->Render( x+ iStartPosX -13, y - iGuildNameHeightStep - 2 );
				}
			
				for (int i(0); i<4; i++)
				{
					CGuiFont::s_Font.BRender(s_szConsortiaNamePart[i], x + iStartPosX, y - iGuildNameHeightStep, _pOwn->getGuildColor(), COLOR_BLACK );
					iStartPosX += CGuiFont::s_Font.GetWidth(s_szConsortiaNamePart[i]);
				}

			}

			// ǰ׺��
			if ( *(_pOwn->GetPreName()) )
			{
				strncpy( s_sNamePart[PRENAME_SEP1_INDEX], "(", strlen("(") );
				strncpy( s_sNamePart[PRENAME_INDEX], _pOwn->GetPreName(), NAME_LENGTH );
				strncpy( s_sNamePart[PRENAME_SEP2_INDEX], ")", strlen(")") );
				s_dwNamePartsColors[PRENAME_INDEX][0] = _pOwn->GetPreColor();
			}
			else
			{
				strncpy( s_sNamePart[PRENAME_SEP1_INDEX], "", NAME_LENGTH );
				strncpy( s_sNamePart[PRENAME_INDEX], "", NAME_LENGTH );
				strncpy( s_sNamePart[PRENAME_SEP2_INDEX], "", NAME_LENGTH );
				s_dwNamePartsColors[PRENAME_INDEX][0] = 0;
			}

			//������
			if ( *(_pOwn->getSecondName()) )
			{
				strncpy( s_sNamePart[MOTTO_NAME_SEP1_INDEX], "(", strlen("(") );
				strncpy( s_sNamePart[MOTTO_NAME_INDEX], _pOwn->getSecondName(), NAME_LENGTH );
				strncpy( s_sNamePart[MOTTO_NAME_SEP2_INDEX], ")", strlen(")") );
			}
			else
			{
				strncpy( s_sNamePart[MOTTO_NAME_SEP1_INDEX], "", NAME_LENGTH );
				strncpy( s_sNamePart[MOTTO_NAME_INDEX], "", NAME_LENGTH );
				strncpy( s_sNamePart[MOTTO_NAME_SEP2_INDEX], "", NAME_LENGTH );
			}
			
			s_dwNamePartsColors[NAME_INDEX][0] = _dwNameColor;
			//@collerd necks 
			const stNetChangeChaPart part = _pOwn->GetPart();
			const int id = part.SLink[enumEQUIP_GLOWAPP].sID;
			switch (id) {
			case 15270: { s_dwNamePartsColors[NAME_INDEX][0] = COLOR_RED; } //RED
			break;
			case 15271: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFFFF8B1E; } //Orange
			break;
			case 15272: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFFFFF20C; } //Yellow
			break;
			case 15273: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFF27E206; } //Green
			break;
			case 15274: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFF15EFE4; } //Cyan
			break;
			case 15275: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFF4460FF; } //Navy Blue
			break;
			case 15276: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFF953FFF; } //Purple
			break;
			case 15277: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFFF04CFF; } //Pink
			break;
			case 15278: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFF000000; } //Black
			break;
			case 15279: { s_dwNamePartsColors[NAME_INDEX][0] = 0xFF9E9E9E; } //Gray
			break;
			}
			//@collerd necks end 
			//is enemy/friend mode@moth
			if (_pOwn->GetIsPK())
			{
				if (_pOwn->IsPlayer())
				{
					//this one color only other players green/red and keep your orginial color 
					isTeamMember || isGuildMember ? _pOwn->IsMainCha() ? s_dwNamePartsColors[NAME_INDEX][0] : s_dwNamePartsColors[NAME_INDEX][0] = COLOR_GREEN : s_dwNamePartsColors[NAME_INDEX][0] = COLOR_RED;
					//if had guild side/ team side //
					if (strcmp("ChaosIcicle", CGameApp::GetCurScene()->GetCurMapInfo()->szDataName) == 0 || strcmp("secretgarden", CGameApp::GetCurScene()->GetCurMapInfo()->szDataName) == 0)	//for teaming maps 
					{
						(_pOwn->getSideID() == 1) ? s_dwNamePartsColors[NAME_INDEX][0] = COLOR_GREEN : s_dwNamePartsColors[NAME_INDEX][0] = COLOR_RED;
					}
				}
			}
			//mod end @
			if (_pOwn->IsBoat())
			{
				strncpy( s_sNamePart[NAME_INDEX], _pOwn->getHumanName(), NAME_LENGTH );
				strncpy( s_sNamePart[BOAT_NAME_SEP1_INDEX], "[", strlen("[") );
				strncpy( s_sNamePart[BOAT_NAME_INDEX], _pOwn->getName(),  NAME_LENGTH);
				strncpy( s_sNamePart[BOAT_NAME_SEP2_INDEX], "]", strlen("]") );
			}
			else 
			{
				strncpy( s_sNamePart[BOAT_NAME_SEP1_INDEX], "", NAME_LENGTH );
				strncpy( s_sNamePart[BOAT_NAME_INDEX], "", NAME_LENGTH );
				strncpy( s_sNamePart[BOAT_NAME_SEP2_INDEX], "", NAME_LENGTH );
				strncpy( s_sNamePart[NAME_INDEX], _pOwn->getName(), NAME_LENGTH );

				if (_pOwn->IsMonster())
				{	//����ǹ���?�ҵȼ���������10������ʾ����ȼ�?

					// �����ȼ�
					static int nMainLevel(0);
					if( g_stUIBoat.GetHuman() )
					{
						nMainLevel = g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_LV);
					}

					// ����ȼ�?
					static int nMonsterLevel(0);
					nMonsterLevel = _pOwn->getGameAttr()->get(ATTR_LV);

					static char szBuf[NAME_LENGTH] = { 0 };
					if (nMonsterLevel - nMainLevel <= 10)
					{	//��ʾ�ȼ�
						sprintf(szBuf, "Lv%d %s", nMonsterLevel, _pOwn->getName());
					}
					else
					{
						sprintf(szBuf, "??? %s", _pOwn->getName());
					}
					strncpy( s_sNamePart[NAME_INDEX], szBuf, NAME_LENGTH );
				}
			}

			/*if (_pOwn->IsNPC())
			{
				s_dwNamePartsColors[NAME_INDEX][0] = 0xFF9BA1E9; // D3DXCOLOR(109,87,218,1); 
			}*/

			// �õ����ֵ��ܳ�
			s_szName[0] = '\0';
			//ZeroMemory( s_szName, sizeof(s_szName) );
			for (int i(0); i<NAME_PART_NUM; i++)
			{
				strncat( s_szName, s_sNamePart[i], NAME_LENGTH );
			}
			int nNameLength = 0 - CGuiFont::s_Font.GetWidth(s_szName)/2;
			int iStartPosX = 0 - CGuiFont::s_Font.GetWidth(s_szName)/2;

			if (_IsShowEvil)
			{
				if (_pImgEvil)
				{
					if (_sEvilLevel != EVIL_MAX)
					{
						_pImgEvil[_sEvilLevel].Render(x + iStartPosX - 16, y - iNameHeightStep);
					}
				}
			}

			// renderÿһ������
			
			
	
			for (int i(0); i<NAME_PART_NUM; i++)
			{
				if( s_dwNamePartsColors[i][1] )
				{
					CGuiFont::s_Font.BRender(s_sNamePart[i], x + iStartPosX, y - iNameHeightStep, s_dwNamePartsColors[i][0], s_dwNamePartsColors[i][1] );
				}
				else
				{
					CGuiFont::s_Font.Render(s_sNamePart[i], x + iStartPosX, y - iNameHeightStep, s_dwNamePartsColors[i][0] );
				}
				iStartPosX += CGuiFont::s_Font.GetWidth(s_sNamePart[i]);
			}

	   }
#else
        if( _IsShowName )  
        {
			if (_pOwn->IsBoat())	//��ʾ��ɫ��+����
			{
				static std::string sNameBuf;
				sNameBuf = _pOwn->getHumanName();
				sNameBuf += "--";
				sNameBuf += _pOwn->getName();

				int nNameLength = 0 - CGuiFont::s_Font.GetWidth(sNameBuf.c_str())/2;
				CGuiFont::s_Font.BRender(sNameBuf.c_str(), x + nNameLength, y - LINE_HEIGHT_STEP, _dwNameColor, COLOR_BLACK );
				if (_pOwn->IsShowSecondName())
				{
					if (strlen(_pOwn->getSecondName()) > 0)
					{
						std::string strSec = "(";
						strSec+=_pOwn->getSecondName(); 
						strSec+=")";
						CGuiFont::s_Font.Render( strSec.c_str(), x - nNameLength ,  y - LINE_HEIGHT_STEP, COLOR_BLACK);
					}
				}
			}
			else	//��ʾ��ɫ��
			{
				int  nNameLength =  0 - CGuiFont::s_Font.GetWidth( _pOwn->getName() )/2;
				CGuiFont::s_Font.BRender( _pOwn->getName(), x +  nNameLength ,   y - LINE_HEIGHT_STEP, _dwNameColor, COLOR_BLACK );
				if(_pOwn->IsShowSecondName())
				{
					if(strlen(_pOwn->getSecondName()) > 0)
					{
						std::string strSec = "(";
						strSec+=_pOwn->getSecondName(); 
						strSec+=")";
						CGuiFont::s_Font.Render( strSec.c_str(), x - nNameLength ,  y - LINE_HEIGHT_STEP, COLOR_BLACK);
					}
				}

			}	//end of if (_pOwn->IsBoat())
		}		
#endif
    }
    else if( _nShowTime > 0 )                        //����
    {
        _nShowTime--;   

		if ( _pObj )	
        {
            int  left     =  x- (int)( _fScale * _pObj->GetWidth()/2) ;
            int  nLine    = _pObj->GetLineNum();

            std::string str = _pObj->GetString();

            if ( str.find("#" )!= -1 )
            {
                if ( nLine ==1) 
                {
                    GetRender().FillFrame( left - 2, y -20 - 32 , 	left + (int)(_fScale * _pObj->GetWidth()) + 4, y-14, _dwBkgColor );
                    _pObj->RenderEx( left,   y-18- 32, 32,_fScale );
                }
                else if (nLine == 2)
                {
                    GetRender().FillFrame( left - 2, y -20 -44 , 	left + (int)(_fScale * _pObj->GetWidth()) + 4, y -14, _dwBkgColor );
                    _pObj->RenderEx( left,   y-18- 44, 24,_fScale );

                }
                else if ( nLine == 3)
                {
                    GetRender().FillFrame( left - 2, y -20 -76 , 	left + (int)(_fScale * _pObj->GetWidth()) + 4, y -14, _dwBkgColor );					  
                    _pObj->RenderEx( left, y-18- 76, 24,_fScale );
                }
            }
            else 
            {
                if ( nLine ==1) 
                {
                    GetRender().FillFrame( left - 2, y -20 -18 , 	left + (int)(_fScale * _pObj->GetWidth()) + 4, y -14, _dwBkgColor );
                    _pObj->RenderEx( left,   y-18- 18, 12,_fScale );
                }
                else if (nLine == 2)
                {
                    GetRender().FillFrame( left - 2, y -20 -36 , 	left + (int)(_fScale * _pObj->GetWidth()) + 4, y -14, _dwBkgColor );
                    _pObj->RenderEx( left,   y-18- 36, 12,_fScale );
                }
                else if ( nLine == 3)
                {
                    GetRender().FillFrame( left - 2, y -20 -54 , 	left + (int)(_fScale * _pObj->GetWidth()) + 4, y -14, _dwBkgColor );
                    _pObj->RenderEx( left,   y-18- 54, 12,_fScale );
                }		
            }
            _fScale +=0.2f;
            if ( _fScale >1.0f )
                _fScale = 1.0f ;

            if ( _nShowTime ==4  )
                _fScale = 0.8f;
            else if ( _nShowTime ==3  )
                _fScale = 0.6f;
            else if ( _nShowTime ==2  )
                _fScale = 0.4f;
            else if ( _nShowTime ==1  )
                _fScale = 0.2f;
        }
    }

#define FFQ 60
	if( _pCurFace && _dwFaceTime )   //����
	{
		if( _dwFaceTime==1 )
			_dwFaceTime = g_dwCurFrameTick;

		DWORD dwElaped = g_dwCurFrameTick - _dwFaceTime;
		y -= 20;
		if( dwElaped > _nMaxShowTime*FFQ/2 )
		{
			_dwFaceTime = 0;
		}
		else if( _pCurFace->GetMax() )
		{
			DWORD dwCurFrame = (dwElaped / (_nFaceFrequency*FFQ)) % _pCurFace->GetMax();
			_pCurFace->SetFrame(dwCurFrame);
			_pCurFace->Render( x-_pCurFace->GetWidth()/2, y - 20 );
		}
	}

#ifdef _LOG_NAME_                     //��ɫ����
    if( CCharacter::IsShowLogName )
    {
		CChaStateMgr* pState = _pOwn->GetStateMgr();
		int nCount = pState->GetSkillStateNum();
		for( int i=0; i<nCount; i++ )
		{
			y -= 20;
			CGuiFont::s_Font.Render( pState->GetSkillState(i)->szName, x, y, COLOR_WHITE );
		}
    }
#endif
}

bool CHeadSay::SetFaceID(unsigned int faceid) 
{
    if( faceid>= FACE_MAX ) return false;

    _nCurFaceFrame = 0;
    _nCurFaceCycle = 0;
    _nFaceTime = _nMaxShowTime;
	_dwFaceTime = 1;
    _pCurFace =  &_pImgFace[faceid];
	return true;
}

void CHeadSay::SetLifeNum(int num, int max)
{
    if( max==0 ) return;

    if(num < 0)   num = 0;
    if(num > max) num = max;
    if(_pImgLife)
    {
        _fLifeW = (float)num / (float)max;
		_fCurHp = num;
		_fMxHp = max;
		
    }
}

void CHeadSay::SetManaNum(int num, int max)
{
    if( max==0 ) return;

    if(num < 0)   num = 0;
    if(num > max) num = max;
    if(_pImgMana)
    {
        _fManaW = (float)num / (float)max;
		_fCurSp = num;
		_fMxSp = max;
		
    }
}

void CHeadSay::_RenderShop( const char* szShopName, int x, int y )
{
	static int nImageHeight = _ImgShop[0].GetHeight();
	static int nImageWidth = _ImgShop[0].GetWidth();
	y -= nImageHeight;

	static int nFontWidth = 0;
	nFontWidth = CGuiFont::s_Font.GetWidth( szShopName );	
	const auto IsVisited = visitedStalls.count(_pOwn->getAttachID()) >=1;
	const DWORD dwImgColor = IsVisited ? COLOR_GREEN : 0xffffffff;

	if( nFontWidth < 40 ) 
	{
		static int nFontTrueWidth = 0;
		nFontTrueWidth = nFontWidth;
		nFontWidth = 40;

		x -= _nShopFrameWidth + nFontWidth/2;

		_nShopX0 = x;
		_nShopY0 = y;
		_nShopX1 = x + nFontWidth + nImageWidth;
		_nShopY1 = y + nImageHeight;

		_ImgShop[0].Render( x, y, dwImgColor );
		x += _nShopFrameWidth;

		_ImgShop[2].Render( x + nFontWidth, y, dwImgColor );

		_ImgShop[1].SetScaleW( nFontWidth );
		_ImgShop[1].Render( x, y, dwImgColor );
		//CGuiFont::s_Font.BRender( szShopName, x + (nFontWidth - nFontTrueWidth) / 2, y+_nShopFontYOff, COLOR_BLACK, COLOR_WHITE );
		CGuiFont::s_Font.Render( szShopName, x + (nFontWidth - nFontTrueWidth) / 2, y+_nShopFontYOff, COLOR_BLACK );
		if (IsVisited) //render red icon if seen stall @mothannakh
		{
			CGuiPic seenicon;
			//or checkSTALL.png
			seenicon.LoadImage("texture/icon/check.png", 32, 32, 0, 0, 0, 0, 0);
			//seenicon.TintColour(209, 22, 13);	//color the image to red 
			seenicon.Render(x - 25, y - _nShopFontYOff - 2);	//render ingame 
		}
	}
	else
	{
		x -= _nShopFrameWidth + nFontWidth/2;

		_nShopX0 = x;
		_nShopY0 = y;
		_nShopX1 = x + nFontWidth + nImageWidth;
		_nShopY1 = y + nImageHeight;

		_ImgShop[0].Render( x, y, dwImgColor );
		x += _nShopFrameWidth;

		_ImgShop[2].Render( x + nFontWidth, y, dwImgColor );

		_ImgShop[1].SetScaleW( nFontWidth );
		_ImgShop[1].Render( x, y, dwImgColor );
		//CGuiFont::s_Font.BRender( szShopName, x, y+_nShopFontYOff, COLOR_BLACK, COLOR_WHITE );
		CGuiFont::s_Font.Render(szShopName, x, y + _nShopFontYOff, COLOR_BLACK);
		if (IsVisited) //render red icon if seen stall @mothannakh
		{
			CGuiPic seenicon;
			seenicon.LoadImage("texture/icon/check.png", 32, 32, 0, 0, 0, 0, 0);
			//seenicon.TintColour(209, 22, 13);	//color the image to red 
			seenicon.Render(x - 25, y - _nShopFontYOff - 2); //render ingame 
		}
	}
}

void CHeadSay::RenderText( const char* szShopName, int x, int y )
{
	static int nImageHeight = _ImgShop[0].GetHeight();
	static int nImageWidth = _ImgShop[0].GetWidth();
	// y -= nImageHeight;
	y -= 100;
	
	static int nFontWidth = 0;
	nFontWidth = CGuiFont::s_Font.GetWidth( szShopName );	
	const DWORD dwImgColor = 0xffffffff;

	if( nFontWidth < 40 ) 
	{
		static int nFontTrueWidth = 0;
		nFontTrueWidth = nFontWidth;
		nFontWidth = 40;

		x -= _nShopFrameWidth + nFontWidth/2;

		_ImgShop[0].Render( x, y, dwImgColor );
		x += _nShopFrameWidth;

		_ImgShop[2].Render( x + nFontWidth, y, dwImgColor );

		_ImgShop[1].SetScaleW( nFontWidth );
		_ImgShop[1].Render( x, y, dwImgColor );
		CGuiFont::s_Font.BRender( szShopName, x + (nFontWidth - nFontTrueWidth) / 2, y+_nShopFontYOff, COLOR_BLACK, COLOR_WHITE );
	}
	else
	{
		x -= _nShopFrameWidth + nFontWidth/2;

		_ImgShop[0].Render( x, y, dwImgColor );
		x += _nShopFrameWidth;

		_ImgShop[2].Render( x + nFontWidth, y, dwImgColor );

		_ImgShop[1].SetScaleW( nFontWidth );
		_ImgShop[1].Render( x, y, dwImgColor );
		CGuiFont::s_Font.BRender( szShopName, x, y+_nShopFontYOff, COLOR_BLACK, COLOR_WHITE );
	}
}

void CHeadSay::SetEvilLevel(short rank)
{
	if (rank > 0 && rank < 1000)
	{
		_sEvilLevel = 0;
	}
	else if (rank >= 1000 && rank < 2000)
	{
		_sEvilLevel = 1;
	}
	else if (rank >= 2000 && rank < 3000)
	{
		_sEvilLevel = 2;
	}
	else if (rank >= 3000 && rank < 4000)
	{
		_sEvilLevel = 3;
	}
	else if (rank >= 4000 && rank < 5000)
	{
		_sEvilLevel = 4;
	}
	else
	{
		_sEvilLevel = 5;
	}
}


void CHeadSay::SetIsShowEvil(bool bShow)
{
	_IsShowEvil = bShow;
}

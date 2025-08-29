#include "StdAfx.h"
#include "uistartform.h"
#include "uiform.h"
#include "uitextbutton.h"
#include "uiformmgr.h"
#include "uiprogressbar.h"
#include "uilabel.h"
#include "netchat.h"
#include "packetcmd.h"
#include "gameapp.h"
#include "LootFilter.h"
#include "uigrid.h"
#include "character.h"
#include "uiheadsay.h"
#include "uimenu.h"
#include "uilist.h"
#include "uigrid.h"
#include "UIGlobalVar.h"
#include "UIMisLogForm.h"
#include "UICozeForm.h"
#include "NetGuild.h"
#include "uiguildmgr.h"
#include "worldscene.h"
#include "uititle.h"
#include "uiboxform.h"
#include "shipfactory.h"
#include "uiboatform.h"
#include "arearecord.h"
#include "isskilluse.h"
#include "ui3dcompent.h"
#include "smallmap.h"
#include "mapset.h"
#include "uiequipform.h"
#include "uiTradeForm.h"
#include "uiFindTeamForm.h"
#include "uistoreform.h"
#include "uidoublepwdform.h"
#include "uiitemcommand.h"
#include "uiminimapform.h"
#include "uibankform.h"
#include "uiboothform.h"
#include "uitradeform.h"
#include "UIChat.h"
#include "UITeam.h"
#include "NPCHelper.h"//add by alfred.shi 20080710

// Add by lark.li 20080811 begin
#include "UITeam.h"
// End
#include "NPCHelper.h"

using namespace GUI;


static CForm*		frmSelectOriginRelive	= nullptr;

//---------------------------------------------------------------------------
// class CStartMgr
//---------------------------------------------------------------------------
CMenu*	      CStartMgr::mainMouseRight = nullptr;
CTextButton*  CStartMgr::btnQQ			= nullptr;
CCharacter2D* CStartMgr::pMainCha		= nullptr;
CCharacter2D* CStartMgr::pTarget		= nullptr;
C3DCompent*   CStartMgr::p3D 			= nullptr;

bool CanRender = true ;
bool RenderOnce = false ;

static char szBuf[32] = { 0 };

void CStartMgr::SetTargetInfo( CCharacter* pTargetCha ){
	MonstetDropID = pTargetCha->GetDefaultChaInfo()->lID; //mothannakh monster id for drop rate show 
	if (pTargetCha->GetDefaultChaInfo()->chCtrlType !=2 )	
	{
		char chaLv[5];
		sprintf(chaLv,"%d",pTargetCha->getLv());
		labTargetInfoName->SetCaption(pTargetCha->getName());
		targetInfoID = pTargetCha->getHumanID();
		RefreshTargetLifeNum(pTargetCha->getHP(),pTargetCha->getHPMax());
		labTargetLevel->SetCaption(chaLv);
		frmTargetInfo->Show();
		
		RefreshTargetModel(pTargetCha);
		
		if (MonstetDropID > 4)
		{
			
			if(RenderOnce || targetInfoID!= MonstetDropID) MonsterDrop(MonstetDropID,pTargetCha);	//call the function 
			btnOpenDrop->SetIsShow(true);
			CanRender= false;
			//setup the name
			if (lablDropN)
			{
				lablDropN->SetCaption(pTargetCha->getName());
				lablDropN->SetIsShow(true);
			}	
		
		}
		else {
			btnOpenDrop->SetIsShow(false);
			//btnMobInfo->SetIsShow(false);
			CanRender = true;
		}
	}
}

void CStartMgr::_FilterBtnClick(CGuiData *pSender, int x, int y, DWORD key, CItemRow* currentRow) {
	CItemObj* itemObj = currentRow->GetIndex(2);
	CItemFilterItem* item = ((CItemFilterItem*)itemObj);
	int itemId = item->getItemID();
	bool isItemHidden = item->getHiddenState();
	if( !CGameApp::GetCurScene() ) return;

	CSceneItem* pItem = CGameApp::GetCurScene()->FilterItemsByItemID(itemId, !isItemHidden);
	item->setHiddenState(!isItemHidden);
	item->Render();
}


//monster drop list start //
void CStartMgr::MonsterDrop(int mobid ,CCharacter* pTargetCha) { //mothannakh mobs drop
	RenderOnce = false ;
	CChaRecord* pInfo = GetChaRecordInfo(mobid);
	lstItemDrop->GetList()->GetItems()->Clear();
	for (int n = 0; n < 10; n++)
	{
		const int DropID = pInfo->lItem[n][0];	//drop id 
		if (DropID > 0)
		{
			CItemRecord* ItemInfo = GetItemRecordInfo(DropID);	//iteminfo   
		   if (!ItemInfo) {	// sexy fix of nullptr items 
                continue;
            }			
			const int ServerDrop = 1;
			int ampsBoost = 1;
			float Luckypet = 1;
			//amps rates 
			if (CGameScene::GetMainCha()->GetStateMgr()->GetStateLv(49) == 3)
			{
				ampsBoost = 2;		
			}
			if (CGameScene::GetMainCha()->GetStateMgr()->GetStateLv(49) == 5)
			{
				ampsBoost = 3;	
			}	
			if (CGameScene::GetMainCha()->GetStateMgr()->HasSkillState(173) || CGameScene::GetMainCha()->GetStateMgr()->HasSkillState(174) > 0)//lucky pet poss 
			{
				SItemGrid item = CGameScene::GetMainCha()->GetPart().SLink[enumEQUIP_FAIRY];
				int nLevel = item.GetAttr(ITEMATTR_VAL_STR)
					+ item.GetAttr(ITEMATTR_VAL_AGI)
					+ item.GetAttr(ITEMATTR_VAL_DEX)
					+ item.GetAttr(ITEMATTR_VAL_CON)
					+ item.GetAttr(ITEMATTR_VAL_STA);
				if(CGameScene::GetMainCha()->GetStateMgr()->HasSkillState(173))
					Luckypet = 1+( 0.02 * nLevel);
				else
					Luckypet = 1+(0.01 * nLevel);
			}
			//amps ends
			const int val = pInfo->lItem[n][1];	//Drop rate value "const lets keep it int only to change it for -1 of quest ids @mothannakh"
			if (pTargetCha->GetDefaultChaInfo()->chCtrlType == 7 || pTargetCha->GetDefaultChaInfo()->chCtrlType == 6)	//disable amps on resources 
			{
				ampsBoost = 1;
				Luckypet = 1;
			}
			float DropCalculation = (100.f / val) * ServerDrop * ampsBoost * Luckypet * 100;
			DropRete = min(DropCalculation, 100);
			float OrginalDrop = min( ((100.f / val) * ServerDrop * 100), 100);
			//list
			CItemRow* row = lstItemDrop->GetList()->NewItem();
			bool isItemFiltered = g_lootFilter->HasFilteredItem(DropID);
			CItemFilterItem* item = new CItemFilterItem(DropID, isItemFiltered);
			row->evtListMouseDown = _FilterBtnClick;

			row->SetIndex(0, new CItemCommand(ItemInfo));		//the icon
			row->SetIndex(1, new CItemInfoItem(DropID, OrginalDrop, DropRete));	//name & rate //
			row->SetIndex(2, item);
			//lets get exp from monster record 
		}
		if (labMobExp && labMobHP && labMobLevel && labMobSpeed && labMobPR && labMobDodge && labMobHitRate && labMobDef && labMobAttack )
		{
				char  pszMob[256] = { 0 };	//buffer 
				//btnMobInfo->SetIsShow(true); //show the form 
				//hp 
				sprintf(pszMob, "%d", pTargetCha->getHPMax());
				labMobHP->SetCaption((const char*)pszMob);		
				//set sp 
				//sprintf(pszMob, "%d", (pTargetCha->GetMaxSP() !=0?pTargetCha->GetMaxSP():pInfo->lMxSp ));
				//labMobSP->SetCaption((const char*)pszMob);				
				//exp
				sprintf(pszMob, "%d", pInfo->lCExp);
				labMobExp->SetCaption((const char*)pszMob);
				//level
				sprintf(pszMob, "%d", (pTargetCha->getLv() !=0?pTargetCha->getLv():pInfo->lLv ));
				labMobLevel->SetCaption((const char*)pszMob);
				//movement speed
				sprintf(pszMob, "%d", (pTargetCha->getMoveSpeed() !=0?pTargetCha->getMoveSpeed():pInfo->lMSpd ));
				labMobSpeed->SetCaption((const char*)pszMob);
				//dodge 
				sprintf(pszMob, "%d", (pTargetCha->GetDodge() !=0?pTargetCha->GetDodge():pInfo->lFlee ));
				labMobDodge->SetCaption((const char*)pszMob);
				//hit rate  
				sprintf(pszMob, "%d", (pTargetCha->GetHitRate() !=0?pTargetCha->GetHitRate():pInfo->lHit ));
				labMobHitRate->SetCaption((const char*)pszMob);
				//def  
				sprintf(pszMob, "%d", (pTargetCha->GetDef() !=0?pTargetCha->GetDef():pInfo->lDef ));
				labMobDef->SetCaption((const char*)pszMob);
				//pr  
				sprintf(pszMob, "%d", (pTargetCha->GetPR() !=0?pTargetCha->GetPR():pInfo->lPDef ));
				labMobPR->SetCaption((const char*)pszMob);
				//min/max attack 
				sprintf(pszMob, "%d / %d ", (pTargetCha->GetMinAttk()!=0?pTargetCha->GetMinAttk():pInfo->lMnAtk),(pTargetCha->GetMaxAttk()!=0?pTargetCha->GetMaxAttk():pInfo->lMxAtk));
				labMobAttack->SetCaption((const char*)pszMob);
				//attack speed start
				if (labMobASpeed)
				{
					sprintf(pszMob, "%d", (pTargetCha->getAttackSpeed() !=0? 100000 /pTargetCha->getAttackSpeed(): 0));
					labMobASpeed->SetCaption((const char*)pszMob);					
				}
				//attack speed end 					
		}	//monster info buffer end 
		
	}	//for end 
	
}
//monster drop list end
void CStartMgr::RefreshTargetModel(CCharacter* pTargetCha){
	if( pTarget ){
		static stNetTeamChaPart stTeamPart;		
		stTeamPart.Convert( pTargetCha->GetPart() );	
		
		if(!pTargetCha->IsPlayer()){
			pTarget->LoadCha(pTargetCha->getMobID());
		}
		
		pTarget->UpdataFace( stTeamPart );
	}
}

void CStartMgr::UpdateBackDrop(){
	CCharacter* pMain = CGameScene::GetMainCha();
	int nArea = CGameApp::GetCurScene()->GetTerrain()->GetTile( pMain->GetCurX()/100, pMain->GetCurY()/100 )->getIsland();
	CWorldScene* world = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	
	if(nArea ==  world->GetOldMainChaInArea()){
		return;
	}
	
	world->SetOldMainChaInArea(nArea);
	
	char buf[64];
	
	if( nArea){
		sprintf(buf,"texture/ui/custom/npcBackdrop/%d.tga",nArea);
	}else{
		sprintf(buf,"texture/ui/custom/npcBackdrop/sea.tga"); 
	}
	CCompent* imgBackDropPlayer = dynamic_cast<CCompent*>(g_stUIStart.frmDetail->Find("imgBackDropPlayer"));
	CCompent* teamBackDrops[4];
	
	for(int i = 0;i<4;i++){
		char formName[32];
		char imgName[32];
		sprintf(formName,"frmTeamMenber%d",i+1);
		sprintf(imgName,"imgBackDropTeam%d",i+1);
		teamBackDrops[i] = dynamic_cast<CCompent*>(g_stUIStart._FindForm(formName)->Find(imgName));
	}
	
		 //https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c/25450408#25450408 # 3
	if(GetFileAttributes(buf) == INVALID_FILE_ATTRIBUTES){
		imgBackDropPlayer->GetImage()->LoadImage("texture/ui/custom/npcBackdrop/0.tga",55, 44, 0, 0, 0);
		for(int i = 0;i<4;i++){
			teamBackDrops[i]->GetImage()->LoadImage("texture/ui/custom/npcBackdrop/0.tga",55, 44, 0, 0, 0);
		}
	}else{
		imgBackDropPlayer->GetImage()->LoadImage(buf,55, 44, 0, 0, 0);
		for(int i = 0;i<4;i++){
			teamBackDrops[i]->GetImage()->LoadImage(buf,55, 44, 0, 0, 0);
		}
	}
	
	
}


void CStartMgr::RemoveTarget(){
	frmTargetInfo->Hide();
	targetInfoID = 0;
	MonstetDropID = 0;
	CanRender = true;
	frmMobInfoDrop->SetIsShow(false);
	//frmMobInfo->SetIsShow(false);
}


void CStartMgr::RefreshTargetLifeNum( long num, long max ){
	if(num < 0){
		num = 0;
	}   
    if(num > max) {
		num = max;
	}
	if (max == 0){
		max = 1;
		num = 0;
	}
	proTargetInfoHP->SetRange(0.0f, (float)max);
	proTargetInfoHP->SetPosition((float)num);
	if(num == 0){
		RemoveTarget();
	}
}

void CStartMgr::_TargetRenderEvent(C3DCompent *pSender, int x, int y)
{
	pTarget->Render();
}

bool CStartMgr::Init()
{
	_IsNewer = false;
	_IsCanTeam = true;

	
	
	//g_stUIStart
	{
		frmTargetInfo = _FindForm("frmTargetInfo");
		if( frmTargetInfo ) {
			frmTargetInfo->Refresh();
			frmTargetInfo->evtMouseDragEnd = _OnDragEnd;
			proTargetInfoHP = dynamic_cast<CProgressBar *>( frmTargetInfo->Find("frmTargetInfoHP") );
			proTargetInfoHP->SetPosition(100.0f);
			
			labTargetInfoName = dynamic_cast<CLabel *>( frmTargetInfo->Find("frmTargetInfoName") );
			
			labTargetLevel = dynamic_cast<CLabel *>( frmTargetInfo->Find("labTargetLv"));
			
			p3D = dynamic_cast<C3DCompent*>( frmTargetInfo->Find("d3dTarget") );
			//drop rate button mothannakh//
			btnOpenDrop = dynamic_cast<CTextButton*>(frmTargetInfo->Find("btnOpenDrop"));
			if (!btnOpenDrop)   		return Error(g_oLangRec.GetString(45), frmTargetInfo->GetName(), "btnOpenDrop");
			frmTargetInfo->evtEntrustMouseEvent = _evtStartFormMouseEvent;
			//drop rate button end 
			//monster info button start 
			//btnMobInfo = dynamic_cast<CTextButton*>(frmTargetInfo->Find("btnMobInfo"));
			//if (!btnMobInfo)   		return Error(g_oLangRec.GetString(45), frmTargetInfo->GetName(), "btnMobInfo");
			frmTargetInfo->evtEntrustMouseEvent = _evtStartFormMouseEvent;			
			//monster info button end 

			p3D->SetRenderEvent( _TargetRenderEvent );
			
			RECT rt;
			rt.left = p3D->GetX();
			rt.right = p3D->GetX2();
			rt.top = p3D->GetY();
			rt.bottom = p3D->GetY2();
			
			pTarget = new CCharacter2D;
			pTarget->Create( rt );
			frmTargetInfo->evtMouseDragEnd = _OnDragEnd;
		}
	}
				
	{
		frmDetail = _FindForm("frmDetail");
		if( frmDetail ) 
		{
			frmDetail->Refresh();
			proMainHP = dynamic_cast<CProgressBar *>( frmDetail->Find("proMainHP1") );
			if( !proMainHP ) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "proMainHP1" );   
			proMainHP->SetPosition(0.0f);

			proMainSP = dynamic_cast<CProgressBar *>( frmDetail->Find("proMainSP") );
			if( !proMainSP ) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "proMainSP" );   
			proMainSP->SetPosition(0.0f);

			proMainExp = dynamic_cast<CProgressBar *>( frmDetail->Find("proMainEXP") );
			if( !proMainExp ) 
			{
				Error( g_oLangRec.GetString(473), frmDetail->GetName(), "proMainEXP" );   
			}
			else
			{
				proMainExp->SetPosition(0.0f);
			}
		
		/*	proMainLifeExp = dynamic_cast<CProgressBar *>( frmDetail->Find("proMainLifeExp") );
			if( !proMainLifeExp ) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "proMainLifeExp" );   
			proMainLifeExp->SetPosition(0.0f);*/



			labMainName = dynamic_cast<CLabel *>( frmDetail->Find("labMainID") );
			if ( !labMainName ) Error( g_oLangRec.GetString(473), frmDetail->GetName(), "labMainID");

			labMainLevel = dynamic_cast<CLabel *>( frmDetail->Find("labMainLv"));
			if ( !labMainLevel ) Error( g_oLangRec.GetString(473), frmDetail->GetName(), "labMainLv");

			imgLeader = dynamic_cast<CImage*>( frmDetail->Find("imgLeader"));
			if ( !imgLeader ) Error( g_oLangRec.GetString(473), frmDetail->GetName(), "imgLeader");

			C3DCompent* d3dSelfDown = dynamic_cast<C3DCompent*>( frmDetail->Find("d3dSelfDown") );
			if (!d3dSelfDown) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "d3dSelfDown" );
			//d3dSelfDown->SetRenderEvent( _MainChaRenderEvent );
			d3dSelfDown->evtMouseDown = _evtSelfMouseDown;
			d3dSelfDown->SetMouseAction( enumMA_Skill );
			

			C3DCompent* p3D = dynamic_cast<C3DCompent*>( frmDetail->Find("d3dSelf") );
			if (!p3D) return Error( g_oLangRec.GetString(473), frmDetail->GetName(), "d3dSelf" );
            
			p3D->SetRenderEvent( _MainChaRenderEvent );
			//p3D->evtMouseDown = _evtSelfMouseDown;
			//p3D->SetMouseAction( enumMA_Skill );	
            
			RECT rt;
			rt.left = p3D->GetX();
			rt.right = p3D->GetX2();
			rt.top = p3D->GetY();
			rt.bottom = p3D->GetY2();
            
			pMainCha = new CCharacter2D;
			pMainCha->Create( rt );
		}

		// ���Լ�ͷ����Ҽ��˵�??
		mnuSelf = CMenu::FindMenu("selfMouseRight");
		if (!mnuSelf)  return Error( g_oLangRec.GetString(45), frmMain800->GetName(), "selfMouseRight" );
		mnuSelf->evtListMouseDown=_OnSelfMenu;
	}

	// frmMain800����
	{
		frmMain800 = _FindForm("frmMain800");
		frmMain800->evtEntrustMouseEvent = _evtTaskMouseEvent;

		tlCity = dynamic_cast<CTitle*>(frmMain800->Find("tlCity"));
		tlField = dynamic_cast<CTitle*>(frmMain800->Find("tlField"));

		//grdHeart = dynamic_cast<CGrid*>(frmMain800->Find("grdHeart"));
		//if( !grdHeart )	return Error( "msgui.clu����<%s>���Ҳ����ؼ�<%s>", frmMain800->GetName(), "grdHeart" );
		//grdHeart->evtSelectChange = _evtChaHeartChange;

		//grdAction = dynamic_cast<CGrid*>(frmMain800->Find("grdAction"));
		//if( !grdAction ) return Error( "msgui.clu����<%s>���Ҳ����ؼ�<%s>", frmMain800->GetName(), "grdAction" );
		//grdAction->evtSelectChange = _evtChaActionChange;
	}



	// ��ɫ����
	//proMainHP1 =  dynamic_cast<CProgressBar *> ( frmMain800->Find("proMainHP1") );
	//if( !proMainHP1 ) return Error( "msgui.clu����<%s>���Ҳ����ؼ�<%s>", frmMain800->GetName(), "proMainHP1" );   
	//proMainHP1->SetPosition(10.0f );
	//
	//proMainHP2 =  dynamic_cast<CProgressBar *> ( frmMain800->Find("proMainHP2") );
	//if( !proMainHP2 ) return Error( "msgui.clu����<%s>���Ҳ����ؼ�<%s>", frmMain800->GetName(), "proMainHP2" );   
	//proMainHP2->SetPosition(10.0f );

	//proMainHP3 =  dynamic_cast<CProgressBar *> ( frmMain800->Find("proMainHP3") );
	//if( !proMainHP3 ) return Error( "msgui.clu����<%s>���Ҳ����ؼ�<%s>", frmMain800->GetName(), "proMainHP3" );   
	//proMainHP3->SetPosition(10.0f );

	//proMainSP =  dynamic_cast<CProgressBar *> ( frmMain800->Find("proMainSP") );
	//if( !proMainSP ) return Error( "msgui.clu����<%s>���Ҳ����ؼ�<%s>", frmMain800->GetName(), "proMainSP" );
 //  	proMainSP->SetPosition (10.0f );

	//_pShowExp = dynamic_cast<CLabel*>(frmMain800->Find( "labMainEXP" ) );
	//_pShowLevel = dynamic_cast<CLabel*>(frmMain800->Find( "labMainLV" ) );

	//frmMainFun����
	{
		frmMainFun = _FindForm("frmMainFun");
		if(!frmMainFun) return false;

		frmMainFun->evtEntrustMouseEvent = _evtStartFormMouseEvent;

		// QQ
		/*FORM_CONTROL_LOADING_CHECK(btnQQ,frmMainFun,CTextButton,"msgui.clu","btnQQ");
		btnQQ->GetImage()->LoadImage("texture/ui/main800.tga",32,32,4,136,201);*/

		// ��������������˸
		btnLevelUpHelp = dynamic_cast<CTextButton*>(frmMainFun->Find("btnLevelUpHelp"));
		//FORM_CONTROL_LOADING_CHECK(btnLevelUpHelp, frmMainFun, CTextButton, "msgui.clu", "btnLevelUpHelp");
		if(btnLevelUpHelp)	btnLevelUpHelp->SetFlashCycle();
	}

	
	// �Ҽ��˵�
	mainMouseRight=CMenu::FindMenu("mainMouseRight");
	if (!mainMouseRight)
	{
		return Error( g_oLangRec.GetString(45), frmMain800->GetName(), "mainMouseRight" );
	}
	mainMouseRight->evtListMouseDown=_evtPopMenu;

	// ���Ǹ������??
	frmMainChaRelive = _FindForm("frmRelive");
	if(!frmMainChaRelive) return false;
	frmMainChaRelive->evtEntrustMouseEvent = _evtReliveFormMouseEvent;

	//��ֻ����ʱ�Ľ���
	frmShipSail = _FindForm("frmShipsail");  //���Ա��� 
	if( !frmShipSail )		return false;

	labCanonShow	 =	(CLabelEx*)frmShipSail->Find("labCanonShow1");
	labSailorShow	 =	(CLabelEx*)frmShipSail->Find("labSailorShow1");
	labLevelShow	 =  (CLabelEx*)frmShipSail->Find("labLvship");
	labExpShow		 =  (CLabelEx*)frmShipSail->Find("labExpship");

	proSailor		 =	(CProgressBar*)frmShipSail->Find("proSailor");				//�;ù�����
	proCanon		 =	(CProgressBar*)frmShipSail->Find("proCanon");				//����������
	frmShipSail->SetIsShow(false);
	
	//	Modify by alfred.shi 20080828
	CTextButton* btn1 = (CTextButton*)frmShipSail->Find("btnShip");
	if (!btn1) return false;
	btn1->evtMouseClick = _evtShowBoatAttr;

	// �Զ�����
	frmFollow = _FindForm("frmFollow");
	if( !frmFollow ) return false;

	labFollow = dynamic_cast<CLabel*>( frmFollow->Find("labFollow") );
	if( !labFollow ) return Error( g_oLangRec.GetString(45), frmFollow->GetName(), "labFollow" );

	// �������??
	frmMainPet = _FindForm("frmMainPet");
	if( !frmMainPet )		return false;

	frmMainPet->Hide();

	labPetLv = dynamic_cast<CLabel*>( frmMainPet->Find("labPetLv") );
	if( !labPetLv ) return Error( g_oLangRec.GetString(45), frmMainPet->GetName(), "labPetLv" );

	imgPetHead = dynamic_cast<CImage*>( frmMainPet->Find("imgPetHead") );
	if( !imgPetHead ) return Error( g_oLangRec.GetString(45), frmMainPet->GetName(), "imgPetHead" );

	proPetHP = dynamic_cast<CProgressBar*>( frmMainPet->Find("proPetHP") );
	if( !proPetHP ) return Error( g_oLangRec.GetString(45), frmMainPet->GetName(), "proPetHP" );

	proPetSP = dynamic_cast<CProgressBar*>( frmMainPet->Find("proPetSP") );
	if( !proPetSP ) return Error( g_oLangRec.GetString(45), frmMainPet->GetName(), "proPetSP" );

	//
	// ���ְ�������
	//
	frmHelpSystem = CFormMgr::s_Mgr.Find("frmHelpSystem");
	if( !frmHelpSystem ) return Error( g_oLangRec.GetString(45), "frmHelpSystem", "frmHelpSystem" );

	lstHelpList   = dynamic_cast<CList*>(frmHelpSystem->Find("lstHelpList"));
	if(! lstHelpList) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), "lstHelpList" );
	lstHelpList->evtSelectChange =  _evtHelpListChange;

	frmHelpSystem->evtEntrustMouseEvent = _evtStartFormMouseEvent;

	char szName[64] = {0};
	for(int i = 0; i < HELP_PICTURE_COUNT; ++i)
	{
		sprintf(szName, "imgHelpShow%d_1", i + 1);
		imgHelpShow1[i] = dynamic_cast<CImage*>(frmHelpSystem->Find(szName));
		if(! imgHelpShow1[i]) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), szName );

		sprintf(szName, "imgHelpShow%d_2", i + 1);
		imgHelpShow2[i] = dynamic_cast<CImage*>(frmHelpSystem->Find(szName));
		if(! imgHelpShow2[i]) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), szName );

		sprintf(szName, "imgHelpShow%d_3", i + 1);
		imgHelpShow3[i] = dynamic_cast<CImage*>(frmHelpSystem->Find(szName));
		if(! imgHelpShow3[i]) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), szName );

		sprintf(szName, "imgHelpShow%d_4", i + 1);
		imgHelpShow4[i] = dynamic_cast<CImage*>(frmHelpSystem->Find(szName));
		if(! imgHelpShow4[i]) return Error( g_oLangRec.GetString(45), frmHelpSystem->GetName(), szName );

		if(i > 0)
		{
			imgHelpShow1[i]->SetIsShow(false);
			imgHelpShow2[i]->SetIsShow(false);
			imgHelpShow3[i]->SetIsShow(false);
			imgHelpShow4[i]->SetIsShow(false);
		}
		else
		{
			imgHelpShow1[i]->SetIsShow(true);
			imgHelpShow2[i]->SetIsShow(true);
			imgHelpShow3[i]->SetIsShow(true);
			imgHelpShow4[i]->SetIsShow(true);
		}
	}

	//
	// ������ť����
	//
	frmBag = CFormMgr::s_Mgr.Find("frmBag");
	if(! frmBag) return Error(g_oLangRec.GetString(45), "frmBag", "frmBag");
	frmBag->evtEntrustMouseEvent = _evtStartFormMouseEvent;

	//
	// �罻��ť���??
	//
	frmSociliaty = CFormMgr::s_Mgr.Find("frmSociliaty");
	if(! frmSociliaty) return Error(g_oLangRec.GetString(45), "frmSociliaty", "frmSociliaty");
	frmSociliaty->evtEntrustMouseEvent = _evtStartFormMouseEvent;
	//MonsterDrop Form //mothannakh
	frmMobInfoDrop = CFormMgr::s_Mgr.Find("frmMobInfoDrop");
	if (!frmMobInfoDrop) return Error(g_oLangRec.GetString(45), "frmMobInfoDrop", "frmMobInfoDrop");
	//monster drop info  form 
	//frmMobInfo = CFormMgr::s_Mgr.Find("frmMobInfo");
	//if (!frmMobInfo) return Error(g_oLangRec.GetString(45), "frmMobInfo", "frmMobInfo");	
	//droplist
	lstItemDrop = dynamic_cast<CListView*>(frmMobInfoDrop->Find("lstItemDrop"));
	if (!lstItemDrop) return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "lstItemDrop");
	//drop name lable
	lablDropN = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("lablDropN"));
	if (!lablDropN)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "lablDropN");
	//lables for monster info 
	//new lables start 
	labMobHP = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobHP"));
	if (!labMobHP)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobHP");
	
	labMobExp = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobExp"));
	if (!labMobExp)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobExp");

	labMobLevel = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobLevel"));
	if (!labMobLevel)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobLevel");

	labMobSpeed = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobSpeed"));
	if (!labMobSpeed)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobSpeed");
	
	//labMobSP = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobSP"));
	//if (!labMobSP)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobSP");	
	
	labMobDodge = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobDodge"));
	if (!labMobDodge)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobDodge");	
	
	labMobHitRate = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobHitRate"));
	if (!labMobHitRate)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobHitRate");	

	labMobDef = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobDef"));
	if (!labMobDef)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobDef");		
	
	labMobPR = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobPR"));
	if (!labMobPR)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobPR");		
	
	labMobASpeed = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobASpeed"));
	if (!labMobASpeed)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobASpeed");			
	
	labMobAttack = dynamic_cast<CLabelEx*>(frmMobInfoDrop->Find("labMobAttack"));
	if (!labMobAttack)   	return Error(g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "labMobAttack");		
		//new lables end 
	
	imgSlots = dynamic_cast<CImage*>(frmMobInfoDrop->Find("imgSlots"));
	if(! imgSlots)return Error( g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "imgSlots" );

	imgLines = dynamic_cast<CImage*>(frmMobInfoDrop->Find("imgLines"));
	if(! imgLines)return Error( g_oLangRec.GetString(45), frmMobInfoDrop->GetName(), "imgLines" );

	strMapName = "";
	
	frmNpcShow = CFormMgr::s_Mgr.Find("frmNpcShow");
	if(! frmNpcShow) return Error(g_oLangRec.GetString(45) , "frmNpcShow", "frmNpcShow");

	lstNpcList = dynamic_cast<CList*>(frmNpcShow->Find("lstNpcList"));
	assert(lstNpcList != nullptr);	
	lstMonsterList = dynamic_cast<CList*>(frmNpcShow->Find("lstMonsterList"));
	assert(lstMonsterList != nullptr);

	chkID  =( CCheckBox *)frmNpcShow->Find( "chkID" );
	chkID->SetIsChecked(true);

	lstNpcList->evtSelectChange =  _evtNPCListChange;
	lstMonsterList->evtSelectChange =  _evtNPCListChange;
	
	lstCurrList = lstNpcList;

	listPage = dynamic_cast<CPage*>(frmNpcShow->Find("pgeSkill"));
	assert(listPage != nullptr);
	listPage->evtSelectPage = _evtPageIndexChange;

	return true;
}

void CStartMgr::ShowNPCHelper(const char * mapName,bool isShow)
{
	std::string strCurMap = g_pGameApp->GetCurScene()->GetTerrainName();

    if(strCurMap == "garner")
		strCurMap = g_oLangRec.GetString(56);
    else if(strCurMap == "magicsea")
        strCurMap = g_oLangRec.GetString(57);
	else if(strCurMap == "darkblue")
		strCurMap = g_oLangRec.GetString(58);
	else  if(strCurMap == "winterland")
		strCurMap = "Winter Land";
	else  if(strCurMap == "jialebi")
		strCurMap = "Carribean";


	strMapName =  strCurMap.c_str();
	{
		CListItems* items = lstCurrList->GetItems();
		items->Clear();
		lstCurrList->Refresh();

		int nTotalIndex = NPCHelper::I()->GetLastID() + 1;
		for(int i = 0; i < nTotalIndex ; ++ i)
		{
			NPCData * p = GetNPCDataInfo(i);
			if(p && strcmp(p->szMapName,strMapName) == 0) 
			{
				char buff[1024];
				std::string strName(p->szName);
				while (strName.length() < 18)
				{
					strName += " ";
				}
				std::string strAreaName = std::string("<") + std::string(p->szArea) + std::string(">");
				while (strAreaName.length() < 16)
				{
					strAreaName += " ";
				}
				//_snprintf_s( buff, _countof( buff ), _TRUNCATE, "%s%s<%d,%d>",strName.c_str(),strAreaName.c_str(),p->dwxPos0,p->dwyPos0);
				sprintf(buff, "%s%s<%d,%d>", strName.c_str(),strAreaName.c_str(),p->dwxPos0,p->dwyPos0);
				lstCurrList->Add(buff);
				lstCurrList->Refresh();
			}
		}
	}

	frmNpcShow->SetIsShow(isShow);
}
void CStartMgr::ShowShipSailForm(bool isShow /* = true  */)
{
	UpdateShipSailForm();
	frmShipSail->SetIsShow(isShow);
}
void CStartMgr::UpdateShipSailForm()
{
	CCharacter* pMain = CGameScene::GetMainCha();
	if(!pMain || !pMain->IsBoat())
		return;

	SGameAttr* pAttr = pMain->getGameAttr();

	char buf[128];

	sprintf(buf, "%d/%d", pAttr->get(ATTR_HP), pAttr->get(ATTR_MXHP));
	labSailorShow->SetCaption(buf);
	proSailor->SetRange((float)0, (float)(pAttr->get(ATTR_MXHP)));
	proSailor->SetPosition((float)(pAttr->get(ATTR_HP)));

	sprintf(buf, "%d/%d", pAttr->get(ATTR_SP), pAttr->get(ATTR_MXSP));
	labCanonShow->SetCaption(buf);
	proCanon->SetRange((float)0, (float)(pAttr->get(ATTR_MXSP)));
	proCanon->SetPosition((float)(pAttr->get(ATTR_SP)));

	labLevelShow->SetCaption(itoa(pAttr->get(ATTR_LV), buf, 10));
	labExpShow->SetCaption(itoa(pAttr->get(ATTR_CEXP), buf, 10));

}

void CStartMgr::End()
{
	//delete pMainCha;
	//pMainCha = nullptr;
	SAFE_DELETE(pMainCha); // UI��������
	SAFE_DELETE(pTarget); // UI��������
}

void CStartMgr::ShowQueryReliveForm( int nType, const char* str )
{
	stSelectBox* pOriginRelive = g_stUIBox.ShowSelectBox( _evtOriginReliveFormMouseEvent, str, false );
	frmSelectOriginRelive = pOriginRelive->frmDialog;
	frmSelectOriginRelive->nTag = nType;
}


void CStartMgr::_evtPageIndexChange(CGuiData *pSender)
{
	NPCHelper* pNPCHelper = NPCHelper::I();
	SAFE_DELETE(pNPCHelper);
	pNPCHelper = new NPCHelper(0,1000);

	int index = g_stUIStart.listPage->GetIndex();
	if(index == 0) //npc
	{
		pNPCHelper->LoadRawDataInfo("scripts/table/NPCList", FALSE);
		g_stUIStart.lstCurrList = g_stUIStart.lstNpcList;
	}
	else if(index == 1)
	{
		pNPCHelper->LoadRawDataInfo("scripts/table/MonsterList", FALSE);
		g_stUIStart.lstCurrList = g_stUIStart.lstMonsterList;
	}
	g_stUIStart.ShowNPCHelper(g_stUIStart.GetCurrMapName(),true);
}

void CStartMgr::_evtNPCListChange(CGuiData *pSender)
{
	CListItems* pItems = g_stUIStart.lstCurrList->GetItems();
	int nIndex = pItems->GetIndex(g_stUIStart.lstCurrList->GetSelectItem());

	CItemRow* itemrow = g_stUIStart.lstCurrList->GetSelectItem();
	CItemObj* itemobj = itemrow->GetBegin();

	std::string itemstring(itemobj->GetString());

	int pos = (int)itemstring.find(">") + 1;
	int pos1 = (int)itemstring.find("<",pos);
	int pos2 = (int)itemstring.find(",",pos);
	int pos3 = (int)itemstring.find(">",pos);

	if(pos1 >= 0 && pos2 > pos1 && pos3 > pos2 && pos3 <= (int)itemstring.length())
	{
		std::string xStr = itemstring.substr(pos1 + 1,pos2 - pos1 - 1);
		std::string yStr = itemstring.substr(pos2 + 1,pos3 - pos2 - 1);

		g_stUIMap.ShowRadar(xStr.c_str(),yStr.c_str());
	}
	g_stUIStart.ShowNPCHelper(g_stUIStart.GetCurrMapName(),false);
}


void CStartMgr::_evtOriginReliveFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	frmSelectOriginRelive = nullptr;
	if( pSender->GetForm()->nTag==enumEPLAYER_RELIVE_ORIGIN )
	{
		if( nMsgType==CForm::mrYes )
		{
			CS_DieReturn(enumEPLAYER_RELIVE_ORIGIN);		
			g_stUIStart.frmMainChaRelive->SetIsShow(false);
		}
		else
		{
			CS_DieReturn(enumEPLAYER_RELIVE_NORIGIN);		
		}
	}
	else
	{
		if( nMsgType==CForm::mrYes )
		{
			CS_DieReturn(enumEPLAYER_RELIVE_MAP);		
			g_stUIStart.frmMainChaRelive->SetIsShow(false);
		}
		else
		{
			CS_DieReturn(enumEPLAYER_RELIVE_NOMAP);		
		}
	}
}

void CStartMgr::_evtReliveFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	//if( name=="btnReCity" )
	{
		CS_DieReturn(enumEPLAYER_RELIVE_CITY);
		pSender->GetForm()->SetIsShow(false);
		if( frmSelectOriginRelive )
		{
			frmSelectOriginRelive->SetIsShow(false);
			frmSelectOriginRelive = nullptr;
		}
	}
}

void CStartMgr::_evtStartFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	std::string name=pSender->GetName();

	if( name=="btnState" )	// ��ʾ���Խ���e
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmState" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );
		}
		return;
	}	
	//btnOpenDrop
	else if (name == "btnOpenDrop")	// ??ê????ü????
	{
		CForm* f = CFormMgr::s_Mgr.Find("frmMobInfoDrop");
		//CForm* f2 = CFormMgr::s_Mgr.Find("frmMobInfo");
		if (f)
		{
			//f2->SetIsShow(false);
			f->SetIsShow(!f->GetIsShow());
		}
		//g_pGameApp->SysInfo("working");
		return;
	}
	/*else if (name == "btnMobInfo")	// ??ê????ü????
	{
		//CForm* f = CFormMgr::s_Mgr.Find("frmMobInfo");
		CForm* f = CFormMgr::s_Mgr.Find("frmMobInfoDrop");
		if (f)
		{
			//f2->SetIsShow(false);
			f->SetIsShow(!f->GetIsShow());
		}
		return;
	}	*/
		//	drop button end 	
	else if( name=="btnSkill" )	// ��ʾ���ܽ���
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmSkill" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );
		}
		return;
	}
	else if( name=="btnMission" )	// ��ʾ�������??
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmMission" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );	
		}
		return;
	}
	//else if( name=="btnGuild" )	    // ��ʾ�������??
	//{		
	//	CForm* f = CFormMgr::s_Mgr.Find( "frmManage" );
	//	if( f ) 
	//	{
	//		f->SetIsShow( !f->GetIsShow() );	
	//	}		
	//	return;
	//}
	else if( name=="btnHelp" )		
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmStartHelp" );
		if( f ) 
		{
			f->evtEntrustMouseEvent = _HelpFrmMainMouseEvent;
			f->SetIsShow( !f->GetIsShow() );
		}
		return;
	}
	//else if( name=="btnShip" )	// ��ʾ��������	Modify by alfred.shi 20080822
	//{
 //       CForm* f = CFormMgr::s_Mgr.Find("frmStartHelp");
	//	if( f ) 
	//	{
	//		f->evtEntrustMouseEvent = _HelpFrmMainMouseEvent;
	//		f->SetIsShow( !f->GetIsShow() );
	//	}

 //       return;
	//}

	// ��ʾ��������		Add by alfred.shi 20080822	beign
	else if( name=="btnShip1" )	
	{
        CForm* f = CFormMgr::s_Mgr.Find("frmStartHelp");
		if( f ) 
		{
			f->evtEntrustMouseEvent = _HelpFrmMainMouseEvent;
			f->SetIsShow( !f->GetIsShow() );
		}

        return;
	}
	//	End

	//if( name=="btnOpenHelp" )	
	//{	
	//	CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
	//	if( frm ) 
	//	{
	//		frm->evtEntrustMouseEvent = _NewFrmMainMouseEvent;
	//		frm->nTag = 0;
	//		frm->ShowModal();
	//	}
	//	return;
	//}

	//	Add by alfred.shi 20080822	begin
	if( name=="btnShip2" )		
	{	
		CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
		if( frm ) 
		{
			frm->evtEntrustMouseEvent = _NewFrmMainMouseEvent;
			frm->nTag = 0;
			frm->ShowModal();
		}
		return;
	}
	//	End

	else if( name=="btnSystem" )	// ��ʾ���ܽ���
	{
		CForm* f = CFormMgr::s_Mgr.Find("frmSystem");
		if(f)		f->SetIsShow(!f->GetIsShow());			
		return;
	}
	else if( name=="btnQQ" )	
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmQQ" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );
		}
		return;
	}
	else if( name == "btnLevelUpHelp" )	// ��ҽ��?��������
	{
		SGameAttr* pAttr = CGameScene::GetMainCha()->getGameAttr();

		int nLevel = pAttr->get(ATTR_LV);
		g_stUIStart.ShowHelpSystem(true, nLevel + HELP_LV1_BEGIN - 1);

		g_stUIStart.ShowLevelUpHelpButton(false);
	}
	else if( name == "btnInfoCenter" )	// �������ֵ�һЩ�淨�Ľ���
	{
		bool bShow = g_stUIStart.frmHelpSystem->GetIsShow();
		g_stUIStart.ShowHelpSystem(!bShow);
	}
	else if( name == "btnOpenBag" )	// �򿪱�����ť����
	{
		CPage* Inventory = dynamic_cast<CPage*>(g_stUIEquip.frmInv->Find("pgePublic"));
			if(Inventory)Inventory->SetIndex(0);
		g_stUIEquip.GetItemForm()->SetIsShow(!g_stUIEquip.GetItemForm()->GetIsShow());
		//g_stUIStart.ShowBagButtonForm(! g_stUIStart.frmBag->GetIsShow());
		//g_stUIStart.ShowSociliatyButtonForm(false);
		
		//g_stUIStart.frmBag->SetIsShow(! g_stUIStart.frmBag->GetIsShow());
		//g_stUIStart.frmSociliaty->SetIsShow(false);
	}
	else if( name == "btnOpenSociliaty" ) // ���罻��ť����
	{
		//g_stUIStart.ShowSociliatyButtonForm(! g_stUIStart.frmSociliaty->GetIsShow());
		//g_stUIStart.ShowBagButtonForm(false);

		//g_stUIStart.frmSociliaty->SetIsShow(! g_stUIStart.frmSociliaty->GetIsShow());
		//g_stUIStart.frmBag->SetIsShow(false);
		
		CForm* f = CFormMgr::s_Mgr.Find( "frmManage" );
		if( f ) 
		{
			bool a =f->GetIsShow();
			f->SetIsShow( !a );
		//	Add by alfred.shi 20080905	begin
		CCharacter* pMainCha = CGameScene::GetMainCha();
		if(pMainCha->getGuildID()<=0)
		{
			g_pGameApp->MsgBox("You are not in a guild.");
		}
		//	End.
		}		
		return;
		
		
		
	}
	//else if( name == "btnOpenItem")	// ��ʾ���߽���
	//{
	//	CForm* f = CFormMgr::s_Mgr.Find( "frmItem" );
	//	if( f )
	//	{
	//		f->SetIsShow( !f->GetIsShow() );
	//	}
	//	return;
	//}
	else if( name == "btnOpenTempBag" ) // ����ʱ����
	{
		//g_stUIStore.ShowTempKitbag();
	}
	else if( name == "btnOpenStore" )	// ���̳�
	{
		// �̳�δ����,����ҳ
		//g_stUIStore.ShowStoreWebPage();

		// ���̳������������??��ʱ���������̳ǣ�
		g_stUIDoublePwd.SetType(CDoublePwdMgr::STORE_OPEN_ASK);
		g_stUIDoublePwd.ShowDoublePwdForm();
	}
	else if( name == "btnOpenGuild" )	// ��ʾ����
	{
	
		CForm* f = CFormMgr::s_Mgr.Find( "frmManage" );
		if( f ) 
		{
			bool a =f->GetIsShow();
			f->SetIsShow( !a );
		//	Add by alfred.shi 20080905	begin
		CCharacter* pMainCha = CGameScene::GetMainCha();
		if(pMainCha->getGuildID()<=0)
		{
			g_pGameApp->MsgBox("You are not in a guild.");
		}
		//	End.
		}		
		return;
	}
	else if( name == "btnOpenTeam" )	// ־Ը��
	{
		CCharacter* pMainCha = CGameScene::GetMainCha();

		// ��ɫ 8 �����½�ֹ���??	Modify by alfred.shi 20080902	begin
		if(g_stUIFindTeam.IsShowFom())
			g_stUIFindTeam.ShowFindTeamForm(false);
		else if(pMainCha && !pMainCha->IsBoat() && pMainCha->getGameAttr()->get(ATTR_LV) >= 8)
		{
			CS_VolunteerOpen((short) CFindTeamMgr::FINDTEAM_PAGE_SIZE);
		}	//	End
		else
		{
			if(pMainCha->IsBoat())
			{
				g_pGameApp->SysInfo(g_oLangRec.GetString(888));
			}
			else
			{
				g_pGameApp->SysInfo(g_oLangRec.GetString(866));
				g_pGameApp->MsgBox(g_oLangRec.GetString(866));	//	Add by alfred.shi 20080905
			}
		}
	}

	return;
}

void CStartMgr::_evtSelfMouseDown(CGuiData *pSender,int x,int y ,DWORD key)
{
	CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	if( !pScene ) return;

	CCharacter* pMain = CGameScene::GetMainCha();
	if( !pMain ) return;

	if( key & Mouse_LDown )
	{
		CSkillRecord* pSkill = pMain->GetReadySkillInfo();
		if( pSkill && g_SkillUse.IsUse( pSkill, pMain, pMain ) )
		{
			pScene->GetMouseDown().ActAttackCha( pMain, pSkill, pMain );
		}
	}
	else if( (key & Mouse_RDown) && (pMain->GetTeamLeaderID()!=0) )
	{		
		pSender->GetForm()->PopMenu( g_stUIStart.mnuSelf, x, y );
	}
}

void CStartMgr::MainChaDied()
{
	if( frmMainChaRelive )
	{
		int nLeft = ( g_pGameApp->GetWindowHeight() - frmMainChaRelive->GetWidth() ) / 2;
		int nTop = ( g_pGameApp->GetWindowHeight() - frmMainChaRelive->GetHeight() ) / 2;
		nTop-=80;
		frmMainChaRelive->SetPos( nLeft, nTop );
		frmMainChaRelive->Refresh();
		
		static CLabel* pInfo = dynamic_cast<CLabel*>( frmMainChaRelive->Find( "labReCity" ) );	
		CCharacter* pCha = CGameScene::GetMainCha();
		bool IsShow = true;
		if( pInfo && pCha )
		{
			if( pCha->IsBoat() )
			{
				pInfo->SetCaption( g_oLangRec.GetString(761) );				
			}
			else
			{
				pInfo->SetCaption( g_oLangRec.GetString(762) );

				if( CGameScene* pScene = CGameApp::GetCurScene() )
				{
					if( CMapInfo *pInfo = pScene->GetCurMapInfo() )
					{
						// Modify by lark.li 20080719 begin
						//if( stricmp( pInfo->szDataName, "teampk" )==0 )
						if( stricmp( pInfo->szDataName, "teampk" )==0 || stricmp( pInfo->szDataName,"starena1") == 0 
							|| stricmp( pInfo->szDataName,"starena2") == 0 || stricmp( pInfo->szDataName,"starena3") == 0)
							IsShow = false;
						// End
					}
				}
			}
		}

		// add by Philip.Wu  ��ɫ������ִ��һ���ƶ������ڹرմ���
		CUIInterface::MainChaMove();

		// add by Philip.Wu  2006-07-05  ��ɫ������رս��׽���??
		// BUG��ܣ�TEST-32  �ɽ��׺󴥷����������޹���bug
		g_stUITrade.CloseAllForm();

		// add by Philip.Wu  2006-07-12  ��ɫ������رճ�������??
		CWorldScene* pWorldScene = dynamic_cast<CWorldScene*>(g_pGameApp->GetCurScene());
		if(pWorldScene && pWorldScene->GetShipMgr())
		{
			pWorldScene->GetShipMgr()->CloseForm();
		}

		// in-game shop crash fix when cha dies with store opened (6/28/2023)
		if (g_stUIStore.GetStoreForm()->GetIsShow())
			g_stUIStore.GetStoreForm()->SetIsShow(false);


		if( IsShow ) 
			frmMainChaRelive->Show();
	}
}

void CStartMgr::CheckMouseDown( int x, int y  )
{
	//if( frmMainFun->GetIsShow() )
	//{
	//	if( !frmMainFun->InRect(x,y) )
	//	{
	//		//frmMainFun->SetIsShow(false);	
	//	}
	//}

	//if ( grdAction->GetIsShow() )
	//{
	//	if ( !grdAction->InRect(x,y) )
	//		grdAction->SetIsShow(false);
	//}		

	//if ( grdHeart->GetIsShow() )
	//{
	//	if ( !grdHeart->InRect(x,y) )
	//		grdHeart->SetIsShow(false);		
	//}	

	//if ( g_stUICoze.GetFaceGrid()->GetIsShow() )
	//{
	//	if ( !g_stUICoze.GetFaceGrid()->InRect(x,y) )
	//		g_stUICoze.GetFaceGrid()->SetIsShow(false);		
	//}
}

void CStartMgr::_evtTaskMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	std::string name=pSender->GetName();

	if( name=="btnStart" )
	{
		CForm* f = CFormMgr::s_Mgr.Find( "frmMainFun" );
		if( f ) 
		{
			f->SetIsShow( !f->GetIsShow() );			
		}
		return;
	}	
	//else if( name=="btnAction" )
	//{
	//	g_stUIStart.grdHeart->SetIsShow( false );
	//	g_stUICoze.GetFaceGrid()->SetIsShow( false );
	//	g_stUIStart.grdAction->SetIsShow(!g_stUIStart.grdAction->GetIsShow() );			
	//	return;
	//}	
	//else if( name=="btnBrow" )	
	//{
	//	g_stUIStart.grdAction->SetIsShow( false );
	//	g_stUICoze.GetFaceGrid()->SetIsShow( false );
	//	g_stUIStart.grdHeart->SetIsShow( !g_stUIStart.grdHeart->GetIsShow() );
	//	return;
	//}	
	//else if( name=="btnChatFace" )
	//{
	//	g_stUIStart.grdAction->SetIsShow( false );
	//	g_stUIStart.grdHeart->SetIsShow( false );
	//	g_stUICoze.GetFaceGrid()->SetIsShow( !g_stUICoze.GetFaceGrid()->GetIsShow() );
	//	return;
	//}

	// ��ݼ��ĵ�� 
	if( pSender->nTag > 10 )
	{
		CCharacter* c = CGameScene::GetMainCha();
        if( !c )		return;

		c->ChangeReadySkill( pSender->nTag );
	}
	return;
}

void  CStartMgr::_evtChaActionChange(CGuiData *pSender)
{
	CCharacter* pCha = g_pGameApp->GetCurScene()->GetMainCha();
	if( !pCha ) return;
	
	pSender->SetIsShow( false );

	CGrid *p = dynamic_cast<CGrid*>(pSender);
	if( !p ) return;
	CGraph * r = p->GetSelect();
	int nIndex = p->GetSelectIndex();
    if( r )
	{		
		pCha->GetActor()->PlayPose( r->nTag, true, true);
	}	
}

void  CStartMgr::_evtChaHeartChange(CGuiData *pSender)
{
	CCharacter* pCha = CGameScene::GetMainCha();
	if( !pCha ) return;  
	pSender->SetIsShow( false );

	CGrid *p = dynamic_cast<CGrid*>(pSender);
	if( !p ) return;
	CGraph * r = p->GetSelect();
	int nIndex = p->GetSelectIndex();
    if( r )
	{
		pCha->GetHeadSay()->SetFaceID(nIndex);
		char szFaceID[10] = {0} ;
		sprintf( szFaceID , "***%d" ,nIndex);	
		CS_Say( szFaceID);		
	}
}

void CStartMgr::RefreshMainLifeNum( long num, long max )
{	
	////HP�ı仯
	//char szHP[32] = { 0 };
	//if ( num < 0 )	num = 0;
	//sprintf( szHP,"%d/%d", num, max);
	//szHP[sizeof(szHP)-1] = '\0';

	//float f = (float) num /(float) max;
	//CProgressBar* pBar = nullptr;
	//if( f < 0.334 )
	//{
	//	pBar = proMainHP3;
	//	proMainHP2->SetIsShow(false);
	//	proMainHP1->SetIsShow(false);
	//}
	//else if( f > 0.666 )
	//{
	//	pBar = proMainHP1;
	//	proMainHP2->SetIsShow(false);
	//	proMainHP3->SetIsShow(false);
	//}
	//else
	//{
	//	pBar = proMainHP2;
	//	proMainHP1->SetIsShow(false);
	//	proMainHP3->SetIsShow(false);
	//}

	//pBar->SetIsShow(true);
	//pBar->SetPosition( 10.0f * f ) ;

	if (proMainHP)
	{
		proMainHP->SetRange(0.0f, (float)max);
		proMainHP->SetPosition((float)num);
	}
}

void CStartMgr::RefreshMainExperience(long num, long curlev, long nextlev)
{
	LG("exp", g_oLangRec.GetString(763), num, curlev, nextlev, 100.0f * (float)(num - curlev) / (float)(nextlev - curlev));
	
	//// EXP�ı仯
	//long max = nextlev - curlev;
	//num = num - curlev;
	//if ( num < 0 ) num = 0;

	//if (max!=0)
	//	sprintf( szBuf , "%4.2f%%" , num*100.0f/max );	
	//else 
	//	sprintf( szBuf , "0.00%");
	//szBuf[sizeof(szBuf)-1] = '\0';

	//_pShowExp->SetCaption( szBuf );
	if ( proMainExp )
	{
		proMainExp->SetRange(0, 1);
		proMainExp->SetPosition((float)(num - curlev) / (float)(nextlev - curlev));
	}
}

/*void CStartMgr::RefreshMainLifeExperience(long num , long max)
{

	LG("exp", g_oLangRec.GetString(763), num, 100.0f * (float)(num - max));


	if ( proMainLifeExp )
	{
		proMainLifeExp->SetRange(0.0f, (float)max);
		proMainLifeExp->SetPosition( (float)num );  
	}
}*/

void CStartMgr::RefreshMainName(const char* szName )
{
	if ( labMainName )
	{
		labMainName->SetCaption(szName);
	}
}


void CStartMgr::RefreshLv( long l )
{
	if (labMainLevel)
	{
		sprintf( szBuf, "%d", l );
		labMainLevel->SetCaption( szBuf );
	}
}

void CStartMgr::RefreshMainSP( long num , long max )
{
	//SP�ı仯
	if ( proMainSP )
	{
		proMainSP->SetRange(0.0f, (float)max);
		proMainSP->SetPosition( (float)num );  
	}
}

// �Ҽ��˵���Ϣ����
void CStartMgr::_evtPopMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	mainMouseRight->SetIsShow(false);
	CMenuItem* pItem=mainMouseRight->GetSelectMenu();
	if (!pItem) return;
	std::string str=pItem->GetString();
	if (str==g_oLangRec.GetString(764))	// Invite transaction
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if( pCha && pMain && pCha!=pMain && pCha->IsEnabled() && pMain->IsEnabled() 
			&& ( ( !pCha->IsBoat() && !pMain->IsBoat() )
				|| ( pCha->IsBoat() && pMain->IsBoat() ) ) )
		{
			if(pMain->IsBoat() || pMain->getGameAttr()->get(ATTR_LV) >= 6)
			{
				CS_RequestTrade( mission::TRADE_CHAR, mainMouseRight->nTag );
			}
			else
			{
				// Characters below level 6 are prohibited from initiating invitation transactions
				g_pGameApp->SysInfo(g_oLangRec.GetString(864));
			}
		}
		else
		{
			g_pGameApp->SysInfo( g_oLangRec.GetString(765) );	// Both parties must be players or ships when trading
		}
	}
	else if (str==g_oLangRec.GetString(482))	// ���Ӻ���
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if(!pCha || !pMain)  return;

		if(pMain->IsBoat() || pMain->getGameAttr()->get(ATTR_LV) >= 7)
		{
			CS_Frnd_Invite( pCha->getHumanName());
		}
		else
		{
			// ��ɫ�ȼ�7�����½�ֹ�������Ӻ���
			g_pGameApp->SysInfo(g_oLangRec.GetString(865));
		}
	}
	else if (str==g_oLangRec.GetString(484))	// �������??
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if(!pCha || !pMain)  return;

		if( (pMain->IsBoat() || pMain->getGameAttr()->get(ATTR_LV) >= 8) &&
			 (pCha->IsBoat() ||  pCha->getGameAttr()->get(ATTR_LV) >= 8))
		{
			CS_Team_Invite( pCha->getHumanName());
		}
		else
		{
			// ��ɫ�ȼ�8�����½�ֹ�����������??
			g_pGameApp->SysInfo(g_oLangRec.GetString(866));
		}

		return;	
	}
	else if (str==g_oLangRec.GetString(483))	// �뿪����
	{
		CS_Team_Leave();
		return;
	}
	else if (str==g_oLangRec.GetString(481))	// ����Է�??
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if(pCha)
		{
			CCozeForm::GetInstance()->OnPrivateNameSet(pCha->getHumanName());
		}
		return;
	}
	else if (str==g_oLangRec.GetString(766))	// ���ս���
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if( pCha && pMain && pCha->IsBoat() && pMain->IsBoat() )
		{
			CS_RequestTrade( mission::TRADE_BOAT, mainMouseRight->nTag );
		}
		else
		{
			g_pGameApp->SysInfo( g_oLangRec.GetString(767) );
		}
		return;
	}
	else if( str==g_oLangRec.GetString(768) )	// �鿴̯λ
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if( pCha && !pCha->IsMainCha() )
		{
			CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
			if( pScene )
			{
				pScene->GetMouseDown().ActShop( CGameScene::GetMainCha(), pCha );
			}
		}
		return;
	}
	else if( str==g_oLangRec.GetString(769) )	// ���鵥��
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if( pCha ) CS_TeamFightAsk( pCha->getAttachID(), pCha->lTag, enumFIGHT_TEAM );
		return;
	}
	else if( str==g_oLangRec.GetString(770) )	// ��ҵ���??
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		if( pCha ) CS_TeamFightAsk( pCha->getAttachID(), pCha->lTag, enumFIGHT_MONOMER );
		return;
	}
	else if(str == g_oLangRec.GetString(855))	// ������?
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if( pCha && pMain && !pCha->IsBoat() && !pMain->IsBoat() )
		{
			const char* szName = pCha->getHumanName();
			CS_MasterInvite(pCha->getHumanName(), mainMouseRight->nTag);
		}
		else
		{
			// ��ʦ����ͽʱ����˫���������??
			g_pGameApp->SysInfo(g_oLangRec.GetString(888));
		}
	}
	else if(str == g_oLangRec.GetString(859))	// ������ͽ
	{
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		if( pCha && pMain && !pCha->IsBoat() && !pMain->IsBoat() )
		{
			const char* szName = pCha->getHumanName();
			CS_PrenticeInvite(pCha->getHumanName(), mainMouseRight->nTag);
		}
		else
		{
			// ��ʦ����ͽʱ����˫���������??
			g_pGameApp->SysInfo(g_oLangRec.GetString(888));
		}
	}else if (str=="Check Eq"){
		//TODO  - Move this to a different form ?.
		CCharacter * pCha = (CCharacter*)mainMouseRight->GetPointer();
		CCharacter * pMain = CGameScene::GetMainCha();
		g_stUIEquip.UpdataEquipSpy( pCha->GetPart(), pCha );
	}
	else if(str == "Block")
	{
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
		CTeam* pTeam = g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked);
		if(!pTeam->FindByName(pCha->getHumanName()))
			pTeam->Add(-1, pCha->getHumanName(), "", 9);
	}
	else if(str == "Unblock")
	{
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
		g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked)->DelByName(pCha->getHumanName());
	}else if(str == "Manage"){
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
		CS_RequestTalk( pCha->getAttachID(), 0 );
	}
	else if (str == "Follow") {	//@mothannakh allow follow without alt+click 
	CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	if (pScene)
	{
		CCharacter* pCha = (CCharacter*)mainMouseRight->GetPointer();
		if (pCha)
			pScene->GetMouseDown().SetAltFollow(CGameScene::GetMainCha(), pCha);
	}
	}

	g_stUIStart.frmMain800->Refresh();
}

void CStartMgr::AskTeamFight( const char* str )
{
	g_stUIBox.ShowSelectBox( _evtAskTeamFightMouseEvent, str, false );
}

void CStartMgr::_evtAskTeamFightMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	CS_TeamFightAnswer( nMsgType==CForm::mrYes );
}

// �����Ҽ��˵�
void  CStartMgr::PopMenu( CCharacter* pCha )
{
	//g_pGameApp->GetCurScene()->GetTerrainName();

	if( !g_stUIStart.IsCanTeam() )
		return;

	if( g_stUIBank.GetBankGoodsGrid()->GetForm()->GetIsShow())		// �����ĵ��޸ģ��������в��������Ҽ��˵�
		return;

	if( g_stUIBooth.GetBoothItemsGrid()->GetForm()->GetIsShow())	// �����ĵ��޸ģ���̯���������Ҽ��˵�
        return;

	if( g_stUITrade.IsTrading())// �����ĵ��޸ģ�����ʱ���������Ҽ��˵�
		return;


	if ( mainMouseRight && pCha && pCha->IsValid() && !pCha->IsHide() && pCha->IsPlayer() )
	{
		mainMouseRight->nTag = pCha->getAttachID();
		mainMouseRight->SetPointer( pCha );

		CCharacter * pMain = CGameScene::GetMainCha();
		if( !pMain ) return;

		if( !pMain->IsValid() || pMain->IsHide() ) return;

		if( pCha->GetIsPet()) return;	// ���ﲻ��������Ҽ��˵�??

		int nMainGuildID = pMain->getGuildID();
		int nChaGuildID  = pCha->getGuildID();
		if( nMainGuildID > 0 && nChaGuildID > 0)
		{
			if(g_stUIMap.IsGuildWar() && ((nMainGuildID <= 100 && nChaGuildID > 100) || (nMainGuildID > 100 && nChaGuildID <= 100))) return;	// ��ͬ��Ӫ
		}

		mainMouseRight->SetAllEnabled( false );
		int nCount = mainMouseRight->GetCount();
		CMenuItem* pItem = nullptr;
		const char *MapName = g_pGameApp->GetCurScene()->GetTerrainName(); // ȡ�����ﵱǰ���ڵ�ͼ Add by ning.yan 20080715
		//Add by sunny.sun20080820
		//Begin
		if( stricmp( MapName,"starena1") == 0 || stricmp( MapName,"starena2") == 0 || stricmp( MapName,"starena3") == 0 )
			return;
		for( int i=0; i<nCount; i++ )
		{
			pItem = mainMouseRight->GetMenuItem( i );
			
			
			if( stricmp( pItem->GetString(), "Manage" )==0 ){
				if(!pCha->getIsPlayerCha()){
					pItem->SetIsHide( false );
					pItem->SetIsEnabled( true );
				}else{
					pItem->SetIsHide( true );
					pItem->SetIsEnabled( false );
				}
			}
			
			if(!pCha->getIsPlayerCha()){
				continue;
			}
			
			//@mothannakh show aviable commands only 
			if( stricmp( pItem->GetString(), g_oLangRec.GetString(764) )==0 )
			{
				if( pMain!=pCha && pMain->IsEnabled()&& !pCha->IsShop() && pCha->IsEnabled() 
					&& ( (pMain->IsBoat() && pCha->IsBoat()) 
						 || (!pMain->IsBoat() && !pCha->IsBoat()) 
					   ) 
				   )

				{
					pItem->SetIsEnabled( true );
				}
				else
				{
					pItem->SetIsEnabled( false );
				}
				if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );	
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(482) )==0 )
			{
				pItem->SetIsEnabled( pMain!=pCha && !pCha->IsShop() );
				if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );	
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(484) )==0 )
			{	// ��������ȡ����ӹ���? Add by ning.yan  20080715 Begin
				//if( stricmp( MapName,"starena1") == 0 || stricmp( MapName,"starena2") == 0 || stricmp( MapName,"starena3") == 0 )
				//	pItem->SetIsEnabled( false );
				//// End
				//else			
				pItem->SetIsEnabled( g_stUIStart.IsCanTeam() && pMain!=pCha && !pCha->IsShop() && ( pMain->GetTeamLeaderID()==0 || ( pMain->IsTeamLeader() && pCha->GetTeamLeaderID()!=pMain->GetTeamLeaderID() ) ) );
			if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );	 
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(483) )==0 )
			{	// ��������ȡ���뿪���鹦��  Add by ning.yan  20080715 Begin
				//if( stricmp( MapName,"starena1") == 0 || stricmp( MapName,"starena2") == 0 || stricmp( MapName,"starena3") == 0 )
				//	pItem->SetIsEnabled( false );
				//// End
				//else
				pItem->SetIsEnabled( g_stUIStart.IsCanTeam() && pMain->GetTeamLeaderID()!=0 && pCha->GetTeamLeaderID()==pMain->GetTeamLeaderID() );
				if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(481) )==0 )
			{
				pItem->SetIsEnabled( pMain!=pCha && !pCha->IsShop() );
				if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(766) )==0 )
			{
				pItem->SetIsEnabled( pMain!=pCha && pMain->IsBoat() && pMain->IsEnabled() && pCha->IsBoat() && pCha->IsEnabled() );
				if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );	
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(768) )==0 )
			{
				pItem->SetIsEnabled( pMain!=pCha && pMain->IsEnabled() && !pMain->IsShop() && pCha->IsEnabled() && pCha->IsShop() );
				if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(769) )==0 )
			{
				pItem->SetIsEnabled( g_stUIStart.IsCanTeam() && pMain!=pCha && pMain->IsEnabled() && pMain->IsTeamLeader() && !pMain->IsShop()
					&& pCha->IsEnabled() && pCha->IsTeamLeader() && !pCha->IsShop()	);
				if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );	
					
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(770) )==0 )
			{
				bool IsInTeam = pMain->GetTeamLeaderID();	//mothannakh get if inteam or not 
				pItem->SetIsEnabled( !IsInTeam && g_stUIStart.IsCanTeam() && pMain!=pCha && !pCha->IsShop() && pCha->IsPlayer() );
				if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );	
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(855) )==0 )	// ������?
			{
				pItem->SetIsEnabled( pMain!=pCha && pCha->IsPlayer() && !pCha->IsShop() && pMain->getGameAttr() && pMain->getGameAttr()->get(ATTR_LV) <= 40 );
					if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );
			}
			else if( stricmp( pItem->GetString(), g_oLangRec.GetString(859) )==0 )	// ������ͽ
			{
				pItem->SetIsEnabled( pMain!=pCha && pCha->IsPlayer() && !pCha->IsShop() && pMain->getGameAttr() && pMain->getGameAttr()->get(ATTR_LV) > 40 );
							if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );
			}else if( stricmp( pItem->GetString(), "Check Eq" )==0 ){
				if (pMain->getGMLv()	 == 99 ){
					pItem->SetIsEnabled( pMain!=pCha );
					if(!pItem->GetIsEnabled()){pItem->SetIsHide( true );} else pItem->SetIsHide( false );
				}else{
					pItem->SetIsHide( true );
				}
				
			}
			else if( stricmp( pItem->GetString(), "Block" )==0 )
			{
				if (!g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked)->FindByName(pCha->getHumanName()))
				{
					pItem->SetIsHide( false );
					pItem->SetIsEnabled( pMain!=pCha );
				}
				else
				{
					pItem->SetIsHide( true );
				}
			}
			else if( stricmp( pItem->GetString(), "Unblock" )==0 )
			{
				if (g_stUIChat.GetTeamMgr()->Find(enumTeamBlocked)->FindByName(pCha->getHumanName()))
				{
					pItem->SetIsHide( false );
					pItem->SetIsEnabled( pMain!=pCha );
				}
				else
				{
					pItem->SetIsHide( true );
				}
			}
			//Follow
			else if (stricmp(pItem->GetString(), "Follow") == 0)
			{
			if (pMain != pCha)
			{
				pItem->SetIsHide(false);
				pItem->SetIsEnabled(pMain != pCha);
			}
			else
			{
				pItem->SetIsHide(true);
			}
			}
		}

		if( mainMouseRight->IsAllDisabled() )
		{
			mainMouseRight->SetIsShow( false );
			return;
		}

		int x=0, y=0;
		g_Render.WorldToScreen(pCha->GetPos().x , pCha->GetPos().y, pCha->GetPos().z, &x, &y );

		if( CForm::GetActive() && CForm::GetActive()->IsNormal() )
			CForm::GetActive()->PopMenu(mainMouseRight,x,y);
		else
			frmMain800->PopMenu(mainMouseRight,x,y);
	}
}

void CStartMgr::CloseForm()
{
	//if( frmMainFun->GetIsShow() )
	//	frmMainFun->Close();

	//grdAction->SetIsShow(false);
	//grdHeart->SetIsShow(false);
	//g_stUICoze.GetFaceGrid()->SetIsShow( false );
}

CTextButton* CStartMgr::GetShowQQButton()
{
	return btnQQ;
}

void CStartMgr::ShowBigText( const char* str )
{
	int nType = 0;
	CCharacter* pMain = CGameScene::GetMainCha();
	if( pMain && CGameApp::GetCurScene() && CGameApp::GetCurScene()->GetTerrain() )
	{
		int nArea = CGameApp::GetCurScene()->GetTerrain()->GetTile( pMain->GetCurX()/100, pMain->GetCurY()/100 )->getIsland();
		CAreaInfo* pArea = GetAreaInfo( nArea );
		if( pArea )
		{
			nType = pArea->chType;
		}
	}

	if( nType )
	{
		if( tlCity ) tlCity->SetIsShow( false );

		if( tlField )
		{
			tlField->SetCaption( str );
			tlField->SetIsShow( true );
		}
	}
	else
	{
		if( tlField ) tlField->SetIsShow( false );

		if( tlCity )
		{
			tlCity->SetCaption( str );
			tlCity->SetIsShow( true );
		}
	}
}

void CStartMgr::FrameMove(DWORD dwTime)
{
	static CTimeWork time(100);
	if( time.IsTimeOut( dwTime ) )
	{
		if (frmShipSail->GetIsShow())
		{
			UpdateShipSailForm();		
		}		
	}

	static CTimeWork pet_time(300);
	if( pet_time.IsTimeOut( dwTime ) ){
		if(g_stUIBoat.GetHuman()){
			SItemGrid Data = g_stUIBoat.GetHuman()->GetPart().SLink[enumEQUIP_FAIRY];
			if(Data.IsValid() && CGameScene::GetMainCha() ){
				CItemRecord* pItemInfo = GetItemRecordInfo( Data.sID );
				if( !pItemInfo || pItemInfo->sType!=enumItemTypePet){
					if( frmMainPet->GetIsShow() ){			
						frmMainPet->Hide();
					}
				}else{
					proPetHP->SetRange( 0, float(Data.sEndure[1] / 50) );
					proPetHP->SetPosition( float(Data.sEndure[0] / 50) );
		
					proPetSP->SetRange( 0, Data.sEnergy[1] );
					proPetSP->SetPosition( Data.sEnergy[0] );
		
					static bool IsFlash;
					IsFlash = !IsFlash;
					if( IsFlash && Data.sEnergy[1]==Data.sEnergy[0] ){
						proPetSP->GetImage()->SetColor( 0xff00ff00 );
					}
					else{
						proPetSP->GetImage()->SetColor( 0xFFFFFFFF );
					}
					if( !frmMainPet->GetIsShow() ){
						frmMainPet->Show();
					}
				}
			}else if( frmMainPet->GetIsShow() ){			
				frmMainPet->Hide();
			}
		}else if( frmMainPet->GetIsShow() ){			
			frmMainPet->Hide();
		}
	}
}

void CStartMgr::_evtShowBoatAttr(CGuiData *pSender,int x,int y ,DWORD key)	// ��ʾ��ֻ����
{
	xShipFactory* pkShip = ((CWorldScene*)g_pGameApp->GetCurScene())->GetShipMgr()->_factory;
	if (pkShip && pkShip->sbf.wnd->GetIsShow())
	{
		pkShip->sbf.wnd->SetIsShow(false);
	}
	else
	{
		CS_GetBoatInfo();
	}
}

void CStartMgr::SwitchMap()
{
	frmMainChaRelive->Close();
	CMenu::CloseAll();

	if( !(dynamic_cast<CWorldScene*>( CGameApp::GetCurScene() )) ) return;

	if( !_IsNewer ) return;

	CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
	if( frm ) 
	{
		frm->evtEntrustMouseEvent = _NewFrmMainMouseEvent;
		frm->nTag = 1;
		frm->ShowModal();
	}
}

//~ �ص����� =================================================================
void CStartMgr::_NewFrmMainMouseEvent(CCompent *pSender, int nMsgType, 
									  int x, int y, DWORD dwKey)
{
	std::string name = pSender->GetName();
	if( name=="btnNo"  || name == "btnClose" )  
	{	
		if (pSender->GetForm()->nTag == 1)
			CBoxMgr::ShowMsgBox( _CloseEvent, g_oLangRec.GetString(771) );
		else
			pSender->GetForm()->Close();
	}
}

void CStartMgr::_HelpFrmMainMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	std::string name = pSender->GetName();  
	if( name=="btnOpenHelp" )
	{	
		CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
		if( frm ) 
		{
			frm->evtEntrustMouseEvent = _NewFrmMainMouseEvent;
			frm->nTag = 0;
			frm->ShowModal();
		}
		return;
	}
}

void CStartMgr::_CloseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	CForm * frm = CFormMgr::s_Mgr.Find("frmVHelp");
	if( frm ) 
	{
		frm->Close();
	}
}

void CStartMgr::SysLabel( const char *pszFormat, ... )
{
	char szBuf[2048] = { 0 };

	va_list list;
	va_start(list, pszFormat);           
	vsprintf(szBuf, pszFormat, list);
	va_end(list);

	labFollow->SetCaption( szBuf );
	frmFollow->Show();
}

void CStartMgr::SysHide()
{
	if( frmFollow->GetIsShow() ) 
		frmFollow->Hide();
}

void CStartMgr::_MainChaRenderEvent(C3DCompent *pSender, int x, int y)
{
	pMainCha->Render();
	//pTarget->Render();
}

void CStartMgr::RefreshMainFace( stNetChangeChaPart& stPart )
{
	if( pMainCha )
	{
		static stNetTeamChaPart stTeamPart;	
		stTeamPart.Convert( stPart );
		pMainCha->UpdataFace( stTeamPart );
	}
	
	
}

void CStartMgr::_OnSelfMenu(CGuiData *pSender, int x, int y, DWORD key)
{
	CMenu* pMenu = dynamic_cast<CMenu*>( pSender );
	if( !pMenu ) return;

	pMenu->SetIsShow( false );

	CMenuItem* pItem= pMenu->GetSelectMenu();
	if (!pItem) return;

	std::string str = pItem->GetString();
	//Modify by sunny.sun20080820
	//Begin
	const char *MapName = g_pGameApp->GetCurScene()->GetTerrainName(); // ȡ�����ﵱǰ���ڵ�ͼ 
	if( stricmp( MapName,"starena1") == 0 || stricmp( MapName,"starena2") == 0 || stricmp( MapName,"starena3") == 0 )
	{
		pItem->SetIsEnabled( false );
	}
	else
	{
		pItem->SetIsEnabled( true );
		if (str==g_oLangRec.GetString(483) )
		{
			CS_Team_Leave();
		}
	}
	//End
}

void CStartMgr::SetIsLeader( bool v )
{
	imgLeader->SetIsShow( v );
}

bool CStartMgr::GetIsLeader()
{
	return imgLeader->GetIsShow();
}

bool CStartMgr::IsCanTeamAndInfo()
{
	if( !_IsCanTeam ) 	g_pGameApp->SysInfo( g_oLangRec.GetString(772) );	
	return _IsCanTeam;
}

void CStartMgr::RefreshPet(){
	if(g_stUIBoat.GetHuman()){
		SItemGrid pGrid = g_stUIBoat.GetHuman()->GetPart().SLink[enumEQUIP_FAIRY];
		SItemGrid pApp = g_stUIBoat.GetHuman()->GetPart().SLink[enumEQUIP_FAIRYAPP];
	
		int ID = pGrid.sID;
		if(ID == 0){
			return;
		}
		if(g_stUIBoat.GetHuman()->GetIsShowApparel() && pApp.sID!=0){
			ID = pApp.sID;
		}
		CItemRecord* pInfo = GetItemRecordInfo( ID );
		int nLevel = pGrid.GetAttr(ITEMATTR_VAL_STR)
			+ pGrid.GetAttr(ITEMATTR_VAL_AGI) 
			+ pGrid.GetAttr(ITEMATTR_VAL_DEX) 
			+ pGrid.GetAttr(ITEMATTR_VAL_CON) 
			+ pGrid.GetAttr(ITEMATTR_VAL_STA);
	
		sprintf( szBuf, "%d", nLevel );
		labPetLv->SetCaption( szBuf );
	
		static CGuiPic* Pic = imgPetHead->GetImage();
		Pic->LoadImage( pInfo->GetIconFile(), 
			imgPetHead->GetWidth(), imgPetHead->GetHeight(), 
			0, 
			4, 4 );
	
	}
}
void CStartMgr::RefreshPet( SItemGrid pGrid,SItemGrid pApp )
{
	int ID = pGrid.sID;
	if(g_stUIBoat.GetHuman()->GetIsShowApparel() && pApp.sID!=0){
		ID = pApp.sID;
	}
	CItemRecord* pInfo = GetItemRecordInfo( ID );
	if(pInfo){
		int nLevel = pGrid.GetAttr(ITEMATTR_VAL_STR)
			+ pGrid.GetAttr(ITEMATTR_VAL_AGI) 
			+ pGrid.GetAttr(ITEMATTR_VAL_DEX) 
			+ pGrid.GetAttr(ITEMATTR_VAL_CON) 
			+ pGrid.GetAttr(ITEMATTR_VAL_STA);
	
		sprintf( szBuf, "%d", nLevel );
		labPetLv->SetCaption( szBuf );
	
		static CGuiPic* Pic = imgPetHead->GetImage();
		Pic->LoadImage( pInfo->GetIconFile(), 
			imgPetHead->GetWidth(), imgPetHead->GetHeight(), 
			0, 
			4, 4 );
	}
}

void CStartMgr::RefreshPet( CItemCommand* pItem )
{
	static CItemRecord* pInfo = nullptr;
	pInfo = pItem->GetItemInfo();

	static SItemHint s_item;
	memset( &s_item, 0, sizeof(SItemHint) );
	s_item.Convert( pItem->GetData(), pInfo );

	// ���³���ȼ�??,ͷ��
	int nLevel = s_item.sInstAttr[ITEMATTR_VAL_STR]
				+ s_item.sInstAttr[ITEMATTR_VAL_AGI] 
				+ s_item.sInstAttr[ITEMATTR_VAL_DEX] 
				+ s_item.sInstAttr[ITEMATTR_VAL_CON] 
				+ s_item.sInstAttr[ITEMATTR_VAL_STA];

	sprintf( szBuf, "%d", nLevel );
	labPetLv->SetCaption( szBuf );

	static CGuiPic* Pic = imgPetHead->GetImage();
	Pic->LoadImage( pInfo->GetIconFile(), 
		imgPetHead->GetWidth(), imgPetHead->GetHeight(), 
		0, 
		4, 4 );
}


void CStartMgr::ShowHelpSystem(bool bShow, int nIndex)
{
	int nCount = g_stUIStart.lstHelpList->GetItems()->GetCount();

	if(0 > nIndex || nCount <= nIndex)
	{
		// Խ��
		frmHelpSystem->SetIsShow(bShow);
		return;
	}

	for(int i = 0; i < nCount; ++i)
	{
		imgHelpShow1[i]->SetIsShow(nIndex == i);
		imgHelpShow2[i]->SetIsShow(nIndex == i);
		imgHelpShow3[i]->SetIsShow(nIndex == i);
		imgHelpShow4[i]->SetIsShow(nIndex == i);
	}

	frmHelpSystem->SetIsShow(bShow);
}


void CStartMgr::ShowLevelUpHelpButton(bool bShow)
{
	if(btnLevelUpHelp)
	{
		btnLevelUpHelp->SetIsShow(bShow);
	}
}

void CStartMgr::ShowInfoCenterButton(bool bShow)
{
	CTextButton* btnInfoCenter = dynamic_cast<CTextButton*>(frmMainFun->Find("btnInfoCenter"));
	if(btnInfoCenter)
	{
		btnInfoCenter->SetIsShow(bShow);
	}
}

void CStartMgr::_evtHelpListChange(CGuiData *pSender)
{
	//g_pGameApp->MsgBox("Index: %d\nName:  %s", nIndex, pSender->GetName());//g_stUIStart.lstHelpList->GetSelectItem()->
	CListItems* pItems = g_stUIStart.lstHelpList->GetItems();
	int nIndex = pItems->GetIndex(g_stUIStart.lstHelpList->GetSelectItem());

	//g_stUIStart.ShowLevelUpHelpButton(false);
	g_stUIStart.ShowHelpSystem(true, nIndex);
}

void CStartMgr::ShowBagButtonForm(bool bShow)
{
	if(frmBag)
	{
		frmBag->SetPos(frmMainFun->GetX2() - frmBag->GetWidth()-109, frmMainFun->GetY() - frmBag->GetHeight()+41);
		frmBag->Refresh();

		frmBag->SetIsShow(bShow);
	}
}

void CStartMgr::ShowSociliatyButtonForm(bool bShow)
{
	if(frmSociliaty)
	{
		frmSociliaty->SetPos(frmMainFun->GetX2() - frmSociliaty->GetWidth()-67, frmMainFun->GetY() - frmSociliaty->GetHeight()+39);
		frmSociliaty->Refresh();

		frmSociliaty->SetIsShow(bShow);
	}
}


void CStartMgr::_OnDragEnd(CGuiData *pSender, int x, int y, DWORD key)
{

	if (g_stUIStart.frmTargetInfo->GetTop()<=0)
	{
		g_stUIStart.frmTargetInfo->SetPos(g_stUIStart.frmTargetInfo->GetLeft(), 0);
		g_stUIStart.frmTargetInfo->Refresh();
	}

	if (g_stUIStart.frmTargetInfo->GetLeft()<=0)
	{
		g_stUIStart.frmTargetInfo->SetPos(0, g_stUIStart.frmTargetInfo->GetTop());
		g_stUIStart.frmTargetInfo->Refresh();
	}

	if (g_stUIStart.frmTargetInfo->GetBottom()>=GetRender().GetScreenHeight())
	{
		g_stUIStart.frmTargetInfo->SetPos( g_stUIStart.frmTargetInfo->GetLeft(),  GetRender().GetScreenHeight() - g_stUIStart.frmTargetInfo->GetHeight() );
		g_stUIStart.frmTargetInfo->Refresh();
	}

	if (g_stUIStart.frmTargetInfo->GetRight()>=GetRender().GetScreenWidth())
	{
		g_stUIStart.frmTargetInfo->SetPos(GetRender().GetScreenWidth() - g_stUIStart.frmTargetInfo->GetWidth(), g_stUIStart.frmTargetInfo->GetTop());
		g_stUIStart.frmTargetInfo->Refresh();
	}

	//g_stUIStart.p3D->SetRenderEvent( _TargetRenderEvent );

	//MessageBox(0, g_stUIStart.p3D ? "gud" : "bed", "?", 0);

	RECT rt;
	rt.left = g_stUIStart.p3D->GetX();
	rt.right = g_stUIStart.p3D->GetX2();
	rt.top = g_stUIStart.p3D->GetY();
	rt.bottom = g_stUIStart.p3D->GetY2();
	pTarget->MoveWnd( rt );

}



#include "stdafx.h"

#include "Character.h"
#include "GameApp.h"
#include "packetcmd.h"
#include "SceneItem.h"
#include "StringLib.h"
#include "uiBoatForm.h"
#include "uiboxform.h"
#include "uidoublepwdform.h"
#include "uiEquipForm.h"
#include "uiformmgr.h"
#include "uigoodsgrid.h"
#include "UIGoodsGrid.h"
#include "uigraph.h"
#include "uiItem.h"
#include "uiItemCommand.h"
#include "uilabel.h"
#include "uilist.h"
#include "uistoreform.h"
#include <shellapi.h>
#include <time.h>

#include "GlobalVar.h"


namespace GUI
{

	CStoreMgr::CStoreMgr()
	{
		frmStore = nullptr;

		m_nCurClass = 0;
		m_nCurSel   = -1;
		m_nVip      = 0;

		_dwDarkScene = 0;

		m_pCurrMainCha = nullptr;
	}
	
	#define EQUIP_TYPE 0
	#define BANK_TYPE 1
		
	
	bool CStoreMgr::Init()
	{
		CFormMgr &mgr = CFormMgr::s_Mgr;

		grdTempBag = dynamic_cast<CGoodsGrid*>(g_stUIEquip.frmInv->Find("grdTempBag"));
		if (!grdTempBag)
		{
			LG("gui", "frmTempBag:grdTempBag not found.\n");
			return false;
		}
		grdTempBag->evtBeforeAccept = CUIInterface::_evtDragToGoodsEvent;


		frmStore = mgr.Find("frmStore");
		if(!frmStore)
		{
			LG("gui", "frmStore not found.\n");
			return false;
		}
		frmStore->evtEntrustMouseEvent = _evtStoreFormMouseEvent;
		frmStore->evtClose             = _evtStoreFormClose;

		// ÉÌ³ÇÊ÷
		trvStore = dynamic_cast<CTreeView*>(frmStore->Find("trvStore"));
		if(!trvStore)
		{
			LG("gui", "frmStore:trvStore not found. \n");
			return false;
		}
		trvStore->SetSelectColor( 0 );
		trvStore->evtMouseDown = _evtStoreTreeMouseClick;

		// ½ðÇ®Ê£Óà
		labMoneyLeft = dynamic_cast<CLabelEx*>(frmStore->Find("labMoneyLeft"));
		if(!labMoneyLeft)
		{
			LG("gui", "frmStore:labMoneyLeft not found. \n");
			return false;
		}

		// ´ú±ÒÊ£Óà
		labBeanLeft = dynamic_cast<CLabelEx*>(frmStore->Find("labBeanLeft"));
		if(!labBeanLeft)
		{
			LG("gui", "frmStore:labBeanLeft not found. \n");
			return false;
		}

		// »áÔ±ÀàÐÍ
		labMemberStyle = dynamic_cast<CLabelEx*>(frmStore->Find("labMemberStyle"));
		if(! labMemberStyle)
		{
			LG("gui", "frmStore:labMemberStyle not found. \n");
			return false;
		}


		labNameTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labNameTitle0"));
		if(!labNameTitle)
		{
			LG("gui", "frmStore:labNameTitle0 not found. \n");
			return false;
		}

		labPriceTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labPriceTitle"));
		if(!labPriceTitle)
		{
			LG("gui", "frmStore:labPriceTitle not found. \n");
			return false;
		}

		labLeftTimeTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labLeftTimeTitle"));
		if(!labLeftTimeTitle)
		{
			LG("gui", "frmStore:labLeftTimeTitle not found. \n");
			return false;
		}

		labLeftNumTitle = dynamic_cast<CLabelEx*>(frmStore->Find("labLeftNumTitle"));
		if(!labLeftNumTitle)
		{
			LG("gui", "frmStore:labLeftNumTitle not found. \n");
			return false;
		}




		// ÉÌ³ÇÒ³±êÇ©
		labListPage = dynamic_cast<CLabelEx*>(frmStore->Find("labListPage"));
		if(!labListPage)
		{
			LG("gui", "frmStore:labListPage not found. \n");
			return false;
		}

		// ÉÌ³ÇÒ³×ó°´Å¦
		btnLeftPage = dynamic_cast<CTextButton*>(frmStore->Find("btnLeftPage"));
		if(! btnLeftPage)
		{
			LG("gui", "frmStore:btnLeftPage not found. \n");
			return false;
		}

		// ÉÌ³ÇÒ³ÓÒ°´Å¦
		btnRightPage = dynamic_cast<CTextButton*>(frmStore->Find("btnRightPage"));
		if(! btnRightPage)
		{
			LG("gui", "frmStore:btnRightPage not found. \n");
			return false;
		}

		// ½»Ò×°´Å¦
		btnTrade = dynamic_cast<CTextButton*>(frmStore->Find("btnTrade"));
		if(! btnTrade)
		{
			LG("gui", "frmStore:btnTrade not found. \n");
			return false;
		}
		btnTrade->SetIsEnabled(false);

		// ÊÔ´©°´Å¥
		btnTryon = dynamic_cast<CTextButton*>(frmStore->Find("btnTryon"));
		if(! btnTryon)
		{
			LG("gui", "frmStore:btnTryon not found. \n");
			return false;
		}
		btnTryon->SetIsEnabled(false);

		btnViewAll = dynamic_cast<CTextButton*>(frmStore->Find("btnViewAll"));
		if(! btnViewAll)
		{
			LG("gui", "frmStore:btnViewAll not found. \n");
			return false;
		}
		btnViewAll->SetIsEnabled(false);

		// ½»Ò×°´Å¥×Ö
		labTrade = dynamic_cast<CLabelEx*>(frmStore->Find("labTrade"));
		if(! labTrade)
		{
			LG("gui", "frmStore:labTrade not found. \n");
			return false;
		}

		// ÊÔ´©°´Å¥×Ö
		labTryon = dynamic_cast<CLabelEx*>(frmStore->Find("labTryon"));
		if(! labTryon)
		{
			LG("gui", "frmStore:labTryon not found. \n");
			return false;
		}

		// È«²¿ÏÔÊ¾°´Å¥×Ö
		labViewAll = dynamic_cast<CLabelEx*>(frmStore->Find("labViewAll"));
		if(! labViewAll)
		{
			LG("gui", "frmStore:labViewAll not found. \n");
			return false;
		}

		// ¸öÈËÐÅÏ¢°ïÖú
		memStoreHelp = dynamic_cast<CMemo*>(frmStore->Find("memStoreHelp"));
		if(! memStoreHelp)
		{
			LG("gui", "frmStore:memStoreHelp not found. \n");
			return false;
		}

		imgBackGround10 = dynamic_cast<CImage*>(frmStore->Find("imgBackGround10"));
		if(! imgBackGround10)
		{
			LG("gui", "frmStore:imgBackGround10 not found. \n");
			return false;
		}

		// ÉÌ³ÇÉÌÆ·ÁÐ±í
		char szName[32] = {0};
		for(int i = 0; i < STORE_PAGE_SIZE; ++i)
		{
			sprintf(szName, "labName_%d", i);
			m_stStoreGui[i].labName = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labName)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "labPrice_%d", i);
			m_stStoreGui[i].labPrice = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labPrice)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "labLeftTime_%d", i);
			m_stStoreGui[i].labLeftTime = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labLeftTime)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "labLeftNum_%d", i);
			m_stStoreGui[i].labLeftNum = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labLeftNum)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "labRemark_%d", i);
			m_stStoreGui[i].labRemark = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labRemark)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "labRightClickView_%d", i);
			m_stStoreGui[i].labRightClickView = dynamic_cast<CLabelEx*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].labRightClickView)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "cmdStore_%d", i);
			m_stStoreGui[i].cmdStore = dynamic_cast<COneCommand*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].cmdStore)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}
			m_stStoreGui[i].cmdStore->SetIsDrag(false);	// ²»ÔÊÐíÍÏ¶¯

			sprintf(szName, "imgSquare_%d", i);
			m_stStoreGui[i].imgSquare = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgSquare)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "btnBlue_%d", i);
			m_stStoreGui[i].btnBlue = dynamic_cast<CTextButton*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].btnBlue)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}
			m_stStoreGui[i].btnBlue->evtMouseRClick  = _evtStoreListMouseRClick;
			m_stStoreGui[i].btnBlue->evtMouseDBClick = _evtStoreListMouseDBClick;

			sprintf(szName, "imgCutLine_%d", i);
			m_stStoreGui[i].imgCutLine = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgCutLine)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "imgHot_%d", i);
			m_stStoreGui[i].imgHot = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgHot)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "imgNew_%d", i);
			m_stStoreGui[i].imgNew = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgNew)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "imgBlue_%d", i);
			m_stStoreGui[i].imgBlue = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgBlue)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}
			
			sprintf(szName, "imgMoney_%d", i);
			m_stStoreGui[i].imgMoney = dynamic_cast<CImage*>(frmStore->Find(szName));
			if(! m_stStoreGui[i].imgMoney)
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}
		} // end for(0..4)
		ClearStoreTreeNode();
		ClearStoreItemList();


		//
		// ÉÌ³Ç½ø¶ÈÌõ
		//
		frmStoreLoad = CFormMgr::s_Mgr.Find("frmStoreLoad");
		if(!frmStoreLoad) return false;
		frmStoreLoad->evtClose = _evtStoreLoadFormClose;

		proStoreLoad = dynamic_cast<CProgressBar*>(frmStoreLoad->Find("proStoreLoad"));
		if(!proStoreLoad) return false;
		proStoreLoad->evtTimeArrive = _evtProTimeArriveEvt;


		//
		// ²é¿´ÏéÏ¸
		//
		frmViewAll = CFormMgr::s_Mgr.Find("frmViewAll");
		if(! frmViewAll)
		{
			LG("gui", "frmViewAll not found.\n");
			return false;
		}
		frmViewAll->evtLost = _evtStoreViewAllLostEvent;

		memViewAll = dynamic_cast<CMemo*>(frmViewAll->Find("memViewAll"));
		if(! memViewAll)
		{
			LG("gui", "frmViewAll:memViewAll not found.\n");
			return false;
		}

		for(int i = 0; i < STORE_ITEM_COUNT; ++i)
		{
			sprintf(szName, "imgSquareViewAll_%d", i);
			imgSquareViewAll[i] = dynamic_cast<CImage*>(frmViewAll->Find(szName));
			if(! imgSquareViewAll[i])
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			sprintf(szName, "cmdSquareIcon_%d", i);
			cmdSquareIcon[i] = dynamic_cast<COneCommand*>(frmViewAll->Find(szName));
			if(! cmdSquareIcon[i])
			{
				LG("gui", "frmStore:%s not found. \n", szName);
				return false;
			}

			cmdSquareIcon[i]->SetIsDrag(false);
		}


		//
		// ÊÔ´©½çÃæ
		//
		frmTryon = mgr.Find("frmTryon");
		if(!frmTryon)
		{
			LG("gui", "frmTryon not found.\n");
			return false;
		}
		frmTryon->evtClose = _evtTryonFormClose;

		// »æÖÆ½ÇÉ«3D¿Ø¼þ
		ui3dplayer = dynamic_cast<C3DCompent*>(frmTryon->Find("ui3dplayer"));
		if(! ui3dplayer)
		{
			LG("gui", "frmStore:ui3dplayer not found. \n");
			return false;
		}
		ui3dplayer->SetRenderEvent(_evtChaTryonRenderEvent);

		CTextButton* btnClearAll = dynamic_cast<CTextButton*>(frmTryon->Find("btnClearAll"));
		if(btnClearAll)
		{
			btnClearAll->evtMouseClick	= _evtChaEquipClearAll;
		}

		CTextButton* btnTurnleft3d = dynamic_cast<CTextButton*>(frmTryon->Find("btnTurnleft3d"));
		if(btnTurnleft3d)
		{
			btnTurnleft3d->evtMouseClick        = _evtChaLeftRotate;
			btnTurnleft3d->evtMouseDownContinue = _evtChaLeftContinueRotate;
		}

		CTextButton* btnTurnright3d = dynamic_cast<CTextButton*>(frmTryon->Find("btnTurnright3d"));
		if(btnTurnright3d)
		{
			btnTurnright3d->evtMouseClick        = _evtChaRightRotate;
			btnTurnright3d->evtMouseDownContinue = _evtChaRightContinueRotate;
		}

		return true;
	}


	void CStoreMgr::CloseForm()
	{
		//if(frmStore)
		//{
		//	frmStore->SetIsShow(false);
		//}
	}


	void CStoreMgr::SwitchMap()
	{
		m_pCurrMainCha = nullptr;
		ShowStoreForm(false);
	}


	void CStoreMgr::FrameMove(DWORD dwTime)
	{
		if(frmStoreLoad->GetIsShow())
		{
			if(GetTickCount() - _dwDarkScene > STORE_OPEN_TIMEOUT * 1000)
			{
				DarkScene(false);
				_dwDarkScene = 0;

				ShowStoreLoad(false);
			}
		}
	}


	// ´ò¿ªÉÌ³ÇÑ¯ÎÊ
	void CStoreMgr::OpenStoreAsk()
	{
		//CBoxMgr::ShowSelectBox(_evtStoreOpenCheckEvent, g_oLangRec.GetString(858), true);
		CS_StoreOpenAsk("");
	}


	void CStoreMgr::ShowStoreForm(bool bShow)
	{
		g_stUIDoublePwd.CloseAllForm();
		CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_STORE, ! bShow);		// Î÷ÃÅÎÄµµÐÞ¸Ä
		_SetIsShowCozeForm(! bShow);

		// ¹Ø±ÕLoading...
		ShowStoreLoad(false);

		if(bShow)
		{
			StoreTreeRefresh();
			_SetIsShowUserInfo(false);

			// ½ûÓÃÁÙÊ±±³°ü ESC ¹¦ÄÜ
			//frmTempBag->SetIsEscClose(false);

			frmStore->SetPos(0, 0);
			frmStore->Refresh();
			frmStore->SetIsShow(true);

			if(! frmTryon->GetIsShow())
			{
				// ÏÔÊ¾ÊÔ´©½çÃæ
				ShowTryonForm(true);
			}

			// ÏÔÊ¾ÁÙÊ±±³°ü
			//ShowTempKitbag(true);

			CUIInterface::MainChaMove();
		}
		else
		{
			frmStore->SetIsShow(false);

			ClearStoreTreeNode();
			ClearStoreItemList();
		}
	}


	void CStoreMgr::ShowStoreLoad(bool bShow)
	{
		if(bShow)
		{
			proStoreLoad->SetRange(0.0f, STORE_OPEN_TIMEOUT * 1.0f);
			proStoreLoad->SetPosition(0.0f);

			frmStoreLoad->SetPos((g_pGameApp->GetWindowWidth()  - frmStoreLoad->GetWidth())  >> 1,
								 (g_pGameApp->GetWindowHeight() - frmStoreLoad->GetHeight()) >> 1);
			frmStoreLoad->Refresh();
			frmStoreLoad->ShowModal();

			_dwDarkScene = GetTickCount();
		}
		else
		{
			frmStoreLoad->SetIsShow(false);
		}

		DarkScene(bShow);
	}


	void CStoreMgr::ShowViewAllForm(bool bShow)
	{
		if(! bShow)
		{
			frmViewAll->SetIsShow(false);
			return;
		}

		if(m_nCurSel < 0 || m_nCurSel >= STORE_PAGE_SIZE)
		{
			return;
		}

		for(int i = 0; i < STORE_ITEM_COUNT; ++i)
		{
			imgSquareViewAll[i]->SetIsShow(false);
			cmdSquareIcon[i]->SetIsShow(false);
			cmdSquareIcon[i]->DelCommand();

			if(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID > 0)
			{
				CItemRecord* pInfo = GetItemRecordInfo(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID);
				if(! pInfo) continue;

				CItemCommand* pItem = new CItemCommand(pInfo);
				SItemGrid& oItemGrid = pItem->GetData();

				oItemGrid.sNum = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemNum;
				for(int j = 0; j < defITEM_INSTANCE_ATTR_NUM; ++j)
				{
					oItemGrid.sInstAttr[j][0] = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemAttrID[j];
					oItemGrid.sInstAttr[j][1] = m_stStoreInfo[m_nCurSel].comItemInfo[i].itemAttrVal[j];
				}

				// ÎïÆ·²ÛÊý
				unsigned long ulForgeP = oItemGrid.GetDBParam(enumITEMDBP_FORGE);
				short sFlute = (short)(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemFlute);
				short sHole  = (short)(ulForgeP / 1000000000);
				ulForgeP = ulForgeP + (sFlute - sHole) * 1000000000;
				oItemGrid.SetDBParam(enumITEMDBP_FORGE, (long)ulForgeP);

				cmdSquareIcon[i]->AddCommand(pItem);
				cmdSquareIcon[i]->SetIsShow(true);
				imgSquareViewAll[i]->SetIsShow(true);
			}
		}

		memViewAll->SetCaption(m_stStoreInfo[m_nCurSel].comRemark);
		memViewAll->ProcessCaption();
		memViewAll->Refresh();

		// ¾ÓÖÐÏÔÊ¾
		frmViewAll->SetPos((g_pGameApp->GetWindowWidth()  - frmViewAll->GetWidth())  >> 1 , 
						   (g_pGameApp->GetWindowHeight() - frmViewAll->GetHeight()) >> 1);
		frmViewAll->Refresh();
		frmViewAll->SetIsShow(true);
	}


	// ÏÔÊ¾ÍøÒ³ÉÌ³Ç
	void CStoreMgr::ShowStoreWebPage()
	{
		extern Cooperate g_cooperate;
		if(g_cooperate.code == COP_CGA) // ºÆ·½ÌØÊâ´¦Àí
		{
            std::string strURL = "http://pv.cga.com.cn/counter.asp?id=720";

            ShellExecute(0, "open", strURL.c_str(), 
                        nullptr, nullptr, SW_SHOW);
        }
        else
        {
		    /*ShellExecute(0, "open",
				    g_oLangRec.GetString(938), // "http://cache.moliyo.com/shop_77/",
				    nullptr, nullptr, SW_SHOW);*/
			//NetColourInfo( 0x9576D1, "Ingame shop currently offline" );
        }
	}


	// ÏÔÊ¾ÊÔ´©
	void CStoreMgr::ShowTryonForm(bool bShow)
	{
		if(bShow)
		{
			CCharacter* pMainCha = g_stUIBoat.GetHuman();
			if(! pMainCha)
				return;

			if(false == frmTryon->GetIsShow())
			{
				// µÚÒ»´ÎÏÔÊ¾Ê±ÉèÖÃÈËÎïÕýÃæ
				m_nChaRotate = 0;

				m_pCurrMainCha = pMainCha;

				m_sLookInfo.SLook = pMainCha->GetPart();
				m_sLookInfo.chSynType = enumSYN_LOOK_SWITCH;

				m_sCurLookInfo = m_sLookInfo;

				m_isFight = m_pCurrMainCha->GetIsFight();
				m_pCurrMainCha->FightSwitch(true);	// Ç¿ÖÆ´ò¿ªµ±Ç°½ÇÉ«Õ½¶·×´Ì¬

				frmTryon->SetPos(frmStore->GetX2(), frmStore->GetY());
				frmTryon->Refresh();
				frmTryon->SetIsShow(true);
				return;
			}

			//
			// ×°±¸ÊÔ´©
			bool bAllowEquip = false;
			DWORD dwBodyType = m_pCurrMainCha->GetDefaultChaInfo()->lID;

			if(0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE)
			{
				bool leftHand = true;
				for(int i = 5; i >= 0; --i){
					if(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID <= 0)
						continue;

					//CItemRecord* pItem = pItemCmd->GetItemInfo();
					CItemRecord* pItem = GetItemRecordInfo(m_stStoreInfo[m_nCurSel].comItemInfo[i].itemID);
					if(! pItem)
						continue;

					short sType = pItem->sType;
					if(1 <= sType && sType <= 11)	// ÎäÆ÷
					{
						
						if(! pItem->IsAllowEquip(dwBodyType)){
							continue;
						}
						
						int slot{};
						int unequip{};
						int appslot{};
						
						if (sType == enumItemTypeSword){
							if(!leftHand){
								appslot = enumEQUIP_SWORD2APP;
								slot = enumEQUIP_LHAND;
							}else{
								try	//crash here 
								{
									appslot = enumEQUIP_SWORD1APP;
									slot = enumEQUIP_RHAND;
									leftHand = false;	
									if(m_sCurLookInfo.SLook.SLink[enumEQUIP_LHAND].sID!=0)
									{
										CItemRecord* pItem2 = GetItemRecordInfo(m_sCurLookInfo.SLook.SLink[enumEQUIP_LHAND].sID);
										if(pItem2->sType != enumItemTypeSword && pItem2->sType != enumItemTypeShield){
											unequip = enumEQUIP_LHAND;
										}
										
										
									}		
								}
								catch (...)
								{
		
								}		//try end 						

							}
						}else if (sType == enumItemTypeGlave){
							appslot = enumEQUIP_GREATSWORDAPP;
							slot = enumEQUIP_LHAND;
							unequip = enumEQUIP_RHAND;
						}else if (sType == enumItemTypeBow){
							appslot = enumEQUIP_BOWAPP;
							slot = enumEQUIP_LHAND;
							unequip = enumEQUIP_RHAND;
						}else if (sType == enumItemTypeHarquebus){
							appslot = enumEQUIP_GUNAPP;
							slot = enumEQUIP_RHAND;
							unequip = enumEQUIP_LHAND;
						}else if (sType == enumItemTypeStylet){
							appslot = enumEQUIP_DAGGERAPP;
							slot = enumEQUIP_LHAND;
							unequip = enumEQUIP_RHAND;
						}else if (sType == enumItemTypeCosh){
							appslot = enumEQUIP_STAFFAPP;
							slot = enumEQUIP_LHAND;
							unequip = enumEQUIP_RHAND;
						}else if (sType == enumItemTypeShield){
							appslot = enumEQUIP_SHIELDAPP;
							slot = enumEQUIP_LHAND;
							if(m_sCurLookInfo.SLook.SLink[enumEQUIP_RHAND].sID != 0){
								CItemRecord* pItem2 = GetItemRecordInfo(m_sCurLookInfo.SLook.SLink[enumEQUIP_RHAND].sID);
								if(pItem2->sType != enumItemTypeSword){
									unequip = enumEQUIP_RHAND;
								}
							}
						}
						
						if(appslot){
							m_sCurLookInfo.SLook.SLink[appslot].sID  = 0;
							m_sCurLookInfo.SLook.SLink[appslot].sNum = 0;
						}
						
						if(unequip){
							m_sCurLookInfo.SLook.SLink[unequip].sID  = 0;
							m_sCurLookInfo.SLook.SLink[unequip].sNum = 0;
						}
							
						if(slot){
							m_sCurLookInfo.SLook.SLink[slot].sID  = (short)pItem->lID;
							m_sCurLookInfo.SLook.SLink[slot].sNum = 1;
							bAllowEquip = true;
						}
						
					}
					else if(sType == 20)	// Ã±×Ó
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}
						m_sCurLookInfo.SLook.SLink[enumEQUIP_HEAD] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_HEAD].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_HEADAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_HEADAPP].sNum = 0;
						bAllowEquip = true;
					}			
					else if(sType == 22 || sType == 27)	// ÒÂ·þ
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}
						m_sCurLookInfo.SLook.SLink[enumEQUIP_BODY] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_BODY].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_BODYAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_BODYAPP].sNum = 0;
						bAllowEquip = true;
					}
					else if(sType == 23)	// ÊÖÌ×
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}
						m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVE] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVE].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVEAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_GLOVEAPP].sNum = 0;
						bAllowEquip = true;
					}
					else if(sType == 24)	// Ð¬×Ó
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}
						m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOES] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOES].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOESAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_SHOESAPP].sNum = 0;
						bAllowEquip = true;
					}
					//@mothannakh allow face slot to try ingame shop 
					else if(sType == 21)	// face
					{
						if(! pItem->IsAllowEquip(dwBodyType))
						{
							continue;
						}	
						m_sCurLookInfo.SLook.SLink[enumEQUIP_FACE] = (short)pItem->lID;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_FACE].sNum = 1;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_FACEAPP] = 0;
						m_sCurLookInfo.SLook.SLink[enumEQUIP_FACEAPP].sNum = 0;						
						bAllowEquip = true;						
					}						
					
					//customc code  end
				}
			}

			if(bAllowEquip)
			{
				//NetChangeChaPart(m_pCurrMainCha, m_sCurLookInfo);
				_ChangeChaPart(g_stUIStore.m_sCurLookInfo);

				m_pCurrMainCha->PlayPose(1, PLAY_LOOP_SMOOTH );
				m_pCurrMainCha->FightSwitch(true);	// Ç¿ÖÆ½ÇÉ«Õ½¶·×´Ì¬
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(882));
			}
		}
		else
		{
			frmTryon->SetIsShow(false);
		}
	}


	// ÏÔÊ¾Í¸Ã÷µ²°å
	void CStoreMgr::ShowAlphaMatteForm(bool bShow)
	{
		CForm* frmAlphaMatte = CFormMgr::s_Mgr.Find("frmAlphaMatte");
		if(frmAlphaMatte)
		{
			frmAlphaMatte->SetSize(g_pGameApp->GetWindowWidth(), g_pGameApp->GetWindowHeight());
			frmAlphaMatte->SetPos(0, 0);
			frmAlphaMatte->Refresh();

			frmAlphaMatte->SetIsShow(bShow);
		}
	}


	// ³¡¾°±äºÚ
	void CStoreMgr::DarkScene(bool bDark)
	{
		CForm* frmStoreDark = CFormMgr::s_Mgr.Find("frmStoreDark");
		if(frmStoreDark) frmStoreDark->SetIsShow(bDark);

		_dwDarkScene = bDark ? GetTickCount() : 0;
	}


	// ´ÓÁÙÊ±±³°üÈ¡³ö
	bool CStoreMgr::PopFromTempKitbag(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
		g_stUIStore.m_NetTempKitbag.sSrcGridID = rkDrag.GetDragIndex();
		g_stUIStore.m_NetTempKitbag.sTarGridID = nGridID;

		// ÅÐ¶ÏÎïÆ·ÊÇ·ñÊÇ¿ÉÖØµþµÄÎïÆ·
		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
		if (!pkItemCmd)	return false;

		if ( pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1 )
		{
			// È¡³ö¿ÉÒÔµþ·ÅµÄÎïÆ·
			m_pkNumberBox = 
				g_stUIBox.ShowNumberBox( _evtDragItemsEvent, pkItemCmd->GetTotalNum(), g_oLangRec.GetString(442), false);

			if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
				return false;
			else
				return true;
		}
		else
		{
			// È¡³öµ¥¸öÎïÆ·
			g_stUIStore.m_NetTempKitbag.sSrcNum = 1;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_KITBAGTMP_DRAG, (void*)&(g_stUIStore.m_NetTempKitbag));
			return true;
		}

		return true;
	}


	void CStoreMgr::_evtDragItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes) 
			return;

		int num =  g_stUIStore.m_pkNumberBox->GetNumber();
		if ( num > 0 )
		{
			g_stUIStore.m_NetTempKitbag.sSrcNum = num;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_KITBAGTMP_DRAG, (void*)&(g_stUIStore.m_NetTempKitbag));
		}
	}


	// Ìí¼ÓÉÌ³ÇÊ÷½áµã£¨¸¸½áµãÎªÁã±íÊ¾¸ù½áµã£©
	void CStoreMgr::AddStoreTreeNode(long nParentID, long nID, const char* szCaption)
	{
		static const char file[] = "texture/ui/store6.tga";
		static const int w = 127;
		static const int h = 20;
		static const int sx = 30;
		static const int sy = 114;
		static const int frame = 0;
		static const int TextX = 5;
		static const int TextY = 5;
		static const int maxcol = 1;
		static const int uw = 32;
		static const int uh = 32;
		static const int nWidth  = trvStore->GetWidth() - trvStore->GetScroll()->GetWidth()-28;
		static const int nHeight = 16 + 3;

		m_mapNode[nID] = std::string(szCaption);

		if(0 == nParentID)
		{
			// ¸¸½áµã
			CNoteGraph* item =  new CNoteGraph( frame );
			item->GetImage()->LoadAllImage( file, w, h, sx, sy );
			item->GetImage()->SetScale( w, h );
			item->SetString( szCaption );
			item->SetTextX( TextX );
			item->SetTextY( TextY );

			CTreeGridNode* obj = new CTreeGridNode(trvStore, item);
			obj->SetIsExpand(false);
			obj->SetColMaxNum(maxcol);
			obj->SetUnitSize(uw, uh);

			CTreeNodeObj* pRootNode = trvStore->GetRootNode();
			pRootNode->AddNode(obj);

			obj->GetItem()->SetColor( COLOR_BLACK );
			obj->GetUpImage()->UnLoadImage();
			obj->GetDownImage()->UnLoadImage();
			obj->SetUnitSize(nWidth, nHeight );
			obj->SetColMaxNum(1);
		}
		else
		{
			// Ò¶×Ó½áµã
			MapNodeIter iterNode = m_mapNode.find(nParentID);

			if(iterNode != m_mapNode.end())
			{
				CTreeGridNode* pGridNode = dynamic_cast<GUI::CTreeGridNode*>(trvStore->GetRootNode()->FindNode(iterNode->second.c_str()));
				if(pGridNode)
				{
					CItem* pItem = new CItem();
					pItem->SetColor( COLOR_BLACK );
					pItem->SetString( szCaption );

					pGridNode->AddItem(pItem);
				}
			}
		}
	}


	// Ìí¼ÓÉÌ³ÇÉÌÆ·ÐÅÏ¢
	void CStoreMgr::AddStoreItemInfo(long nSeq, long nID, const char* szName, long nPrice, const char* szRemark, bool isHot, long nTime, long nRemainNum, long nRemainTime)
	{
		if(0 > nSeq || nSeq >= STORE_PAGE_SIZE)
		{
			return;
		}

		char szTemp[128] = { 0 };
		//split names to fit box size @mothannakh
		StringNewLine(szTemp, 20, szName, strlen(szName));
		m_stStoreGui[nSeq].labName->SetCaption(szTemp);		
		m_stStoreGui[nSeq].labName->SetIsShow(true);

		m_stStoreGui[nSeq].labPrice->SetCaption(StringSplitNum(nPrice, 3, ',')); 
		m_stStoreGui[nSeq].labPrice->SetIsShow(true);

		std::string description(szRemark);
		constexpr int descriptionBoundries = 25;
		//_snprintf_s(szTemp, sizeof(szTemp), _TRUNCATE, "%s", StringLimit(szRemark, 25).c_str());
		if (description.length() > descriptionBoundries) {
			description = description.substr(0, descriptionBoundries - 3) + "...";
		}

		_snprintf_s(szTemp, sizeof(szTemp), _TRUNCATE, "Quantity: %d", nRemainNum); // "%d¸ö"
		m_stStoreGui[nSeq].labLeftNum->SetCaption(nRemainNum >= 0 ? szTemp : "No Quantity Limit");
		m_stStoreGui[nSeq].labLeftNum->SetIsShow(true);

		_snprintf_s(szTemp, sizeof(szTemp), _TRUNCATE, g_oLangRec.GetString(911), nRemainTime);// "%dÐ¡Ê±"
		m_stStoreGui[nSeq].labLeftTime->SetCaption(nRemainTime >= 0 ? szTemp : "No Time Limit");
		m_stStoreGui[nSeq].labLeftTime->SetIsShow(true);

		m_stStoreGui[nSeq].btnBlue->SetIsShow(true);			// µãÑ¡°´Å¥
		m_stStoreGui[nSeq].labRightClickView->SetIsShow(true);
		
		//m_stStoreGui[nSeq].labLeftNum->SetIsShow(false);
		//m_stStoreGui[nSeq].labLeftTime->SetIsShow(false);

		m_stStoreGui[nSeq].imgMoney->SetIsShow(true);
		if(isHot)
		{
			// ÈÈÂôÉÌÆ·
			m_stStoreGui[nSeq].imgHot->SetIsShow(true);
		}
		else
		{
			time_t now;
			time(& now);
			int nDay = ((long)(now) - nTime) / (24 * 3600 * 1000);

			if(nDay < 15)
			{
				// ÐÂÉÌÆ·
				m_stStoreGui[nSeq].imgNew->SetIsShow(true);
			}
		}

		m_stStoreInfo[nSeq].comID     = nID;
		m_stStoreInfo[nSeq].comExpire = nTime;
		m_stStoreInfo[nSeq].comPrice  = nPrice;
		m_stStoreInfo[nSeq].comTime   = nTime;
		m_stStoreInfo[nSeq].isHot     = isHot;

		m_stStoreInfo[nSeq].comExpire = nRemainNum;	// ÏÞÊ±
		m_stStoreInfo[nSeq].itemNum   = nRemainTime;// ÏÞÁ¿

		strncpy_s(m_stStoreInfo[nSeq].comName, szName, _TRUNCATE);     // 
		strncpy_s(m_stStoreInfo[nSeq].comRemark, szRemark, _TRUNCATE); // 
	}


	// Ìí¼ÓÉÌ³ÇÉÌÆ·Ï¸½ÚÐÅÏ¢
	void CStoreMgr::AddStoreItemDetail(long nSeq, long nSubSeq, short sItemID, short sItemNum, short sFlute, short pItemAttrID[], short pItemAttrVal[])
	{
		if(0 > nSeq || nSeq >= STORE_PAGE_SIZE)
		{
			return;
		}

		if(0 > nSubSeq || nSubSeq >= 6)
		{
			return;
		}

		CItemRecord*  pInfo(nullptr);
		CItemCommand* pItem(nullptr);

		pInfo = GetItemRecordInfo(sItemID);
		if (!pInfo) return;

		// ÔÚÉÌ³ÇÄÚ²¿Ò²±£ÁôÒ»·ÝÉÌÆ·µÄÏéÏ¸Êý¾Ý
		m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemID = sItemID;
		m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemNum = sItemNum;
		m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemFlute = sFlute;
		memcpy(m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemAttrID,  pItemAttrID,  sizeof(short) * 5);
		memcpy(m_stStoreInfo[nSeq].comItemInfo[nSubSeq].itemAttrVal, pItemAttrVal, sizeof(short) * 5);

		if(nSubSeq != 0)	// Ö»Ìí¼ÓµÚÒ»¸ö£¬²»ÊÇµÚÒ»¸ö²»Ìí¼Ó
		{
			return;
		}

		pItem = new CItemCommand(pInfo);
		//pItem->GetData().sNum = sItemNum;	// µ±Ç°ÉÌÆ·µ¥¸ö¸ñ×ÓµÄ¸öÊý
		pItem->SetOwnDefText("");			// ²»ÏÔÊ¾ÈÎºÎ×Ô¶¨ÒåÄÚÈÝ

		// ÊôÐÔ
		SItemGrid& oItemGrid = pItem->GetData();
		for(int i = 0; i < defITEM_INSTANCE_ATTR_NUM; ++i)
		{
			oItemGrid.sInstAttr[i][0] = pItemAttrID[i];
			oItemGrid.sInstAttr[i][1] = pItemAttrVal[i];
		}

		// ÎïÆ·²ÛÊý
		unsigned long ulForgeP = oItemGrid.GetDBParam(enumITEMDBP_FORGE);
		short sHole = (short)(ulForgeP / 1000000000);
		ulForgeP = ulForgeP + (sFlute - sHole) * 1000000000;
		oItemGrid.SetDBParam(enumITEMDBP_FORGE, (long)ulForgeP);

		m_stStoreGui[nSeq].cmdStore->AddCommand(pItem);
		m_stStoreGui[nSeq].cmdStore->SetIsShow(true);
		m_stStoreGui[nSeq].imgSquare->SetIsShow(true);
	}


	// Ìí¼ÓÓÃ»§ÉèÖÃ½áµã
	void CStoreMgr::AddStoreUserTreeNode(void)
	{
		//disabled help / management
		//AddStoreTreeNode(0, USER_NODEID, g_oLangRec.GetString(906));
		//AddStoreTreeNode(0, HELP_NODEID, g_oLangRec.GetString(921));
	}


	// ÉèÖÃÉÌ³ÇÁÐ±íÒ³
	void CStoreMgr::SetStoreItemPage(long nCurPage, long nMaxPage)
	{
		m_nCurPage = nCurPage;
		m_nMaxPage = nMaxPage;

		if(m_nMaxPage > 0)
		{
			char szBuffer[32] = {0};
			sprintf(szBuffer, "%d/%d", nCurPage, nMaxPage);
			labListPage->SetCaption(szBuffer);

			btnLeftPage->SetIsShow(true);
			btnRightPage->SetIsShow(true);
			labListPage->SetIsShow(true);
		}
		else
		{
			btnLeftPage->SetIsShow(false);
			btnRightPage->SetIsShow(false);
			labListPage->SetIsShow(false);
		}
	}


	// ÉèÖÃÄ§¶¹ÊýÁ¿
	void CStoreMgr::SetStoreMoney(long nMoBean, long nRplMoney)
	{
		char szBuffer[32] = {0};

		if(nMoBean >= 0)
		{
			sprintf(szBuffer, "%ld", nMoBean);
			labBeanLeft->SetCaption(szBuffer);
		}

		if(nRplMoney >= 0)
		{
			sprintf(szBuffer, "%ld", nRplMoney);
			labMoneyLeft->SetCaption(szBuffer);
		}
	}


	// ÉèÖÃVIPÖµ
	void CStoreMgr::SetStoreVip(long nVip)
	{
		//CTextButton* btnToVip = dynamic_cast<CTextButton*>(frmStore->Find("btnToVIP"));
		if(nVip)
		{
			//if(btnToVip) btnToVip->SetIsEnabled(false);
			labMemberStyle->SetCaption(g_oLangRec.GetString(902)); // °×½ð»áÔ±
		}
		else
		{
			//if(btnToVip) btnToVip->SetIsEnabled(true);
			labMemberStyle->SetCaption(g_oLangRec.GetString(903)); // ÆÕÍ¨»áÔ±
		}

		m_nVip = nVip;
	}


	// Çå³ýÈ«²¿µÄÉÌ³ÇÊ÷½áµã
	void CStoreMgr::ClearStoreTreeNode()
	{
		m_mapNode.clear();
		trvStore->ClearAllNode();

		m_nCurClass = -1;
	}


	// Çå³ýÈ«²¿ÉÌ³ÇÎïÆ·ÁÐ±í
	void CStoreMgr::ClearStoreItemList()
	{
		for(int i = 0; i < STORE_PAGE_SIZE; ++i)
		{
			m_stStoreGui[i].SetIsShow(false);
			m_stStoreInfo[i].Clear();
		}

		btnTrade->SetIsEnabled(false);
		btnTryon->SetIsEnabled(false);
		btnViewAll->SetIsEnabled(false);

		this->_SetIsShowUserInfo(false);

		m_nCurSel = -1;
	}


	// ÉèÖÃÉÌ³Ç°´Å¥ÊÇ·ñ¿ÉÓÃ
	void CStoreMgr::SetStoreBuyButtonEnable(bool b)
	{
		btnTrade->SetIsEnabled(b);
	}


	// »ñµÃµ±Ç°Ñ¡ÖÐID
	int CStoreMgr::GetCurSelItemID()
	{
		if(0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE)
		{
			return m_stStoreInfo[m_nCurSel].comID;
		}

		return -1;
	}


	// ÖØÖÃ×îºó²Ù×÷Ê±¼ä£¬10 Ãë¼ä¸ô´¦Àí
	bool CStoreMgr::ResetLastOperate(bool bSilent)
	{
		static DWORD dwLast = 0;
		DWORD dwCurr = GetTickCount();
		if(dwCurr - dwLast < 10000)
		{
			if(! bSilent)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(895));	// ÉÌ³Ç²Ù×÷¹ýÓÚÆµ·±£¬ÇëÉÔºóÔÙÊÔ£¡
			}

			return false;
		}

		dwLast = dwCurr;
		return true;
	}


	// Ë¢ÐÂÉÌ³ÇÊ÷
	void CStoreMgr::StoreTreeRefresh()
	{
		static const char imagefile[] = "texture/ui/store6.tga";
		static const int w = 30;
		static const int h = 20;
		static const int sx = 0;//88;
		static const int sy_a = 114;//76;
		static const int sy_s = 134;//96;
		static const int itemw = 30;
		static const int itemh = 20;

		trvStore->GetAddImage()->LoadImage(imagefile, w, h, 0, sx, sy_a);
		trvStore->GetAddImage()->SetScale( itemw, itemh );

		trvStore->GetSubImage()->LoadImage(imagefile, w, h, 0, sx, sy_s);
		trvStore->GetSubImage()->SetScale( itemw, itemh );

		trvStore->Refresh();
	}


	// ÊÇ·ñÏÔÊ¾ÓÃ»§ÐÅÏ¢
	void CStoreMgr::_SetIsShowUserInfo(bool bShow)
	{
		if(bShow)
		{
			labListPage->SetIsShow(! bShow);	// ÉÌ³ÇÒ³±êÇ©
			btnLeftPage->SetIsShow(! bShow);	// ÉÌ³ÇÒ³×ó°´Å¦
			btnRightPage->SetIsShow(! bShow);	// ÉÌ³ÇÒ³ÓÒ°´Å¦

			for(int i = 0; i < STORE_PAGE_SIZE; ++i)
			{
				m_stStoreGui[i].SetIsShow(! bShow);
			}
		}

		btnTrade->SetIsShow(! bShow);		// ½»Ò×°´Å¦
		btnTryon->SetIsShow(! bShow);		// ÊÔ´©°´Å¥
		btnViewAll->SetIsShow(! bShow);
		labTrade->SetIsShow(! bShow);		// ½»Ò×°´Å¥×Ö
		labTryon->SetIsShow(! bShow);		// ÊÔ´©°´Å¥×Ö
		labViewAll->SetIsShow(! bShow);

		labNameTitle->SetIsShow(! bShow);
		labPriceTitle->SetIsShow(! bShow);
		labLeftTimeTitle->SetIsShow(! bShow);
		labLeftNumTitle->SetIsShow(! bShow);

		memStoreHelp->SetIsShow(false);

		imgBackGround10->SetIsShow(! bShow);	// ¹Ø¼üµÄ°×É«µ²°å
	}

	void CStoreMgr::_SetIsShowHelpInfo(bool bShow)
	{
		if(bShow)
		{
			labListPage->SetIsShow(! bShow);	// ÉÌ³ÇÒ³±êÇ©
			btnLeftPage->SetIsShow(! bShow);	// ÉÌ³ÇÒ³×ó°´Å¦
			btnRightPage->SetIsShow(! bShow);	// ÉÌ³ÇÒ³ÓÒ°´Å¦

			for(int i = 0; i < STORE_PAGE_SIZE; ++i)
			{
				m_stStoreGui[i].SetIsShow(! bShow);
			}
		}

		btnTrade->SetIsShow(! bShow);		// ½»Ò×°´Å¦
		btnTryon->SetIsShow(! bShow);		// ÊÔ´©°´Å¥
		btnViewAll->SetIsShow(! bShow);
		labTrade->SetIsShow(! bShow);		// ½»Ò×°´Å¥×Ö
		labTryon->SetIsShow(! bShow);		// ÊÔ´©°´Å¥×Ö
		labViewAll->SetIsShow(! bShow);

		labNameTitle->SetIsShow(! bShow);
		labPriceTitle->SetIsShow(! bShow);
		labLeftTimeTitle->SetIsShow(! bShow);
		labLeftNumTitle->SetIsShow(! bShow);

		_LoadStoreHelpInfo();
		memStoreHelp->SetIsShow(bShow);

		imgBackGround10->SetIsShow(bShow);	// ¹Ø¼üµÄ°×É«µ²°å
	}

	void CStoreMgr::_SetIsShowCozeForm(bool bShow)
	{
		CForm* frmMainChat = _FindForm("frmMain800");
		if( !frmMainChat ) return;

		CList* lstOnSay = dynamic_cast<CList*>(frmMainChat->Find("lstOnSay"));
		if(lstOnSay) lstOnSay->SetIsShow(bShow);

		CList* lstOnSaySystem = dynamic_cast<CList*>(frmMainChat->Find("lstOnSaySystem"));
		if(lstOnSaySystem) lstOnSaySystem->SetIsShow(bShow);

		CDragTitle* drpTitle = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitle"));
		if(drpTitle) drpTitle->SetIsShow(bShow);

		CDragTitle* drpTitleSystem = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitleSystem"));
		if(drpTitleSystem) drpTitleSystem->SetIsShow(bShow);
		//fix discord channel black screen when open ingame shop // mothannakh
		//list
		/*CList* lstOnSayDiscord = dynamic_cast<CList*>(frmMainChat->Find("lstOnSayDiscord"));
		if (lstOnSayDiscord) lstOnSayDiscord->SetIsShow(bShow);
			//drop title 
		CDragTitle* drpTitleDiscord = dynamic_cast<CDragTitle*>(frmMainChat->Find("drpTitleDiscord"));
		if (drpTitleDiscord) drpTitleDiscord->SetIsShow(bShow);*/
	}

	void CStoreMgr::_RefreshStoreListHighLight()
	{
		for(int i = 0; i < CStoreMgr::STORE_PAGE_SIZE; ++i)
		{
			g_stUIStore.m_stStoreGui[i].imgBlue->SetIsShow(i == g_stUIStore.m_nCurSel ? true : false);
		}

		if(0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE)
		{
			g_stUIStore.btnTrade->SetIsEnabled(true);
			g_stUIStore.btnTryon->SetIsEnabled(true);
			g_stUIStore.btnViewAll->SetIsEnabled(true);
		}
		else
		{
			g_stUIStore.btnTrade->SetIsEnabled(false);
			g_stUIStore.btnTryon->SetIsEnabled(false);
			g_stUIStore.btnViewAll->SetIsEnabled(false);
		}
	}


	void CStoreMgr::_ShowTradeSelectBox()
	{
		if(0 <= m_nCurSel && m_nCurSel < STORE_PAGE_SIZE &&
			GetCurSelItemID() > 0)
		{
			if(0 == m_nVip && _IsCurSelVipNode())
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(913)); // VIP×¨Çø½öÏÞVIP¸ß¼¶ÓÃ»§¹ºÂò£¡
				return;
			}

			char szTitle[256] = {0};
			sprintf(szTitle, "%s%s: %s\n%s: %s", 
					g_oLangRec.GetString(857), 
					g_oLangRec.GetString(845), g_stUIStore.m_stStoreGui[g_stUIStore.m_nCurSel].labName->GetCaption(), 
					g_oLangRec.GetString(846), g_stUIStore.m_stStoreGui[g_stUIStore.m_nCurSel].labPrice->GetCaption());

			CBoxMgr::ShowSelectBox(_evtTradeCheckEvent, szTitle, true);
		}
	}

	void CStoreMgr::_LoadStoreHelpInfo()
	{
		// °ïÖú
		static bool IsFirst = true;
		if( IsFirst )
		{
			IsFirst = false;
			FILE* fp = fopen( "./scripts/txt/StoreHelp.tx", "rt" );
			if( fp )
			{
				if( fseek(fp, 0, SEEK_END) == 0 )
				{
					long size = ftell( fp );
					fseek(fp, 0, SEEK_SET);

					NET_HELPINFO Info;
					memset( &Info, 0, sizeof(NET_HELPINFO) );
					fread(Info.szDesp, size > HELPINFO_DESPSIZE - 1 ? HELPINFO_DESPSIZE - 1 : size, 1, fp);

					memStoreHelp->SetCaption(Info.szDesp);
					memStoreHelp->ProcessCaption();
					memStoreHelp->Refresh();
				}
				fclose( fp );
			}
			return;
		}
	}

	void CStoreMgr::_ChangeChaPart(stNetLookInfo& SLookInfo)
	{
		if( m_pCurrMainCha && SLookInfo.chSynType==enumSYN_LOOK_SWITCH )
		{
			if ( SLookInfo.SLook.sTypeID!=0 && m_pCurrMainCha->getTypeID()!=SLookInfo.SLook.sTypeID )
			{		
				if( SLookInfo.SLook.sTypeID!=0 && SLookInfo.SLook.sTypeID!=m_pCurrMainCha->getTypeID() ) 
					m_pCurrMainCha->ReCreate( SLookInfo.SLook.sTypeID );
			}

			m_pCurrMainCha->UpdataFace( SLookInfo.SLook );
		}
	}

	bool CStoreMgr::_IsCurSelVipNode(void)
	{
		MapNodeIter it = g_stUIStore.m_mapNode.find(m_nCurClass);
		if(it != m_mapNode.end())
		{
			std::string strName = it->second;
			if(strName.size() > 3)
			{
				strName = strName.substr(0, 3);
				if(0 == _stricmp(strName.c_str(), g_oLangRec.GetString(902))) // °×½ð»áÔ±
				{
					return true;
				}
			}
		}

		return false;
	}


	// ÉÌ³ÇÊ÷µÄÊó±êµã»÷ÊÂ¼þ
	void CStoreMgr::_evtStoreTreeMouseClick(CGuiData *pSender, int x, int y, DWORD key)
	{
		CItemObj* pSelItem = g_stUIStore.trvStore->GetHitItem(x, y);
		if(pSelItem)
		{
			std::string strItem = pSelItem->GetString();
			for(MapNodeIter it = g_stUIStore.m_mapNode.begin(); it != g_stUIStore.m_mapNode.end(); ++it)
			{
				if(it->second == strItem)
				{
					g_stUIStore.m_nCurClass = it->first;
					g_stUIStore.m_nCurPage  = 1;

					if(strItem == g_oLangRec.GetString(906))	// ¸öÈË¹ÜÀí
					{
						g_stUIStore._SetIsShowUserInfo(true);
					}
					else if(strItem == g_oLangRec.GetString(921))	// ÉÌ³Ç°ïÖú
					{
						g_stUIStore._SetIsShowHelpInfo(true);
					}
					else	// ÉÌÆ·ÀàÐÍ
					{
						g_stUIStore._SetIsShowUserInfo(false);
						CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage, STORE_PAGE_SIZE);
					}
					break;
				}
			}

			bool bRootNode = false;
			CTreeNodeRoot* pRoot = g_stUIStore.trvStore->GetRootNode();
			if(pRoot)
			{
				for(int i = 0; i < pRoot->GetChildCount(); ++i)
				{
					CTreeNodeObj* pChild = pRoot->GetChildNode(i);
					if(0 == strcmp(pChild->GetCaption(), strItem.c_str()))
					{
						bRootNode = true;
						break;
					}
				}

				if(bRootNode)
				{
					for(int i = 0; i < pRoot->GetChildCount(); ++i)
					{
						CTreeNodeObj* pChild = pRoot->GetChildNode(i);
						if(strcmp(pChild->GetCaption(), strItem.c_str()))
						{
							pChild->SetIsExpand(false);	// ÊÕÆðÒÑ´ò¿ªµÄ½áµã
						}
						else
						{
							pChild->SetIsExpand(true);
						}
					}
					g_stUIStore.trvStore->Refresh();
				}
			}
		}
	}


	// ÉÌ³Ç°´Å¥µã»÷ÊÂ¼þ
	void CStoreMgr::_evtStoreFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		std::string strName = pSender->GetName();
		if(strName == "btnLeftPage")	// Ç°Ò»Ò³
		{
			if(g_stUIStore.m_nCurPage > 1 && g_stUIStore.m_nCurClass > 0)
			{
				CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage - 1, STORE_PAGE_SIZE);
			}
		}
		else if(strName == "btnRightPage")	// ºóÒ»Ò³
		{
			if(g_stUIStore.m_nCurPage < g_stUIStore.m_nMaxPage && g_stUIStore.m_nCurClass > 0)
			{
				CS_StoreListAsk(g_stUIStore.m_nCurClass, (short)g_stUIStore.m_nCurPage + 1, STORE_PAGE_SIZE);
			}
		}
		else if(strName == "btnTrade")	// ¹ºÂò½»Ò×
		{
			g_stUIStore._ShowTradeSelectBox();
		}
		else if(strName == "btnTryon")	// ÊÔ´©
		{
			g_stUIStore.ShowTryonForm(true);
		}
		else if(strName == "btnViewAll")	// ÏÔÊ¾È«²¿µÀ¾ß
		{
			g_stUIStore.ShowViewAllForm(true);
		}
		else if(strName == "btnReceiveMoney")	// ¶Ò»»°´Å¥
		{
			if(g_stUIStore.imgBackGround10->GetIsShow()) return;
			g_stUIStore.m_pkExchangeNum = 
				g_stUIBox.ShowNumberBox( _evtExchangeEvent, -1, g_oLangRec.GetString(904), false); // ÇëÊäÈë¶Ò»»ÊýÁ¿
		}
		//else if(strName == "btnToVIP")	// ³ÉÎªVIP
		//{
		//	if(g_stUIStore.imgBackGround10->GetIsShow()) return;
		//	CBoxMgr::ShowSelectBox(_evtStoreToVipEvent, g_oLangRec.GetString(915), true); // È·¶¨Òª³ÉÎªVIP¸ß¼¶»áÔ±£¿
		//}
		else if(strName == "btnReceiveMoDou") // ¶Ò»»Ä¦¶¹
		{
			if(g_stUIStore.imgBackGround10->GetIsShow()) return;

			std::string strURL("");
			extern Cooperate g_cooperate;
			switch(g_cooperate.code)
			{
			case COP_OURGAME:
				strURL = "http://shop.ourgame.com/games/hdw/#cz";
				break;

            case COP_CGA:
                strURL = "http://pv.cga.com.cn/counter.asp?id=720";
                break;

			case COP_SINA:

			default:
				strURL = g_oLangRec.GetString(939); // "http://pay.moliyo.com/";
				break;
			}

			ShellExecute(0, "open",
					strURL.c_str(),
					nullptr, nullptr, SW_SHOW);	
		}
		else if(strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
				'0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE)	// Ñ¡ÖÐÉÌÆ·
		{
			g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
			g_stUIStore._RefreshStoreListHighLight();
		}
	/*	else if(strName == "btnRename")	{
			g_stUIBox.ShowNumberBox(_RenameBagOfHolding, -1,  "Enter New Name", false);
		}*/
	}
	
	/*void CStoreMgr::_RenameBagOfHolding(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey){
		if( nMsgType!=CForm::mrYes ) {
			return;
		}
		stNumBox* nameBox = (stNumBox*)pSender->GetForm()->GetPointer();
		if (!nameBox) return;
		char*  strName;
		//nameBox->GetString(strName);
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BAGOFHOLDINGNAME, static_cast<void*>(strName));
		CLabelEx* bagOfHoldingTitle = dynamic_cast<CLabelEx*>(g_stUIStore.frmBagOfHolding->Find("labTitle"));
		strName[17] = 0;
		if(strName[0] == 0){
			bagOfHoldingTitle->SetCaption("Bag of Holding");
		}else{
			bagOfHoldingTitle->SetCaption(strName);
		}
	}*/


	void CStoreMgr::_evtStoreListMouseRClick(CGuiData *pSender, int x, int y, DWORD key)
	{
		std::string strName = pSender->GetName();
		if(strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
				'0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE)	// Ñ¡ÖÐÉÌÆ·
		{
			g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
			g_stUIStore._RefreshStoreListHighLight();

			g_stUIStore.ShowViewAllForm();		// ÏÔÊ¾È«²¿
		}
	}


	void CStoreMgr::_evtStoreListMouseDBClick(CGuiData *pSender, int x, int y, DWORD key)
	{
		std::string strName = pSender->GetName();
		if(strName.size() > 0 && strName.substr(0, 8) == "btnBlue_" &&
				'0' <= strName[strName.size() - 1] && strName[strName.size() - 1] <= '0' + CStoreMgr::STORE_PAGE_SIZE)	// Ñ¡ÖÐÉÌÆ·
		{
			g_stUIStore.m_nCurSel = strName[strName.size() - 1] - '0';
			g_stUIStore._RefreshStoreListHighLight();

			//g_stUIStore.ShowTryonForm();		// ÊÔ´©
			g_stUIStore._ShowTradeSelectBox();	// ¹ºÂò
		}
	}


	// ÉÌ³Ç´ò¿ªÑ¯ÎÊ´¦Àí£¨ÒÑ×÷·Ï£¬¸ÄÓÃ¶þ´ÎÃÜÂëÌæ´úÖ®£©
	void CStoreMgr::_evtStoreOpenCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes)
		{
			return;
		}

		g_stUIDoublePwd.SetType(CDoublePwdMgr::STORE_OPEN_ASK);
		g_stUIDoublePwd.ShowDoublePwdForm();
	}


	// ³ÉÎªVIP È·ÈÏ
	void CStoreMgr::_evtStoreToVipEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		//if(nMsgType != CForm::mrYes)
		//{
		//	return;
		//}

		//if(! g_stUIStore.ResetLastOperate())
		//	return;

		//CS_StoreVIP(20, 1);
	}


	// ´ú±Ò¶Ò»»ÊÂ¼þ´¦Àí
	void CStoreMgr::_evtExchangeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes)
		{
			return;
		}

		g_stUIStore.m_nExchangeNum = 0;
		int num =  g_stUIStore.m_pkExchangeNum->GetNumber();
		if ( num > 0 )
		{
			g_stUIStore.m_nExchangeNum = num;

			char szBuffer[MAX_PATH] ={0};
			sprintf(szBuffer, g_oLangRec.GetString(905), g_stUIStore.m_nExchangeNum); // È·ÈÏ¶Ò»»´ú±Ò: %d
			CBoxMgr::ShowSelectBox(_evtExchangeCheckEvent, szBuffer, true);
		}
	}

	// ´ú±Ò¶Ò»»È·ÈÏ
	void CStoreMgr::_evtExchangeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes)
		{
			return;
		}

		if(! g_stUIStore.ResetLastOperate())
			return;

		CS_StoreChangeAsk(g_stUIStore.m_nExchangeNum);	// Í¨Öª·þÎñÆ÷
	}

	// È·ÈÏ¹ºÂò´¦Àí
	void CStoreMgr::_evtTradeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(nMsgType != CForm::mrYes)
		{
			return;
		}

		if(0 <= g_stUIStore.m_nCurSel && g_stUIStore.m_nCurSel < CStoreMgr::STORE_PAGE_SIZE &&
			g_stUIStore.GetCurSelItemID() > 0)
		{
			g_stUIStore.SetStoreBuyButtonEnable(false);
			CS_StoreBuyAsk(g_stUIStore.GetCurSelItemID());
		}
	}


	void CStoreMgr::_evtStoreFormClose(CForm* pForm, bool& IsClose)
	{
		// ´ò¿ªÈÈ¼ü¹¦ÄÜ
		CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_STORE, true);	// Î÷ÃÅÎÄµµÐÞ¸Ä

		// ÔÊÐíÁÙÊ±±³°ü ESC ¹¦ÄÜ
		//g_stUIStore.frmTempBag->SetIsEscClose(true);

		// Î´Ñ¡ÖÐ
		g_stUIStore.m_nCurSel = -1;

		// ¹Ø±ÕÊÔ´©½çÃæ
		g_stUIStore.ShowTryonForm(false);

		// ÁÙÊ±±³°üÔÚÉÌ³Ç¹Ø±Õºó¿ÉÒÔÍÏ¶¯
		//g_stUIStore.frmTempBag->SetIsDrag(true);

		// ÏÔÊ¾×óÏÂ½ÇÁÄÌìÐÅÏ¢
		g_stUIStore._SetIsShowCozeForm(true);

		// ¹Ø±ÕÁÙÊ±±³°ü
		/*
		if(! g_stUIEquip.GetItemForm()->GetIsShow())
		{
			g_stUIStore.ShowTempKitbag(false);
		}*/

		CS_StoreClose();
	}


////////////////////////////////////////////////////////////////////////////////////////////////////


	void CStoreMgr::_evtStoreLoadFormClose(CForm* pForm, bool& IsClose)
	{
		g_stUIStore.DarkScene(false);
	}

	void CStoreMgr::_evtProTimeArriveEvt(CGuiData *pSender)
	{
		g_stUIStore.DarkScene(false);
	}


////////////////////////////////////////////////////////////////////////////////////////////////////


	void CStoreMgr::_evtStoreViewAllMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		std::string strName = pSender->GetName();

		if(strName == "btnReBack")
		{
			g_stUIStore.ShowViewAllForm(false);
		}
	}

	
	void CStoreMgr::_evtStoreViewAllLostEvent(CGuiData *pSender)
	{
		g_stUIStore.frmViewAll->SetIsShow(false);
	}


////////////////////////////////////////////////////////////////////////////////////////////////////


	// »¹Ô­ÊÔ´©
	void CStoreMgr::ChaEquipClearAll()
	{
		m_sCurLookInfo = m_sLookInfo;
		_ChangeChaPart(m_sCurLookInfo);
		m_pCurrMainCha->FightSwitch(true);	// Ç¿ÖÆ½ÇÉ«Õ½¶·×´Ì¬
	}

	// ÊÔ´© 3D ½ÇÉ«Ïò×óÐý×ª
	void CStoreMgr::ChaLeftRotate()
	{
		m_nChaRotate += 180;
		m_nChaRotate += -(-1) * 15;
		m_nChaRotate = (g_stUIStore.m_nChaRotate + 360) % 360;
		m_nChaRotate -= 180;
	}

	// ÊÔ´© 3D ½ÇÉ«ÏòÓÒÐý×ª
	void CStoreMgr::ChaRightRotate()
	{
		m_nChaRotate += 180;
		m_nChaRotate += -(1) * 15;
		m_nChaRotate = (g_stUIStore.m_nChaRotate + 360) % 360;
		m_nChaRotate -= 180;
	}

	// »æÖÆÊÔ´© 3D ½ÇÉ«
	void CStoreMgr::RenderChaTryon(int x,int y)
	{
		if( !m_pCurrMainCha ) return;

		g_Render.LookAt( D3DXVECTOR3( 11.0f, 36.0f, 10.0f ), D3DXVECTOR3( 8.70f, 12.0f, 8.0f ), MPRender::VIEW_3DUI );
		y += 100;

		MPMatrix44 matCha = *m_pCurrMainCha->GetMatrix();	// ±£´æ½ÇÉ«¾ØÕó

		m_pCurrMainCha->SetScale( D3DXVECTOR3( 0.9f, 0.9f, 0.9f ) );
		m_pCurrMainCha->SetUIYaw(180 + m_nChaRotate);
		m_pCurrMainCha->SetUIScaleDis(9.0f * g_Render.GetScrWidth()/SMALL_RES_X );
		m_pCurrMainCha->RenderForUI(x, y, true);

		m_pCurrMainCha->SetMatrix(& matCha);	// »Ö¸´½ÇÉ«¾ØÕó

		g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());
	}


	// »æÖÆ3D½ÇÉ«£¨»Øµ÷£©
	void CStoreMgr::_evtChaTryonRenderEvent(C3DCompent *pSender, int x, int y)
	{
		g_stUIStore.RenderChaTryon(x, y);
	}

	void CStoreMgr::_evtChaEquipClearAll(CGuiData *sender, int x, int y, DWORD key)
	{
		g_stUIStore.ChaEquipClearAll();
	}

	void CStoreMgr::_evtChaLeftRotate(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIStore.ChaLeftRotate();
	}

	void CStoreMgr::_evtChaRightRotate(CGuiData *sender, int x, int y, DWORD key )
	{
		g_stUIStore.ChaRightRotate();
	}

	void CStoreMgr::_evtChaLeftContinueRotate(CGuiData *sender)
	{
		g_stUIStore.ChaLeftRotate();
	}

	void CStoreMgr::_evtChaRightContinueRotate(CGuiData *sender)
	{
		g_stUIStore.ChaRightRotate();
	}

	void CStoreMgr::_evtTryonFormClose(CForm* pForm, bool& IsClose)
	{
		if(g_stUIStore.m_pCurrMainCha)
		{
			//NetChangeChaPart(g_stUIStore.m_pCurrMainCha, g_stUIStore.m_sLookInfo);
			g_stUIStore._ChangeChaPart(g_stUIStore.m_sLookInfo);
			g_stUIStore.m_pCurrMainCha->FightSwitch(g_stUIStore.m_isFight);

			g_stUIStore.m_pCurrMainCha->SetScale( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
			g_stUIStore.m_pCurrMainCha = nullptr;
		}
	}
}


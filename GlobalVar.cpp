#include "Stdafx.h"

#include "GameApp.h"

#include "SceneObjSet.h"
#include "EffectSet.h"
#include "MusicSet.h"
#include "CharacterPoseSet.h"
#include "MapSet.h"
#include "ChaCreateSet.h"
#include "EventSoundSet.h"
#include "AreaRecord.h"
#include "ServerSet.h"

#include "MPEditor.h"
#include "FindPath.h"
#include "MPFont.h"

#include "EffectObj.h"
#include "notifyset.h"
#include "ChatIconSet.h"
#include "ItemTypeSet.h"
#include "InputBox.h"
#include "ItemPreSet.h"
#include "ItemRefineSet.h"
#include "ItemRefineEffectSet.h"
#include "StoneSet.h"
#include "ElfSkillSet.h"
#include "GameWG.h"
#include "GameMovie.h"
#include "MonsterSet.h"
#include "LootFilter.h"
#include "helpinfoset.h"
#include "NpcRotate.h"
#include "GetserialNumber.h"
#ifndef USE_DSOUND
#include "AudioThread.h"
CAudioThread	g_AudioThread;
#endif

// �����Կ����ȳ�ʼ��
//CLanguageRecord g_oLangRec.GetString("./scripts/table/StringSet.bin", "./scripts/table/StringSet.txt");
CLanguageRecord g_oLangRec("./scripts/table/StringSet.bin", "./scripts/table/StringSet.txt");

bool	volatile	g_bLoadRes				  = FALSE;
CGameApp*	        g_pGameApp	              = nullptr;
LootFilter* g_lootFilter = nullptr;
NpcRotate* g_NpcRotate = nullptr;
GetSerial* g_SerialNumber = nullptr;
CEffectSet*	        CEffectSet::_Instance     = nullptr;
CShadeSet*          CShadeSet::_Instance      = nullptr;
CMusicSet*          CMusicSet::_Instance      = nullptr;
CPoseSet*           CPoseSet::_Instance       = nullptr;
CMapSet*            CMapSet::_Instance        = nullptr;
CChaCreateSet*      CChaCreateSet::_Instance  = nullptr;
CEventSoundSet*     CEventSoundSet::_Instance = nullptr;
CAreaSet*           CAreaSet::_Instance       = nullptr;
CServerSet*         CServerSet::_Instance     = nullptr;
CNotifySet*         CNotifySet::_Instance     = nullptr;
CChatIconSet*		CChatIconSet::_Instance   = nullptr;
CItemTypeSet*		CItemTypeSet::_Instance   = nullptr;
CItemPreSet*		CItemPreSet::_Instance	  = nullptr;
CItemRefineSet*		CItemRefineSet::_Instance = nullptr;
CItemRefineEffectSet* CItemRefineEffectSet::_Instance	= nullptr;
CStoneSet*			CStoneSet::_Instance				= nullptr;
CElfSkillSet*		CElfSkillSet::_Instance				= nullptr;
CMonsterSet*        CMonsterSet::_Instance    = nullptr;//Add by sunny.sun 20080903
CHelpInfoSet*		CHelpInfoSet::_Instance     = nullptr;


MPEditor	        g_Editor;
CFindPath			g_cFindPath(128,38);
CFindPathEx         g_cFindPathEx;
CInputBox			g_InputBox;

CGameWG             g_oGameWG;
CGameMovie			g_GameMovie;


// �ͻ��˰汾��, ��GateServer����֤
short g_sClientVer = 136;
short g_sKeyData = short(g_sClientVer * g_sClientVer * 0x1232222);
char g_szSendKey[4];
char g_szRecvKey[4];

DWORD g_dwCurMusicID = 0;

// CLightEff plight;

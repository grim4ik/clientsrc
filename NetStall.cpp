#include "stdafx.h"
#include "netguild.h"
#include "UIGlobalVar.h"
#include "GuildMemberData.h"
#include "GuildMembersMgr.h"
#include "RecruitMemberData.h"
#include "RecruitMembersMgr.h"
#include "GuildListMgr.h"
#include "guildlistdata.h"
#include "GuildData.h"
#include "UIGuildApply.h"
#include "uiguildlist.h"
#include "UIGuildMgr.h"
#include "UIGraph.h"
#include "UIChat.h"
#include "UITreeView.h"
#include "ChatIconSet.h"
#include "CompCommand.h"
#include "GameApp.h"
#include "Character.h"
#include "StringLib.h"
#include "uiboatform.h"
#include "UIGuildChallengeForm.h"
#include "NetStall.h"



void NetMC_STALL_GETNAME(char stalltype)
{
	if (stalltype==0)
	{
		CUIGuildApply::ShowForm(CGuildData::navy);
	}
	else
	{
		CUIGuildApply::ShowForm(CGuildData::pirate);
	}
}
void NetMC_LISTGUILD_BEGIN(uChar gldtype)
{
	CGuildListMgr::ResetAll();
	CGuildListMgr::SetType((gldtype==0)?CGuildData::navy:CGuildData::pirate);
}

void NetMC_LISTGUILD_END()
{
	CUIGuildList::ResetOrder();
	CGuildListMgr::SortGuildsByName();
	CUIGuildList::ShowGuildList();
}

void NetMC_LISTGUILD(uLong id,cChar *name,cChar *motto,cChar *leadername,uShort memtotal,LLong exp)
{
	CGuildListMgr::AddGuild(new CGuildListData(id,name,motto,leadername,memtotal,exp));
}

void NetMC_GUILD_TRYFORCFM(cChar *oldgldname)
{
	CUIGuildList::OnMsgReplaceApply(oldgldname);
}
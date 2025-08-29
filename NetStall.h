#pragma once

#include "NetIF.h"
#include "PacketCmd.h"



// separate stuff like guild but for stall search
extern void NetMC_STALL_GETNAME(char guildtype);
extern void NetMC_LISTSTALL_BEGIN(uChar gldtype);
extern void NetMC_LISTSTALL_END();
extern void NetMC_LISTSTALL(uLong id,cChar *name,cChar *motto,cChar *leadername,uShort memtotal,LLong exp);
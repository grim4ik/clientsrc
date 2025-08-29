#include "stdafx.h"
#include "UIPortalTime.h"
#include "UIFormMgr.h"
#include "UIListView.h"

#include <ctime>
//#include "GlobalVar.h"
//#include "GameApp.h"
/*
#include "date/tz.h"
using namespace std::chrono;
using namespace date;
*/

// color management for portals
DWORD COLOR_ACTIVE = 0XFFfaaf3a;  // when  portal when its active but not open and not closed
DWORD COLOR_CLOSED = COLOR_RED;	  // if portal is closed >
DWORD COLOR_OPENED = COLOR_GREEN; // PORTAL IS OPEN

Portal::Portal(std::string destination, time_t entryFirst, time_t entryInterval, time_t entryClose, time_t destinationClose, std::string location)
	: destination(destination), entryFirst(entryFirst), entryInterval(entryInterval), entryClose(entryClose), destinationClose(destinationClose), location(location) {
}

bool GUI::CPortalTime::Init() {
	if (frmPortalTime = CFormMgr::s_Mgr.Find("frmPortalTime"); !frmPortalTime) {
		return false;
	}
	frmPortalTime->SetIsDrag(true);

	if (list = dynamic_cast<CListView*>(frmPortalTime->Find("list")); !list) {
		return false;
	}
	list->GetList()->GetItemImage()->SetColor(COLOR_BLUE);
	list->GetList()->GetItems()->SetItemMargin(20, 0);
	list->SetMargin(0, 0, 0, 0);
	return true;
}

void GUI::CPortalTime::Add(Portal entrance) {
	portals.emplace_back(entrance);
	auto row = list->AddItemRow();

	// Column 1 - Destination
	auto pItem = new CItem;
	pItem->SetString(entrance.Destination().data());
	row->SetIndex(0, pItem);
	// small use for multi skins
#if (defined isBlackSkin)
	row->SetColor(COLOR_WHITE);
#else
	row->SetColor(COLOR_RED);
#endif
	// map location Column 2
	pItem = new CItem;
	pItem->SetString(entrance.LocationInfo().data());
	row->SetIndex(1, pItem);
	//
	// Column 3 - Next Time for portal
	pItem = new CItem;
	pItem->SetString("Next");
	row->SetIndex(2, pItem);

	// Column 4 - Remaining  time for portal to close
	pItem = new CItem;
	pItem->SetString("Time");
	row->SetIndex(3, pItem);
	// status of portal Column 5
	pItem = new CItem;
	pItem->SetString("Open");
	row->SetIndex(4, pItem);

	list->Refresh();
}
// clear old list before adding new one
void GUI::CPortalTime::ClearList() {
	portals.clear();
	list->GetList()->Clear();
}

void GUI::CPortalTime::FrameMove(DWORD dwTime) {
	if (!frmPortalTime->GetIsShow()) {
		return;
	}

	// Update entrance times
	static tm _tm{};
	const auto timeNow = time(nullptr);

	// const auto departure = make_zoned("Asia/Amman", system_clock::now());
	// const auto timeNow = departure.get_sys_time().time_since_epoch().count();

	for (auto row = 0; row < portals.size(); ++row) {
		const auto& portal = portals[row];

		auto timeDiff = timeNow - portal.entryFirst;

		static const char* state_opening = "Closed";
		static const char* state_closing = "Open";
		static const char* state_unknwon = "Unknown";
		static const char* state_Active = "Active";
		auto state = [&]() -> const char* {
			if (portal.entryInterval) {
				auto time_open = timeDiff % portal.entryInterval;
				auto states = time_open > portal.entryClose ? state_opening : state_closing;
				auto timer = portal.destinationClose - (timeDiff % portal.entryInterval); // timer when map close
				if (states == state_opening) {

					if (timer > 0 && timer < portal.destinationClose)
						return state_Active;
				}
				return states;
			}
			return state_unknwon;
		}();

		auto time_countdown = [&]() -> time_t {
			if (state == state_opening) {
				return portal.entryInterval - (timeDiff % portal.entryInterval);
			}
			if (state == state_Active) {
				return portal.destinationClose - (timeDiff % portal.entryInterval);
			}
			if (state == state_closing) {
				return portal.entryClose - (timeDiff % portal.entryInterval);
			}
			return 0;
		}();

		// Update state
		auto pItem = list->GetItemObj(row, 4);
		pItem->SetString(state);
		if (state == state_closing) {
			pItem->SetColor(COLOR_OPENED);
		}
		else if (state == state_Active) {
			pItem->SetColor(COLOR_ACTIVE);
		}
		else
			pItem->SetColor(COLOR_CLOSED);
		// small use for multi skins
		// set map name color :
		pItem = list->GetItemObj(row, 0);
#if (defined isBlackSkin)

		pItem->SetColor(COLOR_WHITE);
#else
		pItem->SetColor(COLOR_BLUE);
#endif
		//
		// Update time countdown
		//std::tm* p = gmtime(&time_countdown);
		std::tm p;
		gmtime_s(&p, &time_countdown);
		static char buf[80];
		std::strftime(buf, sizeof(buf), "%T", &p);
		// set data to next open
		if (state == state_opening) // when map closed  to show next wave of it
		{
			pItem = list->GetItemObj(row, 2);
			pItem->SetString(buf);
			pItem->SetColor(COLOR_OPENED);

			pItem = list->GetItemObj(row, 3);
			pItem->SetString("--");
			pItem->SetColor(COLOR_CLOSED);
		}
		else if (state == state_Active) // when map active but portal closed
		{
			pItem = list->GetItemObj(row, 3);
			pItem->SetString(buf);
			pItem->SetColor(COLOR_ACTIVE);

			pItem = list->GetItemObj(row, 2);
			pItem->SetString("--");
			pItem->SetColor(COLOR_ACTIVE);
		}
		else //	when map portal open and people can join it >show the remining time until close
		{
			pItem = list->GetItemObj(row, 3);
			pItem->SetString(buf);
			pItem->SetColor(COLOR_OPENED);

			pItem = list->GetItemObj(row,2);
			pItem->SetString("--");
			pItem->SetColor(COLOR_CLOSED);
		}
		//update location color
		pItem = list->GetItemObj(row, 1);
		pItem->SetColor(COLOR_BLUE);
	}
}

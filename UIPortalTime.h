#pragma once
#include "UIGlobalVar.h"
class Portal {
public:
	Portal(std::string destination, time_t entryFirst, time_t entryInterval, time_t entryClose, time_t destinationClose, std::string location);

	std::string_view Destination() const {
		return destination;
	}
	std::string_view LocationInfo() const {
		return location;
	}
	//functions for upo custom maps //
	void SetentryInterval(time_t time) { entryInterval = time; }
	void Setdestination(std::string MapName) { destination = MapName; }
	//private:
	std::string location; //map location
	std::string destination;	//map	Name 
	time_t entryFirst{ 0 };	// Time when the entry was first executed
	time_t entryInterval{ 0 };	// After the entry is executed for the first time, the time interval between subsequent executions
	time_t entryClose{ 0 };		// The time interval to disappear after each entry is executed

	//NOTE: This may be useless data...
	time_t destinationClose{ 0 };	// The time interval from when the entry is executed to the map close
};


// Interface for UI only
namespace GUI {
	class CPortalTime : CUIInterface {
	public:
		virtual bool Init() override;

		void Add(Portal portal);
		void ClearList();
		auto GePortalForm() { return frmPortalTime; }

	private:
		std::vector<Portal> portals;
		CForm* frmPortalTime{ nullptr };
		CListView* list{ nullptr };

	protected:
		virtual void FrameMove(DWORD dwTime) override;
	};
} // namespace GUI
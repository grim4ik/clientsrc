#include "stdafx.h"

class LootFilter {
  public:
    LootFilter();
    ~LootFilter();

    map<int, bool> GetFilteredItems() { return lootFilter; }
    void AddFilteredItem(unsigned long itemId);
    void RemoveFilteredItem(unsigned long itemId);
    bool HasFilteredItem(unsigned long itemId);
    void PrintAllItems();
  private:
    map<int, bool> lootFilter;
};


extern LootFilter* g_lootFilter;
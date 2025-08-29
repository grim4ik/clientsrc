#include "stdafx.h"
class NpcRotate
{
public:
    NpcRotate();
    ~NpcRotate() {};
    bool IsRotateActive();
    bool CanRotateIDs(unsigned long ID);
    bool IsMatchDistance(DWORD Distance);
    DWORD getnpcdist() {return DistanceRotate; }
private:
    map<int, bool> NpcRotateID;
    DWORD DistanceRotate = 400;
    bool RotateEnabled = true;
};




extern NpcRotate* g_NpcRotate;
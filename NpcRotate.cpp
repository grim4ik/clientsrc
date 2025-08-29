#include "stdafx.h"
#include <map>
#include "NpcRotate.h"
#include "GameApp.h"
#include <string>
#include "log.h"


NpcRotate::NpcRotate() {
    
    if (!CreateDirectory("user", nullptr)) {
        DWORD dw = GetLastError();
        if (dw != 183) {
            return;
        }
    }
    /*  std::fstream NpcRotateIDFile;
    NpcRotateIDFile.open("./user/RotateNpc.txt", std::fstream::in | std::fstream::out | std::fstream::app);	//to open file or create it if not exist 
    if (NpcRotateIDFile.is_open() == 0)
    {
        NpcRotateIDFile << "isenable = 1"<< "\n";
        NpcRotateIDFile << "Distance= 600" << "\n";
        NpcRotateIDFile << "[Ignored NPCS]" << "\n";
        NpcRotateIDFile.close();
    }
    */
    ifstream NpcRotateIDFile;
    NpcRotateIDFile.open("user\\RotateNpc.txt");

    string MonsterIDS;
    if (NpcRotateIDFile.is_open()) {
        while (getline(NpcRotateIDFile, MonsterIDS)) {

            if (MonsterIDS.length() == 0) {
                continue;
            }
            std::size_t disabled = MonsterIDS.find("isenable=0");
            std::size_t enabled = MonsterIDS.find("isenable=1");
           if (disabled != std::string::npos)
           {
               RotateEnabled = false;
               break;
           }
           if (enabled != std::string::npos)
           {
               RotateEnabled = true;
               continue;
           }
           
           else if (MonsterIDS.find("Distance=") != string::npos)
           {
               DistanceRotate = stoi(MonsterIDS.substr(10, 14));
               continue;
           }
           else if (MonsterIDS.find("[Ignored NPCS]") != string::npos)
           {
               continue;
           }
            int MObID = atoi(MonsterIDS.c_str());
            NpcRotateID.insert(pair<int, bool>(MObID, TRUE));
        }
    }
    NpcRotateIDFile.close();
    
}


bool NpcRotate::IsRotateActive()
{
    return RotateEnabled;
}

bool NpcRotate::CanRotateIDs(unsigned long ID)
{
    if (NpcRotateID.find(ID) == NpcRotateID.end()) {
        return true;
    }
    return false;
}

bool NpcRotate::IsMatchDistance(DWORD Distance)
{
    if (Distance <= DistanceRotate)
    {
        return true;
    }
    return false;
}

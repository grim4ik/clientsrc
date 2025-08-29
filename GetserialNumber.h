#pragma once
#include "stdafx.h"
#include <string>

using namespace std;
class GetSerial
{
public:
    GetSerial();
    ~GetSerial();
    void exec(const char* cmd);
    string removeSpaces(string& str);
    string GetSerialNumber() { return PcSerial; }
private:
    string PcSerial = "";
};


extern GetSerial* g_SerialNumber;
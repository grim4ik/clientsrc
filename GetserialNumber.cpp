
#include "stdafx.h"
//#include <cstdio>
#include <array>
//#include <cctype>
#include "GetserialNumber.h"

GetSerial::GetSerial() {

	//exec("wmic path win32_physicalmedia get SerialNumber"); //driver version 
	//exec("wmic baseboard get serialnumber"); // bios/board version
	exec("wmic baseboard get serialnumber,version,product"); // bios/board version with win version
	// wmic bios get serialnumber
	//wmic baseboard get serialnumber
}

GetSerial::~GetSerial() {
}

std::string GetSerial::removeSpaces(std::string& str) {
	str.erase(std::remove_if(str.begin(), str.end(), [](auto const& c) -> bool { return !std::isalnum(c); }), str.end()); // const unsigned
	//new serial version for board 
	if (str.length() > 20) { //if serial more than 20 we remove the extra length 
		str.erase(20);
	}
	std::reverse(str.begin(), str.end());
	// catch bugged return serial with 0000000001000000
	/*
	as default behave return this number "0000_0000_0100_0000_E4D2_5C34_6DBE_5001"
	*/
	/*
	std::string buggedserial = "0000000001000000";

	std::string tempstr = str.substr(12, 16);
	if (tempstr == buggedserial) {
		// str = str.substr(28, str.length());
		str = str.substr(28, 45); // for now just use magic numbers to avoid people with many drivers to be long length
	} else {
		str = tempstr;
	}

	//debug info
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	cout << "bugged serial is :" << tempstr.c_str() << endl;
	cout << "working serial is :" << str.c_str() << endl;
	*/
	return str;
}
void GetSerial::exec(const char* cmd) {
	std::array<char, 128> buffer{};
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe)
		throw std::runtime_error("_popen() failed!");
	auto i = 0; // count current readed lines
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
			//to read second line only 
			if (i == 1) {
				result += buffer.data(); //+=
				break;
			}
			i++;

		}

	}

	// return removeSpaces(result);
	PcSerial = removeSpaces(result);
}
//
#include "stdafx.h"
#include "soundcommon.h"
#include <direct.h>

std::string StringToLower(const std::string& theString)
{
	std::string aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += tolower(theString[i]);

	return aString;
}

std::string GetDirectSoundString( HRESULT theResult )
{
	switch(theResult)
	{
	case DSERR_ALLOCATED: return "DSERR_ALLOCATED";
	case DSERR_ALREADYINITIALIZED: return "DSERR_ALREADYINITIALIZED";
	case DSERR_BADFORMAT: return "DSERR_BADFORMAT";
	case DSERR_BUFFERLOST: return "DSERR_BUFFERLOST";
	case DSERR_CONTROLUNAVAIL: return "DSERR_CONTROLUNAVAIL";
	case DSERR_GENERIC: return "DSERR_GENERIC";
	case DSERR_INVALIDCALL: return "DSERR_INVALIDCALL";
	case DSERR_NOAGGREGATION: return "DSERR_NOAGGREGATION";
	case DSERR_NODRIVER: return "DSERR_NODRIVER";
	case DSERR_NOINTERFACE: return "DSERR_NOINTERFACE";
	case DSERR_OTHERAPPHASPRIO: return "DSERR_OTHERAPPHASPRIO";
	case DSERR_PRIOLEVELNEEDED: return "DSERR_PRIOLEVELNEEDED";
	case DSERR_UNINITIALIZED: return "DSERR_UNINITIALIZED";

	case E_OUTOFMEMORY: return "E_OUTOFMEMORY";
	}

	char aBuf[50];
	sprintf(aBuf,"%x",theResult);
	return aBuf;
}

void MkDir(const std::string& theDir)
{
	std::string aPath = theDir;

	int aCurPos = 0;
	for (;;)
	{
		int aSlashPos = aPath.find('\\', aCurPos);
		if (aSlashPos == -1)
		{
			mkdir(aPath.c_str());
			break;
		}

		aCurPos = aSlashPos+1;

		std::string aCurPath = aPath.substr(0, aSlashPos);
		mkdir(aCurPath.c_str());
	}
}

std::string GetFileDir(const std::string& thePath, bool withSlash)
{
	int aLastSlash = max((int) thePath.rfind('\\'), (int) thePath.rfind('/'));

	if (aLastSlash == -1)
		return "";
	else
	{
		if (withSlash)
			return thePath.substr(0, aLastSlash+1);
		else
			return thePath.substr(0, aLastSlash);
	}
}

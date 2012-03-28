
#ifndef   _MELD_
#define   _MELD_

#include "meld.h"

//void LT_Log(const PPacketBase* beforePacket,
//			const PPacketBase* nowPacket,
//			const Network::PInetAddr& from,
//			const Network::PInetAddr& to,
//			bool isSendType,
//			const string& content,
//			unsigned int applicaitonid);

enum{
	Pktown_in_meld = 1,
};

#ifdef SUPPORT_MELD
#define MELD_LOG(beforePacket,nowPacket,from,to,isSendType,content) LT_Log(beforePacket,nowPacket,from,to,isSendType,content,Pktown_in_meld)
#else
#define MELD_LOG(beforePacket,nowPacket,from,to,isSendType,content) 
#endif

#ifdef _MSC_VER
#ifdef MELD_STATIC

#if defined DEBUG || defined _DEBUG
#pragma comment(lib, "meldSD.lib")
#else
#pragma comment(lib, "meldS.lib")
#endif

#else//LOG4CPLUS_STATIC

#if defined DEBUG || defined _DEBUG
#pragma comment(lib, "meldD.lib")
#else
#pragma comment(lib, "meld.lib")
#endif

#endif//LOG4CPLUS_STATIC
#endif//_MSC_VER

#endif //_MELD_


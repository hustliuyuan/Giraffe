// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef __PACKET_PARSER__
#define __PACKET_PARSER__


#include "packet/PPacketBase.h"
#include "network/PSocketTypeFactory.h"


class NETWORK_EXPORT IPacketParser
{

protected:

	IPacketParser();

public:

		virtual	~IPacketParser();

	virtual const PPacketBase *	 parsePkt(const S8 * inContent, 
		U16 inContentLen, U32 socket_type = Network::PSocketTypeFactory::DEFAULT_SOCKET_TYPE) const = 0;

};

#endif // __PACKET_PARSER__

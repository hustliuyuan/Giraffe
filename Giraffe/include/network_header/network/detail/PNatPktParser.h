// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef __PACKET_PARSER__
#define __PACKET_PARSER__

#include "common/Header.h"
#include "packet/PPacketBase.h"
#include "network/PSocketTypeFactory.h"


class PNatPktParser
{



public:

		PNatPktParser();

		virtual	~PNatPktParser();

	virtual const PPacketBase *	 parsePkt(const S8 * inContent, 
		U16 inContentLen, U32 socket_type = Network::PSocketTypeFactory::DEFAULT_SOCKET_TYPE ) const ;

};

#endif // __PACKET_PARSER__

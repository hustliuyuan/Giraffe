// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//tuxuping 2008-5-20
#ifndef __PACKET_HANDLER_H__
#define __PACKET_HANDLER_H__


#include "network/PacketINFO.h"
#include "packet/PPacketBase.h"
#include "packet/PPacketDefinition.h"


class NETWORK_EXPORT IPacketHandler {

protected:

	IPacketHandler(){}

public:

	typedef Network::PacketINFO PacketINFO;


	virtual ~IPacketHandler(){}

	virtual void handlePkt( const PacketINFO * pktinfo ) = 0;


};


#endif /*__PACKET_HANDLER_H__*/



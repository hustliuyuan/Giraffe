// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//tuxuping 2008-5-20
#ifndef __PACKET_HANDLER_TASK_H__
#define __PACKET_HANDLER_TASK_H__


#include "common/IPacketHandler.h"
#include "common/ITask.h"




class NETWORK_EXPORT IPacketHandlerTask : public ITask , public IPacketHandler
{

public:

	typedef Network::PacketINFO PacketINFO;

	IPacketHandlerTask(){}

	virtual ~IPacketHandlerTask(){}

	virtual void handlePkt( const PacketINFO * pktinfo ) = 0;

	virtual void onRun() =0;

	virtual std::string getName() const=0;



};


#endif /*__PACKET_HANDLER_H__*/



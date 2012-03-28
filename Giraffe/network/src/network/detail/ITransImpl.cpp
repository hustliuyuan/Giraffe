// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"

#include "network/detail/ITransImpl.h"

namespace Network { namespace Detail { 


	//InPacketQueue *		ITransImpl::getPacketQueue(){


	//	return & this->m_oInPacketQueue;

	//}

	ITransImpl::~ITransImpl(void){

		// release the packets in the queue?

	}

	//void	ITransImpl::registerPacketHandler( IPacketHandler * packet_handler ) 
	//{


	//	this->m_pPacketHandler = packet_handler;

	//}

	//void	ITransImpl::handlePkt( const PacketINFO *  info ) 
	//{


	//	this->m_pPacketHandler->handlePkt( info ) ;
	//}

}
}
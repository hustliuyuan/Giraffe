// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef  ITRANSIMPL_H 
#define ITRANSIMPL_H 

#include "common/ITask.h"
#include "packet/PPacketBase.h"
#include "common/Header.h"
#include "common/IPacketHandler.h"
#include <string>
#include "network/PacketINFO.h"
#include "network/PInetAddr.h"
//class PTransImpl;
//#include "detail/PTransImpl.h"

// different thread can / or can not send the packet without explicitly synchronization!
namespace Network { namespace Detail { 

	class ITransImpl : public ITask
	{
	public:

		virtual							~ITransImpl(void);

		// for server design
		// user can send by tcp or reliable udp ( e.g. udt)
		// socket, send <inPacket> through socket <inSocket>

		virtual void					reliableSend( U32 inSocket,  const PPacketBase   * inPacket ) = 0;	
		// for server design
		// user can send by udp

		// for client design
		// user can send <inPacket> to <inAddr>. by tcp or reliable udp ( e.g. udt )    
		virtual void					reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket ) =0;

		// for client design
		// user can send by udp. send <inPacket> to inAddr .
		virtual void					unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket ) =0;

		//  return the packetINFO queue.
		// all the packets wrappered by packetINFO received by this transmitter are stored in this queue.
		// the programmer may dispatch the packet after fetch it from the queue.
		//InPacketQueue *					getPacketQueue() ; // cant be const fucntion


		virtual	void					registerPacketHandler( IPacketHandler * packet_handler ) =0 ;


	//protected:

		// the decendent class  will invoke this function to handle all the packets.
		virtual	void					handlePkt( const PacketINFO *  info ) = 0;

		// all packets received by this transmitter are stored in this queue.
		//InPacketQueue					m_oInPacketQueue;

		// all packet can be processed by this handler;
		// here is the simple design, we even can specify which handler handle which 
		// kind of packets.
		//IPacketHandler *				m_pPacketHandler;


	};

}
}

#endif //ITRANSIMPL_H
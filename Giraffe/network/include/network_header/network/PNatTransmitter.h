// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef PNAT_TRANSMITTER_H
#define PNAT_TRANSMITTER_H

#include "common/ITask.h"
#include "packet/PPacketBase.h"
#include "common/Header.h"
#include "common/IPacketHandler.h"
#include <string>
#include "PacketINFO.h"
#include "PInetAddr.h"

#include "network/detail/PNatTransImpl.h"

// different thread can / or can not send the packet without explicitly synchronization!
namespace Network {

	class NETWORK_EXPORT PNatTransmitter : public ITask	 
	{
	public:

		PNatTransmitter() ;

		virtual ~PNatTransmitter() ;

		// user can send <inPacket> to <inAddr>. by tcp or reliable udp ( e.g. udt )    
		// Notice: the packet size must be small than 
		// TCP_PACKET_SIZE,MAX_RCV_BUF_SIZE and MAX_PACKET defined in PReceiverBuffer.
		//  also small than MAX_SEND_BUF_SIZE and MAX_PACKET defined in PSendBuffer
		// other two reliableSend function has same behavior. 2008-7-25. tuxuping.
		virtual void					reliableSend( const PAddressable & inAddr_able ,  const PPacketBase   * inPacket );

		// for client design
		// user can send by udp. send <inPacket> to inAddr .
		virtual void					unreliableSend( const PAddressable & inAddr_able , const PPacketBase * inPacket );

		//virtual void					reliableSend( U32 inSocket,  const PPacketBase   * inPacket ) ;	
		
		virtual void					reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );

		virtual void					unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket );

		virtual	void					registerPacketHandler( IPacketHandler * packet_handler );

		

		virtual void					onRun();	

		virtual String					getName() const ;

		void							setMyAddressable(const PAddressable & addr_able);		

		bool							addTcpListener( const String& ip,  U16 port, U32 socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE);

	protected:

		virtual Detail::PNatTransImpl*	getNatTrans();	

		Detail::PNatTransImpl*			m_pTrans;
		

	};



}

#endif // PNAT_TRANSMITTER_H
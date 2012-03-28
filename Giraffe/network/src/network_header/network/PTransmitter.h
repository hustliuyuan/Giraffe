// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef PTRANSMITTER_H
#define  PTRANSMITTER_H

#include "common/ITask.h"
#include "packet/PPacketBase.h"
#include "common/IPacketHandler.h"
#include <string>
#include "PacketINFO.h"
#include "PInetAddr.h"
#include "detail/PTransImpl.h"


// different thread can / or can not send the packet without explicitly synchronization!
namespace Network {

	class NETWORK_EXPORT PTransmitter : public ITask	
	{
	public:

		PTransmitter();

		virtual							~PTransmitter(void);

		// for server design
		// user can send by tcp or reliable udp ( e.g. udt)
		// socket, send <inPacket> through socket <inSocket>

		//virtual void					reliableSend( U32 inSocket,  const PPacketBase   * inPacket );	
		
		// for client design
		// user can send <inPacket> to <inAddr>. by tcp or reliable udp ( e.g. udt )    
		virtual void					reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );

		// for client design
		// user can send by udp. send <inPacket> to inAddr .
		virtual void					unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket );


		virtual	void					registerPacketHandler( IPacketHandler * packet_handler );
		virtual	void					registerPacketHandler( IPacketHandler * packet_handler, U16, U16 );

		virtual void					onRun();		

		virtual String					getName() const ;

		bool							addTcpListener( const String& ip,  U16 port, U32 socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE);


	protected:

      virtual Network::Detail::ITransImpl*			getTrans();
	  virtual Network::Detail::ITransImpl*			getTrans(U16, U16);

      Network::Detail::PTransImpl 	*				m_pTrans ;
   

	};

}

#endif  //PTRANSMITTER_H

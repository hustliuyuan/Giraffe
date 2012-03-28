// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef  PNAT_TRANS_IMPL_H 
#define PNAT_TRANS_IMPL_H  

//#include "network/INatTransmitter.h"
#include "SocketINFO.h"
#include "ITransImpl.h"
#include "network/PInetAddr.h"
#include "PNatPkthandler.h"
#include "INatTransImpl.h"




namespace Network { namespace Detail {

	class PNatTransImpl 
		: public INatTransImpl
	{
	public:

		PNatTransImpl( );

		PNatTransImpl(U16 tcpport, U16 udpport);

	public:

		virtual ~PNatTransImpl(void);

		virtual void					unreliableSend( const PAddressable &  remote_addr , const PPacketBase * inPacket );	

		virtual void					reliableSend( const PAddressable &  remote_addr, const PPacketBase * inPacket );

		virtual void					reliableSend( U32 inSocket,  const PPacketBase   * inPacket );	

		virtual void					reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );

		virtual void					unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket );

		virtual	void					registerPacketHandler( IPacketHandler * packet_handler );

		// get my address
		virtual const PAddressable &	getMyAddressable()const ;

		void							setMyAddressable(const PAddressable & addr_able);

		virtual void					onRun();

		//heartbeat interval is set in the local "client/client_conf.ini"
		void							sendHeartbeat();

		//every seconds check one time. added by tuxuping 2008-10-7
		void							handleTransTimeout( );

		virtual String					getName() const;

		// overload this function.
		virtual void					handlePkt( const PacketINFO *  info );	

		bool							addTcpListener( const String& ip,  U16 port, U32 socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE);

		

	protected:	

		// different user can be mapped to different nat server.
		const PInetAddr &				getNatServerAddr( const PInetAddr &  addr ) const ;	


	protected:

		ITransImpl*					m_pTransImpl;

		PAddressable				m_oMyAddressable;

		PNatPktHandler*				m_pNatPktHandler;

		PInetAddr					m_oNatServerAddr;

		// nat server heartbeat
		// each m_iPingInterval time, we start a ping to each peer.
		S32							m_iHeartbeatInterval;

		S32							m_iLastHeartbeatTime;

		// check nat trans timeout.
		S32							m_iTransTimeoutInterval;

		S32							m_iLastTransTimeoutTime;

		// if it is the first run, send nat and tracker heartbeat right now.
		// added by tuxuping 2008-10-8
		B8							m_bIsFirstRun;



	};

} // namespace detail
} // namespace network

#endif // PNAT_TRANS_IMPL_H
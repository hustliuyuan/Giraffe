// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "network/PNatTransmitter.h"
#include "config/PSettings.h"


namespace Network {

	Detail::PNatTransImpl*			PNatTransmitter::getNatTrans() 
	{
		// just for test.
		if( m_pTrans ==NULL ){
			const PSettings & settings= PSettings::getInstance();
			U16 tcp_port = settings.getConfig("local-tcp-server-port").m_value.intvalue;
			U16 udp_port = settings.getConfig("local-udp-server-port").m_value.intvalue;
		 m_pTrans = new Detail::PNatTransImpl(tcp_port,udp_port);
		}
		return m_pTrans;
	}	

	PNatTransmitter::PNatTransmitter() 
	{
		 m_pTrans  = NULL;
				 
	}	

	PNatTransmitter::~PNatTransmitter() 
	{
		delete m_pTrans ;
		m_pTrans =NULL ;		 
	}	

	void	PNatTransmitter::reliableSend( const PAddressable & inAddr_able ,  const PPacketBase   * inPacket )
	{
		Detail::PNatTransImpl * trans = getNatTrans();
		trans->reliableSend(inAddr_able,inPacket);
	}

	void	PNatTransmitter::unreliableSend( const PAddressable & inAddr_able , const PPacketBase * inPacket )
	{
		Detail::PNatTransImpl * trans = getNatTrans();
		trans->unreliableSend(inAddr_able,inPacket);
	}

	//void	PNatTransmitter::reliableSend( U32 inSocket,  const PPacketBase   * inPacket ) 
	//{
	//	Detail::PNatTransImpl * trans = getNatTrans();
	//	trans->reliableSend(inSocket,inPacket);
	//}

	void	PNatTransmitter::reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket )
	{
		Detail::PNatTransImpl * trans = getNatTrans();
		trans->reliableSend(inAddr,inPacket);
	}

	void	PNatTransmitter::unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket )
	{
		Detail::PNatTransImpl * trans = getNatTrans();
		trans->unreliableSend(inAddr,inPacket);
	}

	void	PNatTransmitter::registerPacketHandler( IPacketHandler * packet_handler )
	{
		Detail::PNatTransImpl * trans = getNatTrans();
		trans->registerPacketHandler( packet_handler );
	}

	void	PNatTransmitter::onRun()
	{
		Detail::PNatTransImpl * trans = getNatTrans();
		trans->onRun();
	}

	String	PNatTransmitter::getName() const 
	{
		return "PNatTransmitter";
	}

	void	PNatTransmitter::setMyAddressable(const PAddressable & addr_able)
	{
			Detail::PNatTransImpl * trans = getNatTrans();
			trans->setMyAddressable(addr_able);
	}

	bool	PNatTransmitter::addTcpListener( const String& ip,  U16 port, U32 socket_type )
	{
		Detail::PNatTransImpl * trans = (Detail::PNatTransImpl *)getNatTrans();
		return  trans->addTcpListener(ip,port,socket_type);

	}

}
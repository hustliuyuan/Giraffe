// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/PTransmitter.h"
#include "network/detail/PTransImplTcp.h"
//#include "win32.h"


namespace Network {


	PTransmitter::PTransmitter()	
	{
		m_pTrans =NULL;
	}

	PTransmitter::~PTransmitter(void){

		// release the packets in the queue?
		delete m_pTrans;
		m_pTrans =NULL ;	

	}

	void	PTransmitter::registerPacketHandler( IPacketHandler * packet_handler ) 
	{
		Network::Detail::ITransImpl * trans = getTrans();
		trans->registerPacketHandler(packet_handler);		
	}

	void	PTransmitter::registerPacketHandler( IPacketHandler * packet_handler, U16 tcp_port, U16 udp_port) 
	{
		Network::Detail::ITransImpl * trans = getTrans(tcp_port,udp_port);
		trans->registerPacketHandler(packet_handler);		
	}

	Network::Detail::ITransImpl*	PTransmitter::getTrans()
	{	
		if( m_pTrans ==NULL ){
			const PSettings & settings= PSettings::getInstance();
			U16 tcp_port = settings.getConfig("local-tcp-server-port").m_value.intvalue;
			U16 udp_port = settings.getConfig("local-udp-server-port").m_value.intvalue;
			m_pTrans = new Detail::PTransImplTcp();
			Network::Detail::PTransImplTcp * tcpTransImp = (Network::Detail::PTransImplTcp *)	m_pTrans ;
			tcpTransImp->initialize(tcp_port, udp_port);
		}
		return m_pTrans;
	}

	Network::Detail::ITransImpl*	PTransmitter::getTrans(U16 tcp_port, U16 udp_port)
	{	
		if( m_pTrans ==NULL ){
		//	const PSettings & settings= PSettings::getInstance();
			//U16 tcp_port = settings.getConfig("local-tcp-server-port").m_value.intvalue;
			//U16 udp_port = settings.getConfig("local-udp-server-port").m_value.intvalue;
			m_pTrans = new Detail::PTransImplTcp();
			Network::Detail::PTransImplTcp * tcpTransImp = (Network::Detail::PTransImplTcp *)	m_pTrans ;
			tcpTransImp->initialize(tcp_port, udp_port);
		}
		return m_pTrans;
	}

	//void	PTransmitter::reliableSend( U32 inSocket,  const PPacketBase   * inPacket )
	//{

	//	Detail::PTransImpl * trans = getTrans();
	//	trans->reliableSend(inSocket, inPacket);

	//}

	void	PTransmitter::reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket )
	{
		Detail::ITransImpl * trans = getTrans();
		trans->reliableSend(inAddr, inPacket);

	}


	void	PTransmitter::unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket )
	{
		Network::Detail::ITransImpl * trans = getTrans();
		trans->unreliableSend(inAddr, inPacket);

	}

	void	PTransmitter::onRun()
	{
		Network::Detail::ITransImpl * trans = getTrans();
		trans->onRun();
	}

	String	PTransmitter::getName() const 
	{
		return "PTransmitter";
	}

	bool	PTransmitter::addTcpListener( const String& ip,  U16 port, U32 socket_type )
	{
		Network::Detail::PTransImplTcp * trans = (Network::Detail::PTransImplTcp *)getTrans();
		U32 ip_int = PUtils::addr2Long(ip.c_str());
		return  trans->addTcpListener(ip_int,port,socket_type);
		
	}

}

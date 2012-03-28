// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.



#include "common/Header.h"
#include "network/detail/PTransImpl.h"
#include "packet/PPacketDefinition.h"


namespace Network { namespace Detail {

PTransImpl::PTransImpl(void)
{
	
	PUtils::setZero(( S8 * )m_UdpSockMap, sizeof(m_UdpSockMap));

	//m_pUDPListener=NULL;
}

PTransImpl::~PTransImpl(void)
{
	//LOG_DEBUG("PTransImpl","~PTransImpl");
	SocketMap::const_iterator it = m_LiveSockets.begin();

	while( it != m_LiveSockets.end() ){

		LOG_DEBUG("PTransImpl","Socket:key:"<<it->first.toString() << "value:"<<it->second->pSocket->toString());
		delete it->second->pSocket;
		delete it->second;
		++it;
	}
	this->m_LiveSockets.clear();
	// no need to delete udpsocketmap. because all socket are stored in m_LiveSockets.
	//this->m_pUDPListener = NULL;

	//delete this->m_pUDPListener;
	// TODO: delete sockets.
	//for ( const m_LiveSockets
}

void    PTransImpl::unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket )
{
	

	// removed by tuxuping , because we don't need udp to send flash packet.
	//const PacketINFO * info = new PacketINFO(inAddr, m_UdpSockMap[ inPacket->getSocketType()->getId()]->getSocketFD(), inPacket);
	//assert(info);
	//m_UdpSockMap[ inPacket->getSocketType()->getId()]->send(info);

	const PacketINFO * info = new PacketINFO(inAddr, PSocketTypeFactory::DEFAULT_SOCKET_TYPE, inPacket);
	assert(info);
	m_UdpSockMap[ PSocketTypeFactory::DEFAULT_SOCKET_TYPE ]->send(info);

	return ;
}


B8		PTransImpl::addUDPListener(U16	inPort, U32 socket_type)
{

	// only one instance.

	//U32 id = inSockType->getId();

	U32 id = socket_type;

	assert( m_UdpSockMap[ id ] == NULL ) ;	

	m_UdpSockMap[id] = new PUDPSocket( PSocket::NONE_BLOCK_SOCK, socket_type );

	if( m_UdpSockMap[id] == NULL )
	{
		LOG_ERROR("PTransImpl","Initialize(): new a udp socket server error");
		return false;
	}
	if( m_UdpSockMap[id]->open() ==false)
	{
		LOG_ERROR("PTransImpl","Initialize():new a udp server error");
		return false;
	}
	// bind udp

	const PSettings & Settings = PSettings::getInstance();

	std::string local_ip =  Settings.getConfig("local-ip").m_value.strvalue;

	B8 err = m_UdpSockMap[id]->bind( PUtils::addr2Long(local_ip.c_str()),inPort);

	if( err == false )
	{
		LOG_ERROR("PTransImpl","Initialize():bind udp server error:");
		return false;
	}

	LOG_DEBUG("PTransImpl","udp server started: "<< local_ip <<":"<<inPort);

	PInetAddr udplistener;//,udplistener, localserver;

	udplistener.set(PUtils::addr2Long(local_ip.c_str()),inPort);

	m_LiveSockets[ udplistener ] =  new SocketINFO(m_UdpSockMap[id], true,true);

	return true;

}

	void	PTransImpl::registerPacketHandler( IPacketHandler * packet_handler ) 
	{

		this->m_pPacketHandler = packet_handler;

	}

	void	PTransImpl::handlePkt( const PacketINFO *  info ) 
	{

		this->m_pPacketHandler->handlePkt( info ) ;
	}



} // namespace detail
} // namespace network

// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/detail/PTransImplTcp.h"
#include "packet/PPacketDefinition.h"
#include "stdlib.h"

namespace Network { namespace Detail {

//PTransImplTcp		gTransImplTcp;

PTransImplTcp::PTransImplTcp(void)
{
	//m_pTCPListener=NULL;
	PUtils::setZero(( S8 * )  m_TcpServerMap, sizeof(m_TcpServerMap));
	LOG_DEBUG("PTransImplTcp","PTransImplTcp()"<<" ctor."<<this);
}

PTransImplTcp::~PTransImplTcp(void)
{

	// Notice: m_pTCPListener is deleted in PTransImpl::~PTransImpl();
	// when it deleted all sockets in m_LiveSockets.

}


void	PTransImplTcp::reliableSend( U32 inSocket,  const PPacketBase   * inPacket ){

	LOG_ERROR("PTransImplTcp"," no implemented reliableSend");
	assert(0);

}

std::string PTransImplTcp::getName() const{

	return "PTransImplTcp";

}


B8		PTransImplTcp::tcpSend( const PInetAddr & addr , const PPacketBase   * inPacket ){

	PTCPSocket * s=NULL;

	if( m_LiveSockets.find( addr) != m_LiveSockets.end()){

		s = (PTCPSocket*)(m_LiveSockets[addr]->pSocket);

		assert(s->isTCP());		

		assert(m_LiveSockets[addr]->bSelectWrite);

	}else{

		// TODO: only a subset of packets will raise a new tcp connection.

		//		ֻ�в��ְ�ſ��Խ����µ�tcp���ӣ�������п��ơ???
		U8 type = inPacket->getType();
		U8 subtype = inPacket->getSubType();
		U16 type_combine = (type<<8) | (subtype);
		NewConnectionSet::const_iterator it = this->m_oInitialConnection.find(type_combine);
		if( it != this->m_oInitialConnection.end()) 
		{ // 

		}else 
		{//


		}
		//		raise a new tcp connection.

		s=  new PTCPSocket( PSocket::NONE_BLOCK_SOCK,inPacket->getSocketType());
		assert(s);
		if( false  == s->open() ) 
		{
			LOG_ERROR("PTransImplTcp", "TCPSend(): open error");

			return false;

		}
		if( false == s->connect( addr.getAddr(), addr.getPort(), m_WillCloseSockets) )
		{
			LOG_ERROR("PTransImplTcp", "TCPSend(): connect error:"<<s->toString());
			return false;
		}

		m_LiveSockets[addr] = new SocketINFO(s,true,true);			

	}
	((PTCPSocket *)s)->send( inPacket );

	return true;

}

B8		PTransImplTcp::tcpClose(const PInetAddr & addr ){


	if(m_LiveSockets.find(addr)== m_LiveSockets.end() ||m_LiveSockets[addr]->pSocket->isTCP() ==false ){

		LOG_DEBUG("PTransImplTcp","TCPClose(): the specified socket not exist or not tcp socket: "<< addr.toString());

		return false;
	}

	m_WillCloseSockets.insert(addr);

	return true;

}

void    PTransImplTcp::removeSocket(const PInetAddr	&	inAddr){

	SocketMap::iterator it = m_LiveSockets.begin();

	while( it != m_LiveSockets.end() ){

		LOG_DEBUG("PTransImplTcp","Socket:key:"<<it->first.toString() << " value:"<<it->second->pSocket->toString());

		++it;
	}
	if(  m_LiveSockets.find(inAddr) == m_LiveSockets.end() ) {

		LOG_DEBUG( "PTransImplTcp","removeSocket(): no key:"<< inAddr.toString());

		this->m_WillCloseSockets.erase(inAddr);

		return  ;
	}
	PSocket * s  = m_LiveSockets[inAddr]->pSocket;	

	if( false== s->close() ){

		LOG_ERROR( "PTransImplTcp","removeSocket(): close socket error:"<<s->toString());		
	}
	LOG_DEBUG( "PTransImplTcp","removeSocket(): remove socket in m_LiveSockets:"<<s->toString());

	delete s;

	delete m_LiveSockets[inAddr];

	m_LiveSockets.erase(inAddr);
	//must remember to delete this from m_WillCloseSockets.
	this->m_WillCloseSockets.erase(inAddr);

	return ;
}


void	PTransImplTcp::onRun(){

	B8	nRet = false;

	fd_set		ReadFds, WriteFds, ExceptFds;

	FD_ZERO(&ReadFds);

	FD_ZERO(&WriteFds);

	FD_ZERO(&ExceptFds);

	struct timeval Timeout;

	size_t socknum = m_LiveSockets.size();
	if( socknum >= FD_SETSIZE )
	{
		LOG_ERROR("PTransImplTcp","select sockets more than FD_SETSIZE("<<FD_SETSIZE<<"):"<<socknum);
		//assert(0); // temperorily removed by tuxuping
	}

	const PSettings & Settings = PSettings::getInstance();

	Timeout.tv_sec	=	Settings.Connection.timeoutselectsecond;

	Timeout.tv_usec	=	Settings.Connection.timeoutselectmicroseconds;

	S32 maxsockfd = 0;
	// closing sockets .
	while( !m_WillCloseSockets.empty()){

		removeSocket(*m_WillCloseSockets.begin());

	}
	
	//LOG_DEBUG("PTransImplTcp","m_LiveSockets num is "<<m_LiveSockets.size());

	for ( SocketMap::iterator it = m_LiveSockets.begin(); it != m_LiveSockets.end(); it ++ )
	{
		PSocket* pSocket = it -> second->pSocket;

		S32 sockFd = pSocket->getSocketFD();

		FD_SET(sockFd, &ReadFds);

		// udp packet is sent immediately, so we need not to check its write status.
		// xupingtu 2008-5-29.
		if( pSocket->isTCP() )
		{
			FD_SET(sockFd, &WriteFds);
		}
		FD_SET(sockFd, &ExceptFds);
		maxsockfd = sockFd > maxsockfd ? sockFd : maxsockfd;
	}

	S32     ierr =   select( maxsockfd + 1, &ReadFds ,&WriteFds ,&ExceptFds ,&Timeout);
	//PSocket s = NULL;
	if( ierr ==SOCKET_ERROR) {

		S32 errcode = PUtils::getLastErrorCode();
		LOG_ERROR("PTransImplTcp","OnRun(): select error"<<errcode);		

		// continue.
	}
	// if No Socket availble.
#ifndef _DEBUG
	if( ierr ==0 ) 
	{
		LOG_DEBUG("Transmitter","OnRun(): select nothing,total socket num:"<<socknum);

		return;
	}
#endif

	// read sockets.
	SocketMap::iterator iter = m_LiveSockets.begin();

	while ( iter != m_LiveSockets.end())
	{
		//S32errcode = PUtils::getLastErrorCode();

		PSocket * s = iter->second->pSocket;

		if (FD_ISSET(s->getSocketFD(), &ReadFds))
		{
			this->readDataFromSocket( iter->second ); 
		}
		else if( FD_ISSET(s->getSocketFD(), &WriteFds))
		{
			this->writeDataToSocket(s);
		}
		else if(  FD_ISSET(s->getSocketFD(), &ExceptFds))
		{
			this->handleException( s ) ;
		}

		++iter ;
	}

}

//InPacketQueue* 	PTransImplTcp::getPacketQueue() {
//
//	return &m_oInPacketQueue;
//}

B8		PTransImplTcp::addTcpListener( U32 local_ip, U16	inPort, U32 socket_type)
{
	// the tcp listener only one instance.
	//assert( m_pTCPListener == NULL );

	S32 id = socket_type;

	assert( m_TcpServerMap[id] == 0); 

	m_TcpServerMap[id] = new PTCPSocket( PSocket::NONE_BLOCK_SOCK,id);

	if( m_TcpServerMap[id] == NULL ) {

		LOG_ERROR("PTransImplTcp","Initialize():new a tcp server error");

		return false;
	}
	if( m_TcpServerMap[id]->open() ==false) {

		LOG_ERROR("PTransImplTcp","Initialize():open tcp server error");

		return false;
	}

	// bind tcp 

	const PSettings & Settings = PSettings::getInstance();
	

	//std::string local_ip =  Settings.getConfig("local-ip").m_value.strvalue;

	B8 err = m_TcpServerMap[id]->bind( local_ip,inPort);

	if( err == false ) {

		LOG_ERROR("PTransImplTcp","Initialize():bind tcp server error:");
		std::cout << "bind tcp server error, ip " << local_ip << " port " << inPort << std::endl;
		return false;
	}
	// listen tcp
	if (listen(m_TcpServerMap[id]->getSocketFD(), 4) == -1) {

		//S32 errcode = WSAGetLastError();
		S32 errcode = PUtils::getLastErrorCode(); // 2008-7-22 tuxuping

		LOG_ERROR("PTransImplTcp","Initialize():listener tcp server error:" << errcode);

		return false;
	}

	LOG_DEBUG("PTransImplTcp","data server started: "<< PUtils::long2Addr(local_ip) <<":"<<inPort);

	PInetAddr tcplistener;//, udplistener,localserver;

	tcplistener.set(local_ip,inPort);

	m_LiveSockets[ tcplistener ] =  new SocketINFO(m_TcpServerMap[id], true,true, true) ;	

	return true;

}


B8		PTransImplTcp::initialize( U16 tcpPort, U16 udpPort){

	// start up socket api.
	LOG_DEBUG("PTransImplTcp","Initialize():enter initialize\n");

	U16 tcp_port = tcpPort;

	const PSettings & instance = PSettings::getInstance( );

	if( tcp_port == READ_FROM_CONFIG_FILE ) {

		tcp_port = instance.getConfig("local-tcp-server-port").m_value.intvalue;
	}	

	U32 local_ip = PUtils::addr2Long(instance.getConfig("local-ip").m_value.strvalue) ;

	B8 result = addTcpListener( local_ip, tcp_port ) ;	

	if( result == false ) {

		return result; // if error, please stop initializations.
	}

	// 

	//// tcp server for flash ui.

	//U16 ui_tcp_port = 0;

	//if( ui_tcp_port == READ_FROM_CONFIG_FILE ) {

	//	const PSettings & instance = PSettings::getInstance( );

	//	ui_tcp_port = instance.getConfig("ui-tcp-port").m_value.intvalue;
	//}	

	//
	//local_ip = PUtils::addr2Long("127.0.0.1");

	//result = addTcpListener( local_ip, ui_tcp_port, PSocketType::getFlashAS2Type() ) ;	

	//if( result == false ) {

	//	return result; // if error, please stop initializations.
	//}

	////

	U16 udp_port = udpPort ;

	if( udp_port == READ_FROM_CONFIG_FILE ) {

		const PSettings & instance = PSettings::getInstance( );

		udp_port = instance.getConfig("local-udp-server-port").m_value.intvalue;

	}

	result = addUDPListener( udp_port ) ;

	if( result == false ) {

		return result; // if error, please stop initializations.
	}
	
	////PSettings & instance = PSettings::getInstance( );

	////udp_port = instance.getConfig("ui-udp-port").m_value.intvalue;

	////result = addUDPListener( udp_port, Network::PSocketType::getFlashAS2Type() ) ;
	////
	////if( result == false ) {

	////	return result; // if error, please stop initializations.
	////}
	////
	////LOG_DEBUG("PTransImplTcp","initialize():exit initialize\n");			

	////return result;


	// initialize tcp  and udp
	//struct sockaddr_in tcpaddr; // server address
	//struct sockaddr_in udpaddr; // server address

	//PSettings & instance = PSettings::getInstance( );

	//S16 tcpport = instance->getConfig("tracker-server-port").m_value.intvalue;

	//addTcpListener( tcpport ) ;	

	//LOG_DEBUG("Transmitter","tracker :  initialize(): exit initialize\n");

	return true;

}


//B8		PTransImplTcp::unInitialize(){
//
//	// 1. delete all sockets in the m_LiveSockets;
//	for ( SocketMap::iterator it = m_LiveSockets.begin(); it != m_LiveSockets.end(); it ++ )
//	{
//		PSocket* pSocket = it -> second->pSocket;
//		pSocket->close();		// add by xupingtu 20061012
//		delete pSocket;
//
//		delete it->second;
//
//	}
//
//	m_LiveSockets.clear();
//
//	return true;
//}




void	PTransImplTcp::reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket ){

	tcpSend( inAddr, inPacket );

}

void		PTransImplTcp::readDataFromSocket( SocketINFO * info ) {

	if( info->bIsTcpServer )
	{
		LOG_DEBUG("PTransImplTcp","readDataFromSocket(): ");

		PTCPSocket * s = (PTCPSocket * )(info->pSocket);

		PTCPSocket *s1 = s->accept(m_WillCloseSockets);

		if(s1 == NULL)	// accept error
		{
			return ;
		}

		s1->setSocketType(s->getSocketType());

		// add the accept socket to m_LiveSockets
		PInetAddr key = s1->getDestAddr();
		m_LiveSockets[key]= new SocketINFO(s1, true, true);
		return ;		
	}

	// read all packets. 
	// TODO: we can only read a limited number of packets to prevent from being DDOS a single port.
	// xupingtu 2008-5-29
	while( true ){

		PacketINFO* p = info->pSocket->recv( m_WillCloseSockets);

		if(p && p->packet->getSubType() != PPktDef::NO_DELETE)
		{ //
			//	m_oInPacketQueue.push(p);// removed by tuxuping , 2008-5-22.
			// just for flash_ui_client. it is ugly, but, I dont want to adjust too much 
			// more codes, so...

			// TODO: move these codes to higher layer for PKTown.  tu xuping 2009-6-19
			//if( p->packet->getSocketType() == PSocketTypeFactory::FLASH_AS2_SOCKET_TYPE 
			//	&& info->pSocket->isTCP() )
			//{ // it must be flash packet from ui. so we save the socket and use later.
			//	PSettings &settings = PSettings::getInstance();
			//	S8 temp[8] = { 0 };
			//	PUtils::itoa(p->addr.getPort(), temp, 10);
			//	settings.setConfig("ui-client-tcp-port",PConfigValue::INT_TYPE,
			//		temp);
			//	LOG_DEBUG("PTransImplTcp","readDataFromSocket(): store ui ip:"<<
			//		p->addr.toString());
			//}

			//
			this->handlePkt( p ) ;
			//LOG_WARN("PTransImplTcp"),"OnRun() : m_oInPacketQueue.push(p) packetType = " << (int)p->packet->GetPacketType());
			// added tu. 2010.8.28
		}else  if ( p && p->packet->getSubType() == PPktDef::NO_DELETE)  
		{
			//delete p; // 
			continue;
		}else 			
		{
			break;
		}
	}
}

void	PTransImplTcp::writeDataToSocket( PSocket * s ) 
{
	if( s->isTCP() )
	{
		//printf(" tcp writinging\n");
		((PTCPSocket*)(s))->write(m_WillCloseSockets);
	}
	else	
	{
		//((GUDPSocket*)(s))->write(m_WillCloseSockets); // TODO: tuxuping 2007.11.27
	}
	// omit udp 's writing availble status.
	// for udp packet are send directly without the select function.

}

void	PTransImplTcp::handleException( PSocket * s ) 
{

	S32 errcode;
	S32 len=4;
#ifdef __linux__
	::getsockopt(s->getSocketFD(),SOL_SOCKET,SO_ERROR,(char *)&errcode, (socklen_t*)&len);
#else
	::getsockopt(s->getSocketFD(),SOL_SOCKET,SO_ERROR,(char *)&errcode, (int*)&len);
#endif

	LOG_ERROR("PTransImplTcp","OnRun():exception: "<<errcode<<s->toString());

	// if tcp connection reset
	// raise inter packet.
	if( errcode ==10061 ) // 
	{
		// the service is not active.
		PInetAddr  addr = s->getDestAddr();

		//GPartnerNoExist_Packet * PartnerNoExist_Packet = GPartnerNoExist_Packet::createPacket(1,addr.getAddr(),addr.getPort());
		LOG_DEBUG("PTransImplTcp","select exception: tcp broken, raise inter addr:"<< addr.toString());
		//assert(PartnerNoExist_Packet);

		//PacketINFO* p = new PacketINFO(addr, PartnerNoExist_Packet);
		//assert(p);
		//if( p){
		//	m_oInPacketQueue.push(p);
		//	//LOG_WARN("PTransImplTcp"),"OnRun() : m_oInPacketQueue.push(p) packetType = " << (int)p->packet->GetPacketType());
		//}
	}

	// don't close if udp socket.
	if( s->isTCP())
		m_WillCloseSockets.insert(s->getDestAddr());

}


}
}

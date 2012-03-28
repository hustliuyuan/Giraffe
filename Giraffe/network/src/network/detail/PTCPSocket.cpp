// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.



#include "common/Header.h"
#include <errno.h>

#ifdef __linux__
#include <sys/ioctl.h>
#endif

#include "network/detail/PTCPSocket.h"
#include "os/PUtils.h"
#include "config/PSettings.h"

namespace Network { namespace Detail {

PTCPSocket::PTCPSocket(U32 inBlockOrNot, U32 socket_type)
:PSocket(SOCK_STREAM, inBlockOrNot ), 
m_oRecvBuffer(socket_type), m_oSendBuffer( /*inpt*/ ),timer_begin(0),send_counter(0)
{
	PUtils::setZero(m_cTempBuf,sizeof(m_cTempBuf));
#if defined WIN32 || defined _WIN32
	InitializeCriticalSection(&m_SendQueue_mutex);
	const PSettings & settings= PSettings::getInstance();
	send_pkt_count_num=settings.getConfig("send-count-num").m_value.intvalue;

#endif
}

void	PTCPSocket::setSocketType( U32 inPacketType)
{
	m_oRecvBuffer.setSocketType(inPacketType);
}

U32	PTCPSocket::getSocketType() const 
{
	return this->m_oRecvBuffer.getSocketType()->getId();
}

PTCPSocket::~PTCPSocket()
{
	// delete this old packet.

	//m_Sendinfo.SetZero();
	while( !m_SendQueue.empty() ) 
	{
		const PPacketBase * p = m_SendQueue.front();
		delete p;
		m_SendQueue.pop();
	}

#if defined WIN32 || defined _WIN32
	DeleteCriticalSection(&m_SendQueue_mutex);
#endif
}

B8 PTCPSocket::isTCP()const
{
	return 1; // true
}

PTCPSocket *	 PTCPSocket::createEmptySocket(U32 inBlockOrNot) const
{
	return  new PTCPSocket(inBlockOrNot);
}	

// called by transfer.*************************
//   error socket put to inCloseSockets.
B8 PTCPSocket::write(  AddrSet & inCloseSockets)
{
	// get packet from m_oSendinfo and write to the m_cBuffer.
	
#if defined WIN32 || defined _WIN32
	if(0==timer_begin)timer_begin=::GetTickCount();
	EnterCriticalSection(&m_SendQueue_mutex);
#endif
	while( ! this->m_SendQueue.empty() ) {

		const PPacketBase * pkt = this->m_SendQueue.front();
		assert(pkt != NULL);
		if( this->m_oSendBuffer.tryWritePointer( pkt->getLength() ) )
		{

			this->m_oSendBuffer.writeSendBuffer( pkt );
			send_counter++;
			// removed temperoarily.
			
			char temp_str[2048]={0};
			//pkt->toString(temp_str);	// modified by tu 2010.5.14		
			LOG_DEBUG("TCPSocket","write():0 serialized packet:" << " | towho->:"
				<<this->m_DestAddr.toString()<<"send_counter:"<<send_counter<<"m_Queuesize:"<<m_SendQueue.size());

			this->m_SendQueue.pop(); 

			delete pkt;

		}
		else 
			break;
		//LOG_DEBUG("PTCPSocket","write()1: sendbuffer snapshot: "<<m_oSendBuffer.toString());

	}
#if defined WIN32 || defined _WIN32
	LeaveCriticalSection(&m_SendQueue_mutex);
#endif

	if(  ! this->m_oSendBuffer.getFilledSize( ) ){

		//LOG_DEBUG("TCPSocket","write():2 no data to write.");
		return true ; // no data needed to be sent in this buffer.
	}

	S32 success_size = ::send(m_iFileDesc, m_oSendBuffer.getSendPointer(), m_oSendBuffer.getFilledSize(), 0);

	if( success_size ==SOCKET_ERROR) {
		// assure no this socket in it.
		inCloseSockets.insert(m_DestAddr);	
		LOG_ERROR("TCPSocket","write():3 errno = "<<PUtils::getLastErrorCode()<<" " 
			<< toString()<< " removed from pos 6");
		return false;

	}else 	{

		// move the send_pointer of send_buffer
	    this->m_oSendBuffer.adjustSendPointer( success_size );
		LOG_DEBUG("TCPSocket","write():4 length: " 
			<< success_size << " |towho->:"<<this->m_DestAddr.toString());

	}
#if defined WIN32 || defined _WIN32
	if(send_counter>=send_pkt_count_num)
	{
		//std::cout<<"send-time : "<<::GetTickCount()-timer_begin<<std::endl;
		timer_begin=0;
		send_counter=send_counter-send_pkt_count_num;
	}
#endif
	LOG_DEBUG("PTCPSocket","write():5 sendbuffer snapshot: "<<m_oSendBuffer.toString());

	return true;

}


void PTCPSocket::set(int inSocket, const struct sockaddr_in* remoteaddr)
{
	m_DestAddr.set(*remoteaddr) ;
	m_iFileDesc = inSocket;

	if ( inSocket != kInvalidFileDesc ) 
	{
		//make sure to find out what IP address this connection is actually occuring on. That
		//way, we can report correct information to clients Gking what the connection's IP is

		//struct sockaddr_in LocalAddr;
		int len =  sizeof(m_LocalAddr.getSocketAddr());

#ifdef __linux__
		int err = ::getsockname(m_iFileDesc, (struct sockaddr*)&m_LocalAddr.getSocketAddr(), (socklen_t *)&len);
#else 
		int err = ::getsockname(m_iFileDesc, (struct sockaddr*)&m_LocalAddr.getSocketAddr(), &len);
#endif

		assert(err != SOCKET_ERROR);        
	}
}


PTCPSocket * PTCPSocket::accept(AddrSet & inCloseSockets )
{
	struct sockaddr_in remoteaddr; // client address 
	PUtils::setZero( (char * ) &remoteaddr,sizeof(remoteaddr));

	PTCPSocket *ret = NULL;
	int addrlen =sizeof(sockaddr_in);// must set the len of the remoteaddr. it will be set the true length returned by the accept function.
	int newfd =0;
	PInetAddr addr;
	addr.set(remoteaddr);
#ifdef __linux__
	if ((newfd = ::accept(m_iFileDesc, (struct sockaddr *)&remoteaddr, (socklen_t *)&addrlen)) == SOCKET_ERROR)
	{
#else
	if ((newfd = ::accept(m_iFileDesc, (struct sockaddr *)&remoteaddr, &addrlen)) == SOCKET_ERROR) 
	{
#endif 
		int errcode = PUtils::getLastErrorCode();		
#ifdef __linux__
		if( errcode == ECONNREFUSED )
		{
#else
		if( errcode == WSAECONNRESET )
		{
#endif
			inCloseSockets.insert( addr );
			LOG_ERROR("TCPSocket","accept(): error: WSAECONNRESET. closed by remote before call accept."<<this->toString());			
		}else {

			LOG_ERROR("TCPSocket","accept(): error:"<<errcode<<" when receive connections:"<<this->toString());
		}
		assert( newfd == SOCKET_ERROR ); 
		return ret;		
	}

	int one = 1;

	int err = ::setsockopt( m_iFileDesc, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(int)); 

	if( err ==SOCKET_ERROR) {
		int errcode = PUtils::getLastErrorCode();		
		LOG_ERROR("TCPSocket","accept(): error:"<<errcode<<" not processed: when setsockopt nodelay:"<<this->toString());
	}

	err = ::setsockopt( m_iFileDesc, SOL_SOCKET, SO_KEEPALIVE, (char*)&one, sizeof(int));

	if( err ==SOCKET_ERROR) {
		int errcode = PUtils::getLastErrorCode();		
		LOG_ERROR("TCPSocket","accept(): error:"<<errcode<<" not processed: when setsockopt keepalive:"<<this->toString());
	}

	ret =  PTCPSocket::createEmptySocket( PSocket::NONE_BLOCK_SOCK );

	ret->set(newfd, &remoteaddr);	

	if( ret->m_bIsNoneBlock ){

		ret->setNoneBlockRecv();
	}

	LOG_DEBUG("TCPSocket","accept(): "<<ret->toString());
	return ret;
}

/****************************
parameter:
input:
output:
AddrSet & inCloseSockets 
*****************************/
Network::PacketINFO*	PTCPSocket::recv(AddrSet & inCloseSockets )
{
	int recv_len = 0;

	const PPacketBase * p =  m_oRecvBuffer.getPacket();//the first get to read from buff if the filledbuffsize is larger than the totalpacketsize
	if( p) 
	{
		LOG_DEBUG("ReceiveBuffer","the first get ");
		char temp_str[2048]={0};
		p->toString(temp_str);
		LOG_TRACE("ReceiveBuffer","recv()0: "<<temp_str<< " from:"<< m_DestAddr.toString());
		PacketINFO * pinfo = NULL;
		if( p->getSubType() == PPktDef::NO_DELETE ) 
		{
			pinfo=PacketINFO::getInstance();
			pinfo->packet = p;
		}else 
		{
			pinfo = new PacketINFO( m_DestAddr,this->m_iFileDesc, p);
		}

		return pinfo;
	}

	S32 emptysize =  m_oRecvBuffer.getEmptySize();
	LOG_TRACE("ReceiveBuffer","recv():  emptysize:  "<<emptysize);
	// just for test  tu 2010.8.16


	if( emptysize > TCPPACKET_MAXSIZE)
	{ 
		recv_len = ::recv(m_iFileDesc, m_cTempBuf, sizeof(m_cTempBuf), 0);
		if( recv_len > 0 )
		{
			LOG_TRACE("ReceiveBuffer","the second get ");
			m_oRecvBuffer.writeReceiveBuffer(m_cTempBuf, recv_len);
			const PPacketBase * p =  m_oRecvBuffer.getPacket();
			if( p) 
			{
				char temp_str[2048]={0};
				//p->toString(temp_str);

				LOG_TRACE("ReceiveBuffer","recv()1: "<<temp_str << " from:"
					<< m_DestAddr.toString());
				PacketINFO * pinfo = NULL;
				if( p->getSubType() == PPktDef::NO_DELETE ) 
				{
					pinfo=PacketINFO::getInstance();
					pinfo->packet = p;
				}else 
				{
					pinfo = new PacketINFO( m_DestAddr,this->m_iFileDesc, p);
				}
				return pinfo;
			}
		}
		if( recv_len  == SOCKET_ERROR )
		{

			int err = PUtils::getLastErrorCode();
#ifdef __linux__
			if( err == ESHUTDOWN || err == ECONNABORTED
				|| err == ETIMEDOUT || err == ECONNRESET )
			{
#else
			if( err == WSAESHUTDOWN || err == WSAECONNABORTED 
				|| err == WSAETIMEDOUT||err == WSAECONNRESET )
			{
#endif
				LOG_ERROR("TCPSocket","recv()2: error: "
					<< err<<" "<<toString()<<" was reset from packet receiver,pos:2");
				//LOG_WARN("TCPSocket","recv()2: TODO: add m_DestAddr in CloseSocket container. and removed this comments, test");
				inCloseSockets.insert(m_DestAddr);	
#ifdef __linux__
			}else if(err==ECONNREFUSED)
			{
#else

			}else if(err==WSAECONNREFUSED)
			{
#endif
				inCloseSockets.insert(m_DestAddr);
				LOG_ERROR("TCPSocket","recv()3: error: "
					<< err<<" "<<toString()<<" did not connect or cannot connect");
			}else
#ifdef __linux__
			if(err != EWOULDBLOCK)
			{
#else
			if(err != WSAEWOULDBLOCK)
			{
#endif
				// non-blocking socket
				inCloseSockets.insert(m_DestAddr);	
				LOG_ERROR("TCPSocket","recv()4: error: "
					<< err<<" "<<toString()<<" pos: 4");

			}
	//Error code Meaning 
	// n WSANOTINITIALISED  A successful WSAStartup call must occur before using this function. 
	//   WSAENETDOWN        The network subsystem has failed. 
	// n WSAEFAULT          The buf parameter is not completely contained in a valid part of the user 
	//                      address space. 
	//   WSAENOTCONN        The socket is not connected. 
	//   WSAEINTR           The (blocking) call was canceled through WSACancelBlockingCall. 
	// n WSAEINPROGRESS     A blocking Windows Sockets 1.1 call is in progress, or the service 
	//                      provider is still processing a callback function. 
	// y WSAENETRESET       The connection has been broken due to the keep-alive activity detecting 
	//                      a failure while the operation was in progress. 
	// n WSAENOTSOCK        The descriptor is not a socket. 
	// n WSAEOPNOTSUPP      MSG_OOB was specified, but the socket is not stream-style such as type 
	//   SOCK_STREAM,       OOB data is not supported in the communication domain associated with 
	//                      this socket, or the socket is unidirectional and supports only send operations. 
	// y WSAESHUTDOWN       The socket has been shut down; it is not possible to receive on a socket 
	//                      after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH. 
	// y WSAEWOULDBLOCK     The socket is marked as nonblocking and the receive operation would 
	//                      block. 
	// n WSAEMSGSIZE        The message was too large to fit into the specified buffer and was 
	//                      truncated. 
	// n  WSAEINVAL         The socket has not been bound with bind, or an unknown flag was specified,
	//						or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream 
	//						sockets only) len was zero or negative. 
	// y WSAECONNABORTED	The virtual circuit was terminated due to a time-out or other failure. 
	//						The application should close the socket as it is no longer usable. 
	// y WSAETIMEDOUT			The connection has been dropped because of a network failure or because
	//						the peer system failed to respond. 
	// y WSAECONNRESET		The virtual circuit was reset by the remote side executing a hard 
	//						or abortive close. The application should close the socket as it is no longer 
	//						usable. On a UPD-datagram socket this error would indicate that a previous send 
	//						operation resulted in an ICMP "Port Unreachable" message. 

			
		}
		if (recv_len  == 0 )
		{	// the socket receive finished 
			LOG_WARN("TCPSocket"," "<< toString()<<" was closed by remote side. removed from packet receiver,pos:3");							
			inCloseSockets.insert(m_DestAddr);	 //modified by tuxuping 2007-12-9
			
		}
		
	}
	else
	{
		LOG_DEBUG("TCPSocket","buffer is full,cant write.");							
	}
	return NULL;
}


B8 PTCPSocket::connect(U32 inRemoteIP, U16 inRemotePort , AddrSet & willCloseSockets)
{
	m_DestAddr.set(inRemoteIP, inRemotePort);	

	if( this->setNoneBlockRecv( ) ==false ) 
	{
		S32 err = PUtils::getLastErrorCode();
		LOG_ERROR( "PTCPSocket"," connect():set none blocking error :" 
			<< err<<". deleted this socket");
		assert(0);// added by tuxuping , 2008-5-17
		willCloseSockets.insert( m_DestAddr );
		return false;
	}

	/* don't forget to error check the connect()! */
	int err = ::connect(m_iFileDesc, (sockaddr *)&m_DestAddr.getSocketAddr(), 
		sizeof(m_DestAddr.getSocketAddr()));
	int errornum = PUtils::getLastErrorCode();
			//struct sockaddr_in fLocalAddr; 
	int len=sizeof(m_LocalAddr.getSocketAddr());

#ifdef __linux__
	if(err != -1 || (err == -1 && errornum == EINPROGRESS))
	{
	int err2 = ::getsockname(m_iFileDesc,(struct sockaddr *)&m_LocalAddr.getSocketAddr(), (socklen_t *)&len);	
#else
	if(err != -1 || (err == -1 && errornum == WSAEWOULDBLOCK)){
		int err2= ::getsockname(m_iFileDesc,(struct sockaddr *)&m_LocalAddr.getSocketAddr(), &len);		
#endif 
		if( err2 == SOCKET_ERROR ) {
			
			LOG_WARN("PTCPSocket","connect(): getsockname error: "<<PUtils::getLastErrorCode());
			return false;
		}
	
	}else{
		// error when connect. close the socket.
		LOG_ERROR( "PTCPSocket"," connect(): error ");	
		//assert(0);
		willCloseSockets.insert( m_DestAddr );
		return false;		
	}	
	LOG_DEBUG("TCPSocket","connect():"<<this->toString());


	return true;	
}

void	PTCPSocket::send(const PPacketBase*	inPacket)
{
#if defined WIN32 || defined _WIN32
	EnterCriticalSection(&m_SendQueue_mutex);
#endif
	assert(inPacket != NULL);
	m_SendQueue.push(inPacket);
#if defined WIN32 || defined _WIN32
	LeaveCriticalSection(&m_SendQueue_mutex);
#endif
}

std::string PTCPSocket::toString()const
{
	std::string s;
	s.append(PSocket::toString());
	s.append(" from:");
	s.append(m_LocalAddr.toString());
	s.append(" to:");
	s.append(m_DestAddr.toString());
	return s;
}

} // namespace detail
} // namespace network

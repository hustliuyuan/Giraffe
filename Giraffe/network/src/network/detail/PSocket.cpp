
#include "common/Header.h"
#include <stdlib.h>

#ifdef __linux__
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#endif

#include "network/detail/PSocket.h"
#include "config/PSettings.h"
#include "os/PUtils.h"
#include <sstream>

//InPacketQueue			gPacketQueue;

namespace Network { namespace Detail {

PSocket::~PSocket() 
{	
	if(m_iFileDesc){
		LOG_DEBUG("PSocket", "~PSocket() : close(" << m_iFileDesc << ") ");
#ifdef __linux__
		::close(m_iFileDesc);
#else 
		closesocket(m_iFileDesc);
#endif
	}
}

PSocket::PSocket( U32	inStreamOrDataGram, U32	inBlockOrNot)
{
	this->m_StreamType = inStreamOrDataGram;
	this->m_bIsNoneBlock= inBlockOrNot;

	m_iFileDesc = 0;
}

B8 PSocket::close()
{

#ifdef __linux__
	 int err =	::close(m_iFileDesc);
#else 
	 int err =	closesocket(m_iFileDesc);
#endif

	 if( err ==SOCKET_ERROR ) {
		 LOG_ERROR( "PSocket"," close socket error"  );
		 return false;
	 }	
	 return true;
	 //m_iFileDesc = ~0;
 }


B8 PSocket::open()
{

	m_iFileDesc= socket(PF_INET, m_StreamType, 0);
	if (m_iFileDesc == SOCKET_ERROR){
		LOG_ERROR("PSocket"," open(): create a socket error  errorcode:" <<PUtils::getLastErrorCode());
		return false;
	}
	size_t sockbuff = 512*1024;
	if( 0> setsockopt(m_iFileDesc,SOL_SOCKET,SO_RCVBUF,(const char * )&sockbuff, sizeof(sockbuff))){

		int errcode = PUtils::getLastErrorCode();
		LOG_ERROR("PSocket", "open(): set send buffer size error:"<<errcode);
		//perror("Bind(): bind error: ");  // add by xupingtu 20060811
		return false;
	}

	LOG_DEBUG("PSocket", "open(): create a socket fd = :"<<m_iFileDesc);
	
	if( this->m_bIsNoneBlock ){
		setNoneBlockRecv();
	}

	return true;	
}

Network::PacketINFO*	PSocket::recv(   AddrSet & inCloseSockets ){
	
	return  NULL;//
}

B8   PSocket::setNoneBlockRecv( ) {

	unsigned long nFlag = 1; // set none blocking

#ifdef __linux__
	if(ioctl(m_iFileDesc,FIONBIO,&nFlag) == -1){
#endif
#if defined WIN32 || defined _WIN32
	if(ioctlsocket( m_iFileDesc,FIONBIO,&nFlag) == -1){
#endif
		LOG_ERROR( "PTCPSocket"," set none blocking error ");
		
		return false;
	}

	LOG_DEBUG("PSocket","open(): set no block, success: " );

	return true;

//#ifdef WIN32
//
//	DWORD ul= 1;
//
//	int retval = ioctlsocket(this->m_iFileDesc,FIONBIO,&ul);
//
//	LOG_DEBUG("PSocket","open(): set no block, return code: "<< retval );
//#endif

}

std::string PSocket::toString() const{
	 std::ostringstream s;	 
	 s<<"socket("<<this->getSocketFD()<<")";	 
	 s<<" "<< (isTCP() ? "TCP," : "UDP,");
	 return s.str();
 }

B8 PSocket::bind(U32	inIP, U16 inPort){	

/*	PUtils::setZero( (char * )&m_LocalAddr.m_sockaddr,  sizeof(m_LocalAddr.m_sockaddr));
	
	m_LocalAddr.m_sockaddr.sin_family = AF_INET;
	m_LocalAddr.m_sockaddr.sin_port = htons(inPort);
	m_LocalAddr.m_sockaddr.sin_addr.s_addr = htonl(inIP);*/

	m_LocalAddr.set(inIP,inPort);
	
	///INADDR_ANY; //htonl(inIP); //modified by xupingtu 20060810
	//m_LocalAddr.m_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//if((inIP >> 24) > 223){		// inIP is a multiCast address
	//	m_LocalAddr.m_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//	
	//	struct ip_mreq mreq;
	//	mreq.imr_multiaddr.s_addr = htonl(inIP);
	//	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	//    	LOG_DEBUG("PSocket"), "Bind() Multicast setsocketopt IP_ADD_MEMBERSHIP");
	//	if(0 > setsockopt(m_iFileDesc, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)))
	//	{
	//		perror("setsockopt IP_ADD_MEMBERSHIP is error ");
	//		//close(m_iFileDesc);
	//		LOG_ERROR("PSocket "), " Multicast setsocketopt IP_ADD_MEMBERSHIP error ");
	//		LOG_ERROR("PSocket"), " m_iFileDesc = " << m_iFileDesc );
	//		return ERROR;
	//	}
	//}
	//else {
	//	LOG_DEBUG("PSocket"), "bind() It is a single ip Addr = " << PUtils::Addr2String(inIP, inPort) <<  std::endl); 
	//}
	
	int err = ::bind(m_iFileDesc, (sockaddr *)&m_LocalAddr.getSocketAddr(), sizeof(m_LocalAddr.getSocketAddr()));
	
	if (err == SOCKET_ERROR)
	{
		//m_LocalAddr.m_sockaddr.sin_port = 0;
		//m_LocalAddr.m_sockaddr.sin_addr.s_addr = 0;
		int errcode = PUtils::getLastErrorCode();
		LOG_ERROR("PSocket", "Bind(): bind error:"<<errcode);
		//perror("Bind(): bind error: ");  // add by xupingtu 20060811
		return false;
	}

	size_t sockbuff = 512*1024;
	if( 0> setsockopt(m_iFileDesc,SOL_SOCKET,SO_SNDBUF,(const char * )&sockbuff, sizeof(sockbuff))){

		int errcode = PUtils::getLastErrorCode();
		LOG_ERROR("PSocket", "Bind(): set send buffer size error:"<<errcode);
		//perror("Bind(): bind error: ");  // add by xupingtu 20060811
		return false;
	}
	
	return true;
}



#ifdef __linux__

U32 PSocket::GetEth0Addr()
{
	int sock_gif = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_gif < 0){
		perror("get_myifaddr");
		return 0;
	}
	
	struct ifreq myifr;
	strcpy(myifr.ifr_name, "eth0");
	if(ioctl(sock_gif, SIOCGIFADDR, &myifr)== -1)
	{
		perror("get_myifaddr SIOCGIFADDR");
		::close(sock_gif);
		return 0;
	}
	unsigned long my_ip = ((struct sockaddr_in *)&(myifr.ifr_addr))->sin_addr.s_addr;
	::close(sock_gif);
	return my_ip;
}


#endif
}
}

#include "common/Header.h"
#include "network/detail/PUDPSocket.h"
#include "packet/PPacketFactory.h"
#include "os/PUtils.h"
#include "config/PSettings.h"

namespace Network { namespace Detail {

	PUDPSocket::PUDPSocket( U32	inBlockOrNot, U32 socket_type ):
PSocket(SOCK_DGRAM,inBlockOrNot),send_counter(0),timer_begin(0)//, m_pPacketType(inpt)
{
	PUtils::setZero(m_cInBuff,sizeof(m_cInBuff));
	m_iSocketType= socket_type;
#if defined WIN32 || defined _WIN32
	const PSettings & settings= PSettings::getInstance();
	send_pkt_count_num=settings.getConfig("send-count-num").m_value.intvalue;
#endif
}

PUDPSocket::~PUDPSocket()
{
	if(m_SendList.size() > 0){
		//std::cout << "PUDPSocket maybe memory out m_SendList.size() = " << m_SendList.size() << std::endl;
		for( UDPOutPacketList::iterator it = m_SendList.begin(); it != m_SendList.end();  ++it ) {
			const PacketINFO * info = *it;
			delete info->packet;
			delete info;// added by tuxuping, 2008-7-22
		}
	}
}

B8		PUDPSocket::isTCP()const{
	return 0; // false
}


void		PUDPSocket::send( const PacketINFO * info)
{
#if defined WIN32 || defined _WIN32
	if(0==timer_begin)timer_begin=::GetTickCount();
#endif
	//m_SendList.push_back(info);
	const PPacketBase * p = info->packet;
	struct sockaddr_in remoteAddr = info->addr.getSocketAddr();

	PUtils::setZero(  this->m_cOutBuff,sizeof(m_cOutBuff));

	p->serialize(m_cOutBuff,0 );

	int len = sendto(this->m_iFileDesc, m_cOutBuff, p->getLength(), 0, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) ;

	if(len == SOCKET_ERROR){

		char temp_str[2048]={0};
		//p->toString(temp_str);	
		LOG_ERROR("UDPSocket","send():ERROR! to :" << info->addr.toString() << "errcode:"<<PUtils::getLastErrorCode()
			<<" pkt:" << temp_str);
		//perror("UDPSocket :: write()");
		
		//return;
	}
	else if(len != p->getLength())
	{
		LOG_WARN("UDPSocket","len != pPacket->getLength() "<<len<<" : "<<p->getLength());
	}
	else{
		char temp_str[2048]={0};// modified by tu 2010.5.14
		//p->toString(temp_str);
		LOG_DEBUG("UDPSocket","send():to:"<<info->addr.toString()<<" pkt:" << temp_str);
		
	}	

	// need delete info and packet ? // tuxuping 2008-7-22
	delete info->packet;
	delete info;
#if defined WIN32 || defined _WIN32
	send_counter++;
	if(send_counter>=send_pkt_count_num)
	{
		std::cout<<"send-time : "<<::GetTickCount()-timer_begin<<std::endl;
		timer_begin=0;
		send_counter=send_counter-send_pkt_count_num;
	}
#endif

}

B8 PUDPSocket::write(AddrSet & inCloseSockets )
{
	//B8 retVal = G_SUCCESS;
	//if( m_SendList.empty()) {
	//	return retVal;
	//}

	//UDPOutPacketList::iterator it = m_SendList.begin();
	//const PacketINFO * info = *it;
	//LOG_DEBUG("UDPSocket","write():Writing Packet "<< info->packet->toString() << " to -> " << info->addr.toString());			

	//// send the packet.
	//struct sockaddr_in remoteAddr = info->addr.m_sockaddr;
	//const PPacketBase * pPacket = info->packet;
	//
	//int WriteLen = 0;
	//while(WriteLen < pPacket->GetContentLen()){
	//	int remainSize = pPacket->GetContentLen() - WriteLen;
	//	int sendSize = (remainSize > 1316) ? 1316 : remainSize;
	//	int len = sendto(this->m_iFileDesc, pPacket->GetContent() + WriteLen, sendSize, 0, (struct sockaddr*)&remoteAddr, sizeof(remoteAddr)) ;
	//	if(len == SOCKET_ERROR){
	//		LOG_ERROR("UDPSocket"),"write():Writing Packet  ERROR!  WriteLen = " << WriteLen << " pPacket->GetContentLen() = " << pPacket->GetContentLen());
	//		perror("UDPSocket :: write()");
	//		retVal = ERROR;
	//		break;
	//	}
	//	else{
	//		//LOG_DEBUG("UDPSocket"),"write():Writing Packet send len = "<<len);
	//		WriteLen += len;
	//	}
	//}

	//LOG_DEBUG("UDPSocket"),"write():Writing Packet finished send len = "<<WriteLen );
	//m_SendList.erase(it);

	//
	//delete pPacket;
	//delete info;		

	//return retVal;	
	return false;
}

//   error socket put to inCloseSockets.
//   read packets are put to globally packet queue.
Network::PacketINFO * PUDPSocket::recv(AddrSet & inCloseSockets )
{
	// no use of the argurments of inCloseSockets.
	struct sockaddr_in	remoteAddr;
	int remoteAddrLen = sizeof(struct sockaddr_in);

#ifdef __linux__
	int nPacketLen = recvfrom( m_iFileDesc, this->m_cInBuff, UDPPACKET_MAXSIZE, 0, (struct sockaddr*)&remoteAddr, (socklen_t *)&remoteAddrLen);
#else
	int nPacketLen = recvfrom( m_iFileDesc, this->m_cInBuff, UDPPACKET_MAXSIZE, 0, (struct sockaddr*)&remoteAddr, &remoteAddrLen);
#endif

	if (nPacketLen < 0 ) 
	{
		////LOG_INFO("UDPSocket","recv(): recvfrom err: "<<PUtils::getLastErrorCode() <<"\n");
		return NULL;
	}

	///////////////////////////////// Put packet to UDP queue /////////////////////////////////
	//char* newBuf = new char[nPacketLen]; removed by tuxuping 2008-8-21
	
	//memcpy(newBuf, m_cInBuff, nPacketLen); removed by tuxuping 2008-8-21
	//LOG_DEBUG("UDPSocket"),"recv(): nPacketLen = "<< nPacketLen<< " from: <-"<< m_DestAddr.toString());

	// change packetfactory to this by tuxuping 2009-6-19
	const PPacketFactory & instance = PPacketFactory::getInstance();
	const PPacketBase* pUdpPacket = instance.createPacket(m_cInBuff, nPacketLen, m_iSocketType);

	if(pUdpPacket!=NULL)
	{		
		m_DestAddr.set(remoteAddr);
		////LOG_DEBUG("UDPSocket","recv(): "<< pUdpPacket->toString()<< " from:"<< m_DestAddr.toString());
		//LOG_DEBUG("UDPSocket","recv(): "<< pUdpPacket->toString());

		//gPacketQueue.push( pUdpPacket);
		PacketINFO * pinfo = new PacketINFO( m_DestAddr, this->m_iFileDesc, pUdpPacket);
		return pinfo;

	}
	LOG_ERROR("UDPSocket","recv(): uncompleted packet");
	//delete[] newBuf; removed by tuxuping 2008-8-21. because we use temp buffer. 
	
	return NULL;
}

} // namespace detail
} // namespace network

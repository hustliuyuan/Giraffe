// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/IRecvBufferParser.h"
#include "os/PUtils.h"
#include "packet/PPacketFactory.h"
#include "network/PSocketTypeFactory.h"
#include <sstream>
#include "network/PReceiveBuffer.h"
#include "network/detail/PDefaultRecvBufferParser.h"


namespace Network { namespace Detail {



	//PDefaultRecvBufferParser::~PDefaultRecvBufferParser(void)
	//{

	//}

	const PPacketBase * PDefaultRecvBufferParser::getPacket()
	{
		U32  available  = m_pParent->getFilledSize();
		const Network::PSocketType * pSocketType = m_pParent->getSocketType();
		/////////////////////////
		if( pSocketType->getBaseHeaderLen() > available )
		{
			//LOG_DEBUG("getMyPacket"), "GetBaseHeaderLen() =  " << m_pPacketType->GetBaseHeaderLen()<<" available="<<available);	

			return NULL;
		}
		//U16 type = ntohs( *(U16*)(m_cBuff + m_iReadPointer));
		//assert(type ==0x2448 || type ==0x2444 ); 
		LOG_DEBUG("PDefaultRecvBufferParser","getMyPacket(): m_cBuff: "<<m_pParent->toString());			

		//U16 size = ntohs( *(U16*)(m_cBuff + m_iReadPointer + m_pPacketType->getSizeFrom()));

		U16 size = 0;
		const S8*  buff_header = m_pParent->getBufferHeader();
		U32  readPointer = m_pParent->getReadPointer();
		if( readPointer == PReceiveBuffer::MAX_RCV_BUF_SIZE - 1 ){
			
			S8 temp_size[2] = {0};
			temp_size[0] = *(buff_header+ PReceiveBuffer::MAX_RCV_BUF_SIZE - 1); 
			temp_size[1] = *(buff_header );
			size = *(U16*)(temp_size);
		}else 
		{
			size = *(U16*)(buff_header + readPointer + pSocketType->getSizeFrom());
		}
		LOG_DEBUG("PDefaultRecvBufferParser", "getMyPacket(): size = " 
			<< size<<" GetSizeFrom "<<pSocketType->getSizeFrom());	

		U16 totalpacketsize=(U16)(pSocketType->getSizeCountFrom() + size);

		//LOG_ERROR("getMyPacket"), "m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<<"availabe = " << available << " totalpacketsize = " << totalpacketsize);

		if( available < totalpacketsize)
		{
			LOG_DEBUG("PDefaultRecvBufferParser",
				"getMyPacket() : available < totalpacketSize ! availabe = " 
				<< available << " totalpacketsize = " << totalpacketsize);
			return NULL;
		}
		LOG_DEBUG("PDefaultRecvBufferParser", 
			"getMyPacket() : available >= totalpacketSize ! availabe = " 
			<< available << " totalpacketsize = " << totalpacketsize);
		char * packetbuf = new char [totalpacketsize ];
		//LOG_ERROR("GetMyPacket1"),"m_bFlag = " <<m_bFlag <<"m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<< " totalpacketsize = " <<totalpacketsize<<" m_pPacketType="<<m_pPacketType->getType()<<"packetbuf="<<errorf(packetbuf));
		PUtils::setZero(packetbuf,totalpacketsize);	

		if(0> m_pParent->readReceiveBuffer(packetbuf, totalpacketsize))
		{
			return NULL;
		}

		//ioType = m_pPacketType;
		LOG_DEBUG("PDefaultRecvBufferParser","getMyPacket(): m_cBuff: " 
			<< m_pParent->toString());

		const PPacketFactory & instance = PPacketFactory::getInstance();

		const PPacketBase * pPacket = 
			instance.createPacket(packetbuf,totalpacketsize,pSocketType->getId());
		//if(pPacket == NULL){		// add by xupingtu 20061024 // deleted by tuxuping 2008-5-17
		delete [] packetbuf;
		//}
		return  pPacket;
	}

	//const PPacketBase * IRecvBufferParser::getWAR3Packet( ) {

	//	U32  available  = getFilledSize();
	//	/////////////////////////
	//	if( m_pPacketType->getBaseHeaderLen() > available ){
	//		//LOG_DEBUG("getMyPacket"), "GetBaseHeaderLen() =  " << m_pPacketType->GetBaseHeaderLen()<<" available="<<available);	

	//		return NULL;
	//	}
	//	//U16 type = ntohs( *(U16*)(m_cBuff + m_iReadPointer));
	//	//assert(type ==0x2448 || type ==0x2444 ); 
	//	LOG_DEBUG("IRecvBufferParser"," getWAR3Packet(): m_cBuff "<<IRecvBufferParser::toString());


	//	U16 size =  *(U16*)(m_cBuff + m_iReadPointer + m_pPacketType->getSizeFrom());
	//	LOG_DEBUG("IRecvBufferParser", "getWAR3Packet() : size = " << size<<" GetSizeFrom "
	//		<<m_pPacketType->getSizeFrom());	

	//	U16 totalpacketsize=(U16)(m_pPacketType->getSizeCountFrom() + size) ;

	//	//LOG_ERROR("getMyPacket"), "m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<<"availabe = " << available << " totalpacketsize = " << totalpacketsize);

	//	if( available < totalpacketsize){
	//		LOG_DEBUG("IRecvBufferParser", "getWAR3Packet() : available < totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//		return NULL;
	//	}
	//	LOG_DEBUG("IRecvBufferParser", "getWAR3Packet() : available >= totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//	char * packetbuf = new char [totalpacketsize ];
	//	//LOG_ERROR("GetMyPacket1"),"m_bFlag = " <<m_bFlag <<"m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<< " totalpacketsize = " <<totalpacketsize<<" m_pPacketType="<<m_pPacketType->getType()<<"packetbuf="<<errorf(packetbuf));
	//	PUtils::setZero(packetbuf,totalpacketsize);	

	//	if(0> readReceiveBuffer(packetbuf, totalpacketsize)){
	//		return NULL;
	//	}

	//	//ioType = m_pPacketType;
	//	LOG_DEBUG("IRecvBufferParser","getWAR3Packet():m_cBuff " <<IRecvBufferParser::toString());

	//	const PPacketBase * pPacket = PPacketFactory::createPacket(packetbuf,totalpacketsize,m_pPacketType);
	//	//if(pPacket == NULL){		// add by xupingtu 20061024 // deleted by tuxuping 2008-5-17
	//	delete [] packetbuf;
	//	//}
	//	return  pPacket;
	//}

	//const PPacketBase * IRecvBufferParser::getXmlPacket( ) {

	//	U32  available  = getFilledSize();
	//	/////////////////////////
	//	if( m_pPacketType->getBaseHeaderLen() > available )
	//	{
	//		//LOG_DEBUG("getMyPacket"), "GetBaseHeaderLen() =  " << m_pPacketType->GetBaseHeaderLen()<<" available="<<available);	

	//		return NULL;
	//	}
	//	//U16 type = ntohs( *(U16*)(m_cBuff + m_iReadPointer));
	//	//assert(type ==0x2448 || type ==0x2444 ); 
	//	LOG_DEBUG("IRecvBufferParser","getXmlPacket(): m_cBuff: "<<IRecvBufferParser::toString());			

	//	//U16 size = ntohs( *(U16*)(m_cBuff + m_iReadPointer + m_pPacketType->getSizeFrom()));

	//	U32 size = 0;	

	//	S8 * temp_size_str = new S8[m_pPacketType->getSizeLen() + 1 ] ;

	//	PUtils::setZero(temp_size_str, m_pPacketType->getSizeLen() + 1 );

	//	U32 size_pos =  m_iReadPointer + m_pPacketType->getSizeFrom() ;

	//	// if the size_pos is so big. we must read it from the start of the m_cBuff
	//	if( size_pos >= MAX_RCV_BUF_SIZE ) {

	//		memcpy(temp_size_str, m_cBuff + size_pos % MAX_RCV_BUF_SIZE ,
	//			m_pPacketType->getSizeLen() ) ;
	//	} else 
	//	{
	//		assert(size_pos < MAX_RCV_BUF_SIZE );

	//		if( size_pos > MAX_RCV_BUF_SIZE - m_pPacketType->getSizeLen() + 1 ){

	//			memcpy(temp_size_str, m_cBuff + size_pos  , MAX_RCV_BUF_SIZE - size_pos );
	//			memcpy(temp_size_str + MAX_RCV_BUF_SIZE - size_pos, m_cBuff, 
	//				m_pPacketType->getSizeLen() - ( MAX_RCV_BUF_SIZE - size_pos ));			

	//		}else 
	//		{
	//			memcpy(temp_size_str, m_cBuff + size_pos ,
	//				m_pPacketType->getSizeLen() ) ;

	//			//size = *(U16*)(m_cBuff + m_iReadPointer + m_pPacketType->getSizeFrom());
	//		}
	//	}

	//	sscanf(temp_size_str,"0x%4x",&size);

	//	delete [] temp_size_str;

	//	LOG_DEBUG("IRecvBufferParser", "getXmlPacket(): size = " << size<<" GetSizeFrom "<<m_pPacketType->getSizeFrom());	

	//	U16 totalpacketsize=(U16)(m_pPacketType->getSizeCountFrom() + size);

	//	//LOG_ERROR("getMyPacket"), "m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<<"availabe = " << available << " totalpacketsize = " << totalpacketsize);

	//	if( available < totalpacketsize)
	//	{
	//		LOG_DEBUG("IRecvBufferParser", "getXmlPacket() : available < totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//		return NULL;
	//	}
	//	LOG_DEBUG("IRecvBufferParser", "getXmlPacket() : available >= totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//	char * packetbuf = new char [totalpacketsize ];
	//	//LOG_ERROR("GetMyPacket1"),"m_bFlag = " <<m_bFlag <<"m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<< " totalpacketsize = " <<totalpacketsize<<" m_pPacketType="<<m_pPacketType->getType()<<"packetbuf="<<errorf(packetbuf));
	//	PUtils::setZero(packetbuf,totalpacketsize);	

	//	if(0> readReceiveBuffer(packetbuf, totalpacketsize))
	//	{
	//		return NULL;
	//	}

	//	//ioType = m_pPacketType;
	//	LOG_DEBUG("IRecvBufferParser","getXmlPacket(): m_cBuff: " <<IRecvBufferParser::toString());

	//	const PPacketBase * pPacket = PPacketFactory::createPacket(packetbuf,totalpacketsize,m_pPacketType);
	//	//if(pPacket == NULL){		// add by xupingtu 20061024 // deleted by tuxuping 2008-5-17
	//	delete [] packetbuf;
	//	//}
	//	return  pPacket;
	//}

	//const PPacketBase * PDefaultRecvBufferParser::getPacket()
	//{
	//	const PPacketBase* p = NULL;

	//	//p = getMyPacket();

	//	//switch( m_pPacketType->getId() ){
	//	//case PSocketType::DEFAULT_SOCKET_TYPE:
	//	//	p = getMyPacket();
	//	//	break;
	//	//case PSocketType::WAR3_PACKET:
	//	//	p= getWAR3Packet();
	//	//	break;
	//	//case PSocketType::FLASH_AS2_SOCKET_TYPE:
	//	//	p = getXmlPacket();
	//	//	break;
	//	//default:
	//	//	assert(0);
	//	//}		
	//	return p;
	//}


} // namespace Detail
} // namespace Network
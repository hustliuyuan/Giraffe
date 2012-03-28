// Billion Network Tech Software Inc.
// project:Network 
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "network/PReceiveBuffer.h"
#include "os/PUtils.h"
#include "packet/PPacketFactory.h"
#include "network/PSocketTypeFactory.h"
#include "network/IRecvBufferParser.h"
#include <sstream>
//#include "win32.h"

namespace Network { //namespace Detail {

	PReceiveBuffer::PReceiveBuffer( U32 in_socket_type)
	{
		PUtils::setZero(m_cBuff, MAX_RCV_BUF_SIZE);
		//this->m_iLastCheckedPos = 0;
		this->m_iReadPointer =0;
		this->m_iWritePointer =0;
		this->m_pPacketType = PSocketTypeFactory::getSocketType(in_socket_type);
	}

	PReceiveBuffer::~PReceiveBuffer(void)
	{

	}

	//const PPacketBase * PReceiveBuffer::getMyPacket()
	//{
	//	U32  available  = getFilledSize();
	//	/////////////////////////
	//	if( m_pPacketType->getBaseHeaderLen() > available )
	//	{
	//		//LOG_DEBUG("getMyPacket"), "GetBaseHeaderLen() =  " << m_pPacketType->GetBaseHeaderLen()<<" available="<<available);	

	//		return NULL;
	//	}
	//	//U16 type = ntohs( *(U16*)(m_cBuff + m_iReadPointer));
	//	//assert(type ==0x2448 || type ==0x2444 ); 
	//	LOG_DEBUG("PReceiveBuffer","getMyPacket(): m_cBuff: "<<PReceiveBuffer::toString());			

	//	//U16 size = ntohs( *(U16*)(m_cBuff + m_iReadPointer + m_pPacketType->getSizeFrom()));

	//	U16 size = 0;
	//	if( m_iReadPointer == MAX_RCV_BUF_SIZE - 1 ){
	//		
	//		S8 temp_size[2] = {0};
	//		temp_size[0] = *(m_cBuff+ MAX_RCV_BUF_SIZE - 1); 
	//		temp_size[1] = *(m_cBuff );
	//		size = *(U16*)(temp_size);
	//	}else 
	//	{
	//		size = *(U16*)(m_cBuff + m_iReadPointer + m_pPacketType->getSizeFrom());
	//	}
	//	LOG_DEBUG("PReceiveBuffer", "getMyPacket(): size = " << size<<" GetSizeFrom "<<m_pPacketType->getSizeFrom());	

	//	U16 totalpacketsize=(U16)(m_pPacketType->getSizeCountFrom() + size);

	//	//LOG_ERROR("getMyPacket"), "m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<<"availabe = " << available << " totalpacketsize = " << totalpacketsize);

	//	if( available < totalpacketsize)
	//	{
	//		LOG_DEBUG("PReceiveBuffer", "getMyPacket() : available < totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//		return NULL;
	//	}
	//	LOG_DEBUG("PReceiveBuffer", "getMyPacket() : available >= totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//	char * packetbuf = new char [totalpacketsize ];
	//	//LOG_ERROR("GetMyPacket1"),"m_bFlag = " <<m_bFlag <<"m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<< " totalpacketsize = " <<totalpacketsize<<" m_pPacketType="<<m_pPacketType->getType()<<"packetbuf="<<errorf(packetbuf));
	//	PUtils::setZero(packetbuf,totalpacketsize);	

	//	if(0> readReceiveBuffer(packetbuf, totalpacketsize))
	//	{
	//		return NULL;
	//	}

	//	//ioType = m_pPacketType;
	//	LOG_DEBUG("PReceiveBuffer","getMyPacket(): m_cBuff: " <<PReceiveBuffer::toString());

	//	const PPacketFactory & instance = PPacketFactory::getInstance();

	//	const PPacketBase * pPacket = instance.createPacket(packetbuf,totalpacketsize,m_pPacketType->getId());
	//	//if(pPacket == NULL){		// add by xupingtu 20061024 // deleted by tuxuping 2008-5-17
	//	delete [] packetbuf;
	//	//}
	//	return  pPacket;
	//}

	//const PPacketBase * PReceiveBuffer::getWAR3Packet( ) {

	//	U32  available  = getFilledSize();
	//	/////////////////////////
	//	if( m_pPacketType->getBaseHeaderLen() > available ){
	//		//LOG_DEBUG("getMyPacket"), "GetBaseHeaderLen() =  " << m_pPacketType->GetBaseHeaderLen()<<" available="<<available);	

	//		return NULL;
	//	}
	//	//U16 type = ntohs( *(U16*)(m_cBuff + m_iReadPointer));
	//	//assert(type ==0x2448 || type ==0x2444 ); 
	//	LOG_DEBUG("PReceiveBuffer"," getWAR3Packet(): m_cBuff "<<PReceiveBuffer::toString());


	//	U16 size =  *(U16*)(m_cBuff + m_iReadPointer + m_pPacketType->getSizeFrom());
	//	LOG_DEBUG("PReceiveBuffer", "getWAR3Packet() : size = " << size<<" GetSizeFrom "
	//		<<m_pPacketType->getSizeFrom());	

	//	U16 totalpacketsize=(U16)(m_pPacketType->getSizeCountFrom() + size) ;

	//	//LOG_ERROR("getMyPacket"), "m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<<"availabe = " << available << " totalpacketsize = " << totalpacketsize);

	//	if( available < totalpacketsize){
	//		LOG_DEBUG("PReceiveBuffer", "getWAR3Packet() : available < totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//		return NULL;
	//	}
	//	LOG_DEBUG("PReceiveBuffer", "getWAR3Packet() : available >= totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//	char * packetbuf = new char [totalpacketsize ];
	//	//LOG_ERROR("GetMyPacket1"),"m_bFlag = " <<m_bFlag <<"m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<< " totalpacketsize = " <<totalpacketsize<<" m_pPacketType="<<m_pPacketType->getType()<<"packetbuf="<<errorf(packetbuf));
	//	PUtils::setZero(packetbuf,totalpacketsize);	

	//	if(0> readReceiveBuffer(packetbuf, totalpacketsize)){
	//		return NULL;
	//	}

	//	//ioType = m_pPacketType;
	//	LOG_DEBUG("PReceiveBuffer","getWAR3Packet():m_cBuff " <<PReceiveBuffer::toString());

	//	const PPacketBase * pPacket = PPacketFactory::createPacket(packetbuf,totalpacketsize,m_pPacketType);
	//	//if(pPacket == NULL){		// add by xupingtu 20061024 // deleted by tuxuping 2008-5-17
	//	delete [] packetbuf;
	//	//}
	//	return  pPacket;
	//}

	//const PPacketBase * PReceiveBuffer::getXmlPacket( ) {

	//	U32  available  = getFilledSize();
	//	/////////////////////////
	//	if( m_pPacketType->getBaseHeaderLen() > available )
	//	{
	//		//LOG_DEBUG("getMyPacket"), "GetBaseHeaderLen() =  " << m_pPacketType->GetBaseHeaderLen()<<" available="<<available);	

	//		return NULL;
	//	}
	//	//U16 type = ntohs( *(U16*)(m_cBuff + m_iReadPointer));
	//	//assert(type ==0x2448 || type ==0x2444 ); 
	//	LOG_DEBUG("PReceiveBuffer","getXmlPacket(): m_cBuff: "<<PReceiveBuffer::toString());			

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

	//	LOG_DEBUG("PReceiveBuffer", "getXmlPacket(): size = " << size<<" GetSizeFrom "<<m_pPacketType->getSizeFrom());	

	//	U16 totalpacketsize=(U16)(m_pPacketType->getSizeCountFrom() + size);

	//	//LOG_ERROR("getMyPacket"), "m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<<"availabe = " << available << " totalpacketsize = " << totalpacketsize);

	//	if( available < totalpacketsize)
	//	{
	//		LOG_DEBUG("PReceiveBuffer", "getXmlPacket() : available < totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//		return NULL;
	//	}
	//	LOG_DEBUG("PReceiveBuffer", "getXmlPacket() : available >= totalpacketSize ! availabe = " << available << " totalpacketsize = " << totalpacketsize);
	//	char * packetbuf = new char [totalpacketsize ];
	//	//LOG_ERROR("GetMyPacket1"),"m_bFlag = " <<m_bFlag <<"m_iReadPointer = " <<m_iReadPointer<<"m_iWritePointer = " <<m_iWritePointer<< " totalpacketsize = " <<totalpacketsize<<" m_pPacketType="<<m_pPacketType->getType()<<"packetbuf="<<errorf(packetbuf));
	//	PUtils::setZero(packetbuf,totalpacketsize);	

	//	if(0> readReceiveBuffer(packetbuf, totalpacketsize))
	//	{
	//		return NULL;
	//	}

	//	//ioType = m_pPacketType;
	//	LOG_DEBUG("PReceiveBuffer","getXmlPacket(): m_cBuff: " <<PReceiveBuffer::toString());

	//	const PPacketBase * pPacket = PPacketFactory::createPacket(packetbuf,totalpacketsize,m_pPacketType);
	//	//if(pPacket == NULL){		// add by xupingtu 20061024 // deleted by tuxuping 2008-5-17
	//	delete [] packetbuf;
	//	//}
	//	return  pPacket;
	//}

	const PPacketBase * PReceiveBuffer::getPacket()
	{
		const PPacketBase* p = NULL;

		Network::IRecvBufferParser *  parser = 
			PSocketTypeFactory::getBufferParser(m_pPacketType->getId());
		parser->setRecvBuffer(this);

		return parser->getPacket();

	}

	B8 PReceiveBuffer::isCanWrite( U32 size)const
	{
		B8 retval = true;
		if(getEmptySize() < (S32)size){	// modified by xupingtu 20060919
			retval = false;
		}
		return retval;
	}

	void	PReceiveBuffer::setSocketType( U32 sock_type)
	{
		m_pPacketType = PSocketTypeFactory::getSocketType( sock_type );
	}

	const  PSocketType*	PReceiveBuffer::getSocketType( ) const
	{
		return this->m_pPacketType;

	}

	S32		PReceiveBuffer::getEmptySize() const
	{
		S32 emptySize = 0;
		if( m_iWritePointer < m_iReadPointer  ){
			emptySize  =  m_iReadPointer - m_iWritePointer;
		}
		else  {
			emptySize  = MAX_RCV_BUF_SIZE - (m_iWritePointer -  m_iReadPointer);
		}
		return emptySize -1;	// 留出一个空位来，不让写指针追上读指针，避免对读写指针相等的判断
	}

	U32		PReceiveBuffer::getFilledSize() const
	{
		S32 filledSize = 0;

		if(m_iWritePointer < m_iReadPointer){
			filledSize = MAX_RCV_BUF_SIZE + m_iWritePointer - m_iReadPointer;
		}
		else{
			filledSize = m_iWritePointer - m_iReadPointer;
		}
		return filledSize;
	}

	std::string PReceiveBuffer::toString()const
	{
		std::ostringstream  s;
		s<<" capacity:"<<MAX_RCV_BUF_SIZE;
		s<<" read_pos:"<<this->m_iReadPointer<< " write_pos:"<<this->m_iWritePointer;
		s<<" empty_area("<<getEmptySize()<<"):(";
		if( m_iWritePointer < m_iReadPointer ) 
		{
			s<<m_iWritePointer<<","<<m_iReadPointer<<"),";
		}else
		{
			s<<m_iWritePointer<<","<<MAX_RCV_BUF_SIZE<<")";

			if( m_iReadPointer > 1 ) 
			{
				s<<"||(0,"<<m_iReadPointer-1<<")";
			}
		}	
		s<<" filled_area("<<getFilledSize()<<"):(";
		if(m_iWritePointer < m_iReadPointer)
		{
			s<<m_iReadPointer<<","<<MAX_RCV_BUF_SIZE<<")";
			if( m_iWritePointer >1 ) {

				s<<"||(0,"<<m_iWritePointer-1<<")";
			}
		}else
		{
			s<<m_iReadPointer<<","<<m_iWritePointer<<"),";
		}
		return s.str();
	}

	S32		PReceiveBuffer::readReceiveBuffer(char * dstBuff, const S32 readLen)
	{
		if(dstBuff == NULL){
			return -1;
		}
		if((S32)getFilledSize() < readLen){
			return -1;
		}

		S32 maxContinueReadSize = MAX_RCV_BUF_SIZE - m_iReadPointer;
		if(maxContinueReadSize >= readLen){
			memcpy(dstBuff, m_cBuff + m_iReadPointer, readLen);
			//LOG_ERROR("readReceiveBuffer"),"read 1 a packet dstBuff = " <<PReceiveBuffer::toString(dstBuff));

		}
		else{

			memcpy(dstBuff, m_cBuff + m_iReadPointer, maxContinueReadSize);
			memcpy(dstBuff + maxContinueReadSize, m_cBuff, readLen - maxContinueReadSize);
			//LOG_ERROR("readReceiveBuffer"),"read 2 a packet dstBuff = " <<PReceiveBuffer::toString(dstBuff));

		}

		m_iReadPointer = (m_iReadPointer + readLen) % MAX_RCV_BUF_SIZE;
		
		// added by tuxuping 2008-10-31
		if( m_iReadPointer == m_iWritePointer )
		{
			m_iReadPointer = 0;
			m_iWritePointer = 0;
		}
		return readLen;
	}

	S32		PReceiveBuffer::writeReceiveBuffer(const char *pSrcBuff, const S32 writeLen)
	{
		if(pSrcBuff == NULL){
			return -1;
		}
		if(getEmptySize() < writeLen){
			return -1;
		}

		S32 maxContinueWriteSize = MAX_RCV_BUF_SIZE - m_iWritePointer;
		if(maxContinueWriteSize >= writeLen){
			memcpy(m_cBuff + m_iWritePointer, pSrcBuff, writeLen);
			//LOG_ERROR("writeReceiveBuffer"),"write 1 a packet pSrcBuff = " <<PReceiveBuffer::toString(m_cBuff + m_iWritePointer));

		}
		else{
			memcpy(m_cBuff + m_iWritePointer, pSrcBuff, maxContinueWriteSize);
			memcpy(m_cBuff, pSrcBuff + maxContinueWriteSize, writeLen - maxContinueWriteSize);
			//LOG_ERROR("writeReceiveBuffer"),"write 2 a packet pSrcBuff = " <<PReceiveBuffer::toString(m_cBuff + m_iWritePointer));

		}

		m_iWritePointer = (m_iWritePointer + writeLen) % MAX_RCV_BUF_SIZE;
		return writeLen;
	}



	void	PReceiveBuffer::setReadPointer(U32  pos) 
	{
		this->m_iReadPointer = pos;
	}
	U32		PReceiveBuffer::getReadPointer() const
	{
		return m_iReadPointer;
	}

	//void	PReceiveBuffer::setWritePointer(U32  pos) 
	//{
	//	this->m_iWritePointer = pos;
	//}
	U32		PReceiveBuffer::getWritePointer() const
	{
		return m_iWritePointer;
	}

	S8*		PReceiveBuffer::getBufferHeader()const
	{
		return (S8 * )m_cBuff;
	}

	void	PReceiveBuffer::toString(std::string	& sss) const 
	{
		sss.append( toString());
	}

	void	PReceiveBuffer::outputState()const
	{
		LOG_DEBUG("PReceiveBuffer","outputState:" << toString());
	}	
	void	PReceiveBuffer::toString(S8 * str, U32 from_pos, U32 total_len ) const 
	{
		String  to = toString();
		U32 len = ( to.size() < total_len - from_pos ) ? to.size(): total_len - from_pos -1 ;
		memcpy(str+from_pos, to.c_str(),len );
		return ;
	}

//} // namespace detail
} // namespace network
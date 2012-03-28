// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/detail/psendbuffer.h"
#include "os/PUtils.h"
#include <sstream>

namespace Network { namespace Detail {

PSendBuffer::PSendBuffer(/*const PSocketType * packettype*/)
{
	PUtils::setZero(m_cBuff, MAX_SEND_BUF_SIZE);
	//this->m_iLastCheckedPos = 0;
	this->m_iSendPointer =0;
	this->m_iWritePointer =0;
	this->m_iEndPointer = 0;
	//this->m_pPacketType = packettype;
}

PSendBuffer::~PSendBuffer(void)
{

}


bool PSendBuffer::isCanWrite( S32 size)const
{
	bool retval = true;
	if(getEmptySize() < size){	// modified by xupingtu 20060919
		retval = false;
	}
	return retval;
}

//void PSendBuffer::setSocketType(const PSocketType* packettype)
//{
//	m_pPacketType = packettype;
//}

S32 PSendBuffer::getEmptySize() const
{
	int emptySize = 0;
	if( m_iWritePointer < m_iSendPointer  ){

		emptySize  =  m_iSendPointer - m_iWritePointer - 1;
	}
	else  {

		emptySize  = MAX_SEND_BUF_SIZE - 1 -  m_iWritePointer ;
	}
	return emptySize ;	// ����һ����λ��������дָ��׷�϶�ָ�룬����Զ�дָ����ȵ��ж�
}

S32 PSendBuffer::getFilledSize() const
{
	int filledSize = 0;

	filledSize = this->m_iEndPointer - this->m_iSendPointer;

	return filledSize;
}

std::string PSendBuffer::toString() const
{
	std::ostringstream s;
	s<<"send_pos:"<< this->m_iSendPointer;
	s<<" write_pos:"<< this->m_iWritePointer;
	s<<" end_pos:"<< this->m_iEndPointer;
	s<<" empty_area("<<getEmptySize()<<"):";
	s<<" filled_size("<<getFilledSize()<<"):";
	return s.str();
	
}




S32 PSendBuffer::writeSendBuffer(const PPacketBase * inPacket)
{

	if(inPacket == NULL){
		LOG_ERROR("PSendBuffer","writeSendBuffer(): inPacket is NULL");
		return -1;
	}

	if(getEmptySize() <= (S32 ) inPacket->getLength()){

		LOG_ERROR("PSendBuffer","writeSendBuffer(): empty size is not enough.");
		assert(0);
		return -1;
	}

	// determine where to write.
	S32   write_pointer = this->getWritePointer();

	inPacket->serialize( this->m_cBuff, this->getWritePointer() );

	this->adjustWritePointer( inPacket->getLength());

	LOG_DEBUG("PSendBuffer","writeSendBuffer(): "<<toString());

	return 1;

}


const S8*	PSendBuffer::getSendPointer() const {

	return ( this->m_cBuff + this->m_iSendPointer );

}

S32		PSendBuffer::getWritePointer( ) const {

	return this->m_iWritePointer;

}

void	PSendBuffer::adjustSendPointer( S32  sent_len ){

	this->m_iSendPointer += sent_len;	

	// ���û�з����꣬��ֻ�ǵ���sender_pointer.
	if( this->m_iSendPointer < this->m_iEndPointer )

		return ;

	// ����
	if( this->m_iSendPointer == this->m_iEndPointer ) {

		if( ( this->m_iWritePointer == this->m_iSendPointer )  || ( this->m_iWritePointer == 0 ) ) {

			this->m_iEndPointer = 0;

			this->m_iSendPointer = 0;

			this->m_iWritePointer = 0;
		

		}else { 

			assert( ( this->m_iWritePointer  > 0 ) && ( this->m_iWritePointer < this->m_iEndPointer  )) ;

			this->m_iEndPointer = this->m_iWritePointer ;

			this->m_iSendPointer = 0; 
			
		}

	}
	return ;

}

void	PSendBuffer::adjustWritePointer( S32  write_len ) {

	if( ( this->m_iSendPointer < this->m_iEndPointer )&& 
		(this->m_iWritePointer == this->m_iEndPointer )) {

		assert( this->m_iWritePointer + write_len < MAX_SEND_BUF_SIZE -1 );

	}else if ( this-> m_iSendPointer > this->m_iWritePointer )  {

		assert( this->m_iWritePointer + write_len + 1 < m_iSendPointer  );
	}

	this->m_iWritePointer += write_len;

	this->m_iEndPointer = this->m_iWritePointer;

}

B8	PSendBuffer::tryWritePointer( S32  write_len ) {

	if(( this->m_iSendPointer < this->m_iEndPointer )&& 
		(this->m_iWritePointer == this->m_iEndPointer )&&
		(this->m_iWritePointer + write_len > MAX_SEND_BUF_SIZE -1 ) )
	{
		
		// removed by tuxuping. 2008-7-25
		//assert( this->m_iSendPointer > MAX_SEND_BUF_SIZE -1 - this->m_iEndPointer  );
		if( this->m_iSendPointer > MAX_SEND_BUF_SIZE -1 - this->m_iEndPointer  )
		{
			this->m_iWritePointer = 0 ;	
		}
		// else , do not adjust writePointer.
		
	}

	return isCanWrite( write_len ) ;	

}

} // namespace detail
} // namespace network

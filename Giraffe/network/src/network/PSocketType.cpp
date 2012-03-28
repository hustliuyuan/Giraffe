// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "os/PUtils.h"
#include "network/PSocketType.h"


namespace Network {


	PSocketType::PSocketType(U32  iType,char * inDeli, U32 iDeliSize, U32  iSizeFrom, U32 iSizeLen, U32  iSizeCountFrom){

		PUtils::setZero( m_cDelimitation, sizeof( m_cDelimitation ) );
		if( inDeli!=NULL)
		{
			size_t len = strlen(inDeli);
			// added by tuxuping 2009-6-19. check the inDelisize.
			if( iDeliSize > len )
			{
				LOG_ERROR("PSocketType","constructor: too much inDelisize!");
				return ;
			}
			memcpy(this->m_cDelimitation, inDeli, iDeliSize);
		}
		this->m_iDelimitationSize = iDeliSize;
		this->m_iSizeFrom = iSizeFrom;
		this->m_iSizeLen = iSizeLen;
		this->m_iSizeCountFrom = iSizeCountFrom;

		this->m_iId = iType;
	}

	PSocketType::~PSocketType(void)
	{

	}
	U32		PSocketType::getSizeFrom()const {
		return this->m_iSizeFrom;
	}
	U32		PSocketType::getSizeLen() const{
		return this->m_iSizeLen;

	}
	U32		PSocketType::getSizeCountFrom() const {
		return this->m_iSizeCountFrom;

	}
	void		PSocketType::getDelimitation( char *  store, U32  * storeLen) const{


		if( this->m_cDelimitation != NULL){
			memcpy(store, this->m_cDelimitation, m_iDelimitationSize);
			*storeLen = m_iDelimitationSize;
		}
	}



	U32		PSocketType::getBaseHeaderLen( )const{

		return m_iSizeFrom + m_iSizeLen ;
	}

	U32		PSocketType::getId() const {
		return m_iId;
	}


}
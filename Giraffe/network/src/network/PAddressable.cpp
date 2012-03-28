// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/PAddressable.h"
#include "os/PUtils.h"


namespace Network {

	

	PAddressable::~PAddressable()
	{


	}


	PAddressable::PAddressable() 
	{
		//Set( (const unsigned char*)NULL, 0 );
		m_iGIP = 0;
		m_iRIP = 0 ;
		m_cNetStatus = 0 ;		
		m_sGPort = 0 ;
		m_sRPort = 0 ;
	}

	PAddressable::PAddressable( U32 pub_ip, U16 pub_port, 
		U32 pri_ip, U16 pri_port, S8 netstatus,
		const unsigned char *pHost, unsigned short port )
	{
		m_iGIP = pub_ip;
		m_iRIP = pri_ip ;
		m_cNetStatus = netstatus ;		
		m_sGPort = pub_port ;
		m_sRPort = pri_port ;

		//Set( pHost, port );


	}

	/*void PAddressable::Set( const unsigned char *pHost, unsigned short port )
	{
		memset( &m_sockaddr, 0, sizeof (struct SOCKADDR) );

		m_sockaddr.sin_family = AF_INET;

		if ( NULL != pHost )
		{
			memcpy( &m_sockaddr.sin_addr, pHost, 16 );
		}
		else
		{
			memset( &m_sockaddr.sin_addr, 0, 16 );
		}

		m_sockaddr.sin_port = htons(port);

		m_socklen = sizeof (m_sockaddr);
	}
	
	void PAddressable::Set( struct SOCKADDR sockaddr )
	{
		m_sockaddr = sockaddr;
		m_socklen = sizeof (m_sockaddr);
	}

	void PAddressable::Set( unsigned int addr, unsigned short port )
	{
		m_sockaddr.sin_family = AF_INET;
		m_sockaddr.sin_addr.s_addr = htonl( addr );
		m_sockaddr.sin_port = htons( port );
		m_socklen = sizeof ( m_sockaddr );
	}*/

	void PAddressable::InetAddrLongToString( unsigned long iAddr, char *strAddr )
	{
		sockaddr_in addr;

		addr.sin_addr.s_addr = htonl(iAddr);
		char *temp = inet_ntoa(addr.sin_addr);    
		::strcpy( strAddr, temp );
	}

	U32		PAddressable::getLength( ) const
	{

		return 13;

	}


	U32 PAddressable::serialize ( S8 *  buf,  S32 offset ) const
	{
		S8* buffer = buf+offset;

		writeU32 (buffer, m_iGIP);
		buffer += 4;
		writeU32 (buffer, m_iRIP);
		buffer += 4;
		writeU8 (buffer, m_cNetStatus);
		buffer++;
		writeU16 (buffer, m_sGPort);
		buffer += 2;
		writeU16 (buffer, m_sRPort);
		buffer += 2;

		return getLength();
	}

	U32 PAddressable::deserialize ( const S8 * buf, S32 offset )
	{
		const S8* buffer = buf+offset;

		readU32 (buffer, m_iGIP);
		buffer += 4;
		readU32 (buffer, m_iRIP);
		buffer += 4;
		readU8 (buffer, m_cNetStatus);
		// added by tuxuping 2008-9-26
		//1111,0111. (binary)  the 4th bit from the left is if this address transitted or not 
		m_cNetStatus &= 247;  
		buffer++;
		readU16 (buffer, m_sGPort);
		buffer += 2;
		readU16 (buffer, m_sRPort);
		buffer += 2;


		return getLength();
	}


	String		PAddressable::toString()const
	{
		std::ostringstream s;
		s<<" gaddr:"<<PUtils::long2Addr(m_iGIP);
		s<<":"<<m_sGPort;
		s<<",raddr:"<<PUtils::long2Addr(m_iRIP);
		s<<":"<<m_sRPort;
		s<<" nstat:"<<(U32)m_cNetStatus;
		return s.str();
	}

	U32	PAddressable::toString(S8 *  to_str)const
	{
		std::ostringstream s;
		s<<" gaddr:"<<PUtils::long2Addr(m_iGIP);
		s<<":"<<m_sGPort;
		s<<",raddr:"<<PUtils::long2Addr(m_iRIP);
		s<<":"<<m_sRPort;
		s<<" nstat:"<<(U32)m_cNetStatus;

		String str1 = s.str();	
		memcpy(to_str, str1.c_str(),str1.size());
		return str1.size();
	}

	PAddressable &PAddressable::operator = ( const PAddressable &right )
	{
		m_iGIP = right.getGIP();
		m_iRIP = right.getRIP();
		m_cNetStatus = right.getNetStatus();		
		m_sGPort = right.getGPort();
		m_sRPort = right.getRPort();
		return *this;
	}

	B8		PAddressable::operator	<( const PAddressable & addrssable ) const 
	{
		// the global ip and address determins all.
		if( this->m_iGIP < addrssable.getGIP())
		{
			return true;
		}
		else if( this->m_iGIP == addrssable.getGIP())
		{
			return (this->m_sGPort < addrssable.getGPort());
		}
		return false;

	}

	B8		PAddressable::operator	==( const PAddressable & addrssable ) const
	{

		if( (this->m_iGIP == addrssable.getGIP()) 
			&& (this->m_sGPort == addrssable.getGPort())
			&& (this->m_iRIP == addrssable.getRIP()) 
			&& (this->m_sRPort == addrssable.getRPort())
			 ) 
			// the old comparing operator is '<', astonished
			// I am fucking this error. tuxuping .2008-9-29
			// and , the important is : 
			// in some nat server, different private address is mapped to 
			// same public address, so we must compare all the private address
			// and public address, or it will raise disaster if we use PAddreassable
			// as a unique key in a map.
			// tuxuping .2008-9-29

		{
			return true;
		}
		else 
		{
			return false;

		}
	}

	U32 PAddressable::getGIP ()const
	{
		return m_iGIP;
	}

	U32 PAddressable::getRIP ()const
	{
		return m_iRIP;
	}


	B8 PAddressable::hasUPnP ()const
	{
		return 0 != (m_cNetStatus & 1);
	}

	B8 PAddressable::hasFireWall ()const
	{
		return 0 != (m_cNetStatus & 2);
	}

	B8 PAddressable::hasIP ()const
	{
		return 0 != (m_cNetStatus & 4);
	}

	U16 PAddressable::getGPort ()const
	{
		return m_sGPort;
	}

	U16 PAddressable::getRPort ()const
	{
		return m_sRPort;
	}

	U8	PAddressable::getNetStatus()const  
	{
		return m_cNetStatus;
	}
	void PAddressable::setNetStatus (U8 x)
	{
		m_cNetStatus = x;
	}

	void PAddressable::setGIP (U32 x)
	{
		m_iGIP = x;
	}

	void PAddressable::setRIP (U32 x)
	{
		m_iRIP = x;
	}


	void PAddressable::setGPort (U16 x)
	{
		m_sGPort = x;
	}

	void PAddressable::setRPort (U16 x)
	{
		m_sRPort = x;
	}

		// it is 0, added by tuxuping , 2008-7-18
	B8		PAddressable::isNull() const
	{
		return ( this->m_cNetStatus == 0 &&
			     this->m_iGIP ==0 &&
				 this->m_iRIP ==0 &&
				 this->m_sGPort ==0 &&
				 this->m_sRPort ==0 );

	}

		// get a null object. added by tuxuping , 2008-7-18
	const PAddressable &	PAddressable::getNull() 
	{
		static	PAddressable null_object ;	
		return  null_object;
	}

	B8	PAddressable::isFail2Transit()const
	{
		return 0 != (m_cNetStatus & 8);
	}
	void  PAddressable::setFailTransit( B8	fail_or_not )
	{
		m_cNetStatus |= ( fail_or_not ? 8 :0 );
	}

}




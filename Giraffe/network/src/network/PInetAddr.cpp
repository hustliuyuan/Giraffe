// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "network/PInetAddr.h"
#include "os/PUtils.h"
#include <string.h>


namespace Network
{


    PInetAddr::PInetAddr( const char * pHost, U16 port )
    {
        set( pHost, port );
    }

    void PInetAddr::set( const S8 * pHost, U16 port )
    {
        m_sockaddr.sin_family		=	AF_INET;
		m_sockaddr.sin_addr.s_addr	=	(pHost) ? inet_addr(pHost) : INADDR_NONE;
        m_sockaddr.sin_port			=	htons( port );
        //m_socklen					=	sizeof( m_sockaddr );
    }

    void PInetAddr::set( U32 addr, U16 port )
    {
        m_sockaddr.sin_family		=	AF_INET;

		m_sockaddr.sin_addr.s_addr = htonl(addr);


        m_sockaddr.sin_port			=	htons( port );
       // m_socklen					=	sizeof( m_sockaddr );
    }

    void PInetAddr::set( const sockaddr_in& addr)
    {
        m_sockaddr					=	addr;
        //m_socklen					=	sizeof( addr );
    }

	PInetAddr::PInetAddr( U32 addr, U16 port )
	{
		set(addr,port);
	}


    const S8 * PInetAddr::getIPAddr() const
    {
		//not re-enterable
        return inet_ntoa( m_sockaddr.sin_addr );
    }

    U32 PInetAddr::getAddr() const
    {
        return ntohl( m_sockaddr.sin_addr.s_addr );
    }

    U16 PInetAddr::getPort() const
    {
        return ntohs( m_sockaddr.sin_port );
    }

    PInetAddr&	PInetAddr::operator = (const PInetAddr& right)
    {
		if(&right != this)
		{
			m_sockaddr				=	right.m_sockaddr;
			//m_socklen				=	right.m_socklen;
		}
        return *this;
    }

	B8 PInetAddr::operator < (const PInetAddr& right) const 
	{
		if (this->getAddr() < right.getAddr()) return true;
        else if (this->getAddr() > right.getAddr()) return false;

        if (this->getPort() < right.getPort()) return true;
        else return false;
	}
	B8 PInetAddr::operator == (const PInetAddr& right) const 
	{
		 return this->getAddr() == right.getAddr() && this->getPort() == right.getPort();
	}

    //B8 operator < (const PInetAddr& left, const PInetAddr& right)
    //{
    //    if (left.getAddr() < right.getAddr()) return true;
    //    else if (left.getAddr() > right.getAddr()) return false;

    //    if (left.getPort() < right.getPort()) return true;
    //    else return false;
    //}


    //B8 operator == (const PInetAddr& left, const PInetAddr& right)
    //{
    //    return left.getAddr() == right.getAddr() && left.getPort() == right.getPort();
    //}

    std::string PInetAddr::toString() const
    {
        return PUtils::addr2String(getAddr(),getPort());
    }



    U32	PInetAddr::getLength()const
    {
        return sizeof(U32) + sizeof(U16) ;
    }

    U32		PInetAddr::serialize( S8 *  buf,  S32 offset ) const
    {
        U32 cur_pos = 0;
        S8 * buff = buf + offset;

        writeU32(buff+cur_pos, this->getAddr());
        cur_pos += sizeof(U32);

        writeU16(buff+cur_pos, this->getPort());
        cur_pos += sizeof(U16);

        return cur_pos;
    }

    U32		PInetAddr::deserialize( const S8 * buf, S32 offset )
    {
        U32 cur_pos = 0;

        const S8 * buff = buf + offset;

        U32 ip = this->readU32( buff+ cur_pos );
        cur_pos += sizeof(U32 );

        U16 port  = this->readU16( buff+ cur_pos );
        cur_pos += sizeof(U16 );

        this->set(ip,port);

        return cur_pos;
    }

}

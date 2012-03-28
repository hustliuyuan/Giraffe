// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef INETADDR_H
#define INETADDR_H


#include "common/ISerializble.h"
#include <string>


//本文件中出现的U32地址和U16端口都是主机字节序
//2008-6-30 kimi修改
namespace Network
{
    class NETWORK_EXPORT PInetAddr : public ISerializble
    {
    public:


        PInetAddr( const S8 * pHost = NULL, U16 port = 0 );
		PInetAddr( U32 addr, U16 port );

		void						set( const sockaddr_in& addr);
        void						set( const S8 * pHost = NULL, U16 port = 0 );
        void						set( U32 addr, U16 port );

		PInetAddr&					operator = (const PInetAddr& right);
		B8 operator < (const PInetAddr& right) const ;
		B8 operator == (const PInetAddr& right) const ;

		virtual U32					getLength()const;
        virtual U32					serialize( S8 *  buf,  S32 offset ) const ;
        virtual U32					deserialize( const S8 * buf, S32 offset ) ;


        U32							getAddr()		const;
        U16							getPort()		const;
        const S8*					getIPAddr()		const;
        std::string					toString()      const;

		sockaddr_in&				getSocketAddr(){ return m_sockaddr;}
		sockaddr_in					getSocketAddr()const{ return m_sockaddr;}
    private:
        // Internet Address.
        sockaddr_in					m_sockaddr;
        //S32						    m_socklen;
    };


	//B8 operator < (const PInetAddr& left, const PInetAddr& right);
	//B8 operator == (const PInetAddr& left, const PInetAddr& right);


}

#endif


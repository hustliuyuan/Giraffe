/************************************************************************/
/*tuxuping,kimi*/
/*2008-11-25*/
/************************************************************************/

/*
revised by kimi 2008-12-03
UDPPACKET_MAXSIZE, TCPPACKET_MAXSIZE
packet length check in 'serialize' and 'deserialize'
*/

// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//// base class of packet, is inherited by child class..

#ifndef PACKET_BASE_H
#define PACKET_BASE_H

#include "PPacketDefinition.h"
#include "common/ISerializble.h"
#ifndef SIMPLE_NETWORK_MODE
#include "network/PSocketType.h"
#include "network/PSocketTypeFactory.h"
#endif




/**包的基类
*/
class NETWORK_EXPORT PPacketBase : protected ISerializble
{
protected:
	PPacketBase(U8 type, U8 subtype , S16 version = CURRENT_PKT_VERSION);

public:

#ifndef SIMPLE_NETWORK_MODE
	typedef Network::PSocketType  PSocketType;
#endif

	virtual ~PPacketBase();

	U8								getType()const ;
	U8								getSubType()const ;
	S16								getVersion()const ;

// simple mode means there is only one socket type: MY_TYPE.
#ifndef SIMPLE_NETWORK_MODE
	const U32	getSocketType() const ;
	// added by tuxuping 2008-7-23
	void	setSocketType(U32 socket_type);
#endif

public:

//protected:
	// 字符串显示,用于记录日志
	// this function is invoked inside of the network.dll
	// can not be invoked outside of the dll.
	virtual String					toString() const ;
public:
	
	// this function is invoked outside of the network.dll.
	// added by tuxuping 2009-6-23
	virtual U32						toString(S8 * s )const ;

	// return the length , which is the size when  it is serialized.
	// in this implementation, we compute the length when the packet is created.
	// it is also can be computed on the fly when this function is invoked.
	virtual U32						getLength( ) const;
	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;
	virtual U32						deserialize( const S8 * buf, S32 offset ) ;

protected:
	void							setType(U8 x);
	void							setSubType(U8 x);
	void							setVersion(S16 x);

protected:

#ifndef SIMPLE_NETWORK_MODE
	 U32						    m_iSocketType;
#endif


	U8								m_cType;//类型
	U8								m_cSubType;//子类型
	S16								m_sVersion;//版本

};

#endif /*PACKET_BASE_H*/


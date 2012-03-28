// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef __NAT_REQ_PKT__
#define __NAT_REQ_PKT__


#include "packet/PPacketBase.h"
#include "network/PInetAddr.h"
#include "packet/PPacketDefinition.h"


class PNATReqPkt : public PPacketBase
{
public:
	PNATReqPkt();


	typedef Network::PInetAddr PInetAddr;

	PNATReqPkt( const PInetAddr & selfaddr, const PInetAddr & destaddr );


	virtual U32			getLength( ) const;
	virtual U32			serialize( S8 *  buf,  S32 offset ) const ;
	virtual U32			deserialize( const S8 * buf, S32 offset ) ;

	const PInetAddr	&		getSelfAddr() const ;
	const PInetAddr	&		getDestAddr() const ;

	virtual String      toString() const ;
	virtual U32      toString(S8 * str) const;

private:
	 //U32                  m_selfGlobalIP;
	 //U16                  m_selfGlobalPort;
	 //U32                  m_destGlobalIP;
	 //U16                  m_destGlobalPort;
	PInetAddr               m_oSelfAddr;
	PInetAddr               m_oDestAddr;
};

#endif

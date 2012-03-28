// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef HHHH_PNATREQPKT_HHHHH
#define HHHH_PNATREQPKT_HHHHH


#include "packet/PPacketBase.h"
#include "network/PInetAddr.h"

class PNATNotifyPkt : public PPacketBase
{
public:

	typedef Network::PInetAddr  PInetAddr;

	PNATNotifyPkt();

	virtual U32			getLength( ) const;
	virtual U32			serialize( S8 *  buf,  S32 offset ) const ;
	virtual U32			deserialize( const S8 * buf, S32 offset ) ;

	const PInetAddr&	getRequiredAddr() const ;
	 const void  setRequireAddr(PInetAddr oRequiredAddr) ;

	virtual String      toString() const ;
	virtual U32			toString(S8 * str) const;

private:
	//U32                  m_requriedGlobalIP;
	//U16                  m_requiredGlobalPort;
	PInetAddr			   m_oRequiredAddr;

};
#endif

// Billion Network Tech Software Inc.
// project:Network 
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef __PINTERNAL_NO_DELETE__
#define __PINTERNAL_NO_DELETE__

#include "packet/PPacketBase.h"

class NETWORK_EXPORT PInternalNoDeletePkt :
	public PPacketBase
{
	enum
	{
		// other packet is followed.
		NOT_LAST_PKT = 1 ,
		// the user himself want to quit the program .
		UNKNOWN =2,
	};
public:

	PInternalNoDeletePkt();

	PInternalNoDeletePkt(S32 reseaon );

	~PInternalNoDeletePkt(void);
	
	virtual U32			getLength( ) const;

	static const PInternalNoDeletePkt * getInstance() ;

	virtual String			toString() const ;

	virtual U32			toString(S8 * s )const ;

	virtual U32			serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32			deserialize( const S8 * buf, S32 offset ) ;	

	S32					getReseaon() const;
	
private:

	S32					m_iReaseaon;

};


#endif //__PINTERNAL_NO_DELETE__

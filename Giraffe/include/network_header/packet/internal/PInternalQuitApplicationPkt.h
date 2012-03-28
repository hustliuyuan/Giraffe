// Billion Network Tech Software Inc.
// project:Network 
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef __PINTERNAL_QUITAPPLICATION__
#define __PINTERNAL_QUITAPPLICATION__

#include "packet/PPacketBase.h"

class NETWORK_EXPORT PInternalQuitApplicationPkt :
	public PPacketBase
{
	enum
	{
		// the user is crowd down since another same user login on other terminal.
		RESEAON_CROWD = 1 ,
		// the user himself want to quit the program .
		RESEAON_USER_BEHAVIOR =2,
	};
public:

	PInternalQuitApplicationPkt();

	PInternalQuitApplicationPkt(S32 reseaon );

	~PInternalQuitApplicationPkt(void);
	
	virtual U32			getLength( ) const;

	virtual String      toString() const ;

	virtual U32		toString(S8 * s )const ;

	virtual U32			serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32			deserialize( const S8 * buf, S32 offset ) ;	

	S32					getQuitReseaon() const;
	
private:

	// if user A has login, and a user with same name 'A' login, then the first 'A' need 
	// not to send a leavepacket to the ls.
	// this field is useless now. 2008-10-21. tuxuping.
	S32					m_iReaseaon;

};


#endif //__PINTERNAL_QUITAPPLICATION__

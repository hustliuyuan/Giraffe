#ifndef  __GQUIT_APPLICATION_PKT_H__
#define  __GQUIT_APPLICATION_PKT_H__

//#pragma once
#include "Base/PPktDef.h"

class GQuitApplicationPkt : public PPacketBase 
{
public:

	GQuitApplicationPkt(U8 subtype = PMyPktDef::QUIT_APPLICATION);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~GQuitApplicationPkt(void);

};

#endif// __GQUIT_APPLICATION_PKT_H__
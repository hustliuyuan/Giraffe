#ifndef __GCL_PKT_PARSER_H__
#define __GCL_PKT_PARSER_H__

#include "Base/GHeader.h"

class GCLPktParser :
	public IPacketParser
{
public:

	GCLPktParser(void);

	virtual ~GCLPktParser(void);

	virtual const PPacketBase *	 parsePkt(const S8 * inContent, 
		U16 inContentLen, U32 socket_type ) const ;
};
#endif //__GMS_PKT_PARSER_H__

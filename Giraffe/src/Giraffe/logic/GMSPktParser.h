#ifndef __GMS_PKT_PARSER_H__
#define __GMS_PKT_PARSER_H__

class GMSPktParser :
	public IPacketParser
{
public:

	GMSPktParser(void);

	virtual ~GMSPktParser(void);

	virtual const PPacketBase *	 parsePkt(const S8 * inContent, 
		U16 inContentLen, U32 socket_type ) const ;
};
#endif //__GMS_PKT_PARSER_H__

// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef __PACKET_FACTORY__
#define __PACKET_FACTORY__

#include "packet/PPacketBase.h"
#include "network/PSocketTypeFactory.h"
#include <boost/multi_array.hpp>
#include "common/IPacketParser.h"
#include <set>

class NETWORK_EXPORT PPacketFactory
{

private:

	PPacketFactory();
	
public:

	virtual	~PPacketFactory();

	enum{
		Dimentions = 2, 
	};

	//first dimentions is socketype, second is majortype;
	typedef  boost::multi_array< const IPacketParser *, Dimentions >  ParserContainer ;

	// 
	//typedef  std::map< U32 /*socket_type_id*/, IPacketParser *  parser >  ParserContainer ;

	static PPacketFactory & getInstance();	

	const	PPacketBase *	createPacket(const S8 * inContent, U16 inContentLen, U32 socket_type = Network::PSocketTypeFactory::DEFAULT_SOCKET_TYPE) const ;

	bool	registerPktParser( unsigned char  majortype, 
		const IPacketParser *  parser, 
		U32 socktype);

private: 

	 // support 
	//enum{
	//	MAX_SOCKET_TYPE
	//};
	ParserContainer		 m_oParserContainer;
		//

std::set< U8 >		 m_oMajorTypeContainer;

};

#endif // __PACKET_FACTORY__


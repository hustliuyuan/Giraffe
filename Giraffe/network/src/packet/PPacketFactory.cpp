// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "packet/PPacketFactory.h"
#include <set>
//#include "win32.h"

PPacketFactory & PPacketFactory::getInstance()
{
	static PPacketFactory instance ;
	return instance;
}

bool	PPacketFactory::registerPktParser( unsigned char  majortype, 
						  const IPacketParser *  parser, 
						  U32 socket_type)
{

	U8 socktype_id = (U8)( socket_type ) ;

	std::set<U8>::const_iterator it = m_oMajorTypeContainer.find(majortype);
	if( it != m_oMajorTypeContainer.end() )
	{
		LOG_WARN("PPacketFactory","already registered warn1: sockettype_id: " 
			<< socket_type << " majortype: " << (U32) majortype);
		return false;
	}


	if( m_oParserContainer[socktype_id][ majortype ] != NULL ) 
	{
		LOG_WARN("PPacketFactory","already registered warn2: sockettype_id: " 
			<< socket_type << " majortype: " << (U32) majortype);

		return false ;
	}
	
	m_oParserContainer[socktype_id][ majortype ] = parser;	
	m_oMajorTypeContainer.insert(majortype);
	LOG_INFO("PPacketFactory","successfully registered pktparser:" 
		<< socket_type << " majortype: " << (U32) majortype);

	return true;
	
}


const PPacketBase *	PPacketFactory::createPacket(const S8 * inContent, U16 inContentLen, U32 socket_type )const
{
	U8 socktype_id = (U8)( socket_type ) ;
	U8 majortype = 0; // default is 0 
	if( socket_type == Network::PSocketTypeFactory::DEFAULT_SOCKET_TYPE )
	{
	  majortype = (U8)inContent[PACKET_TYPE_OFFSET];
	}

	if( m_oParserContainer[ socktype_id ][ majortype ] == NULL ) 
	{
		LOG_WARN("PPacketFactory","not supported warn2: sockettype_id: " 
			<< socket_type << " majortype: " << (U32) majortype);		
		return NULL; 
	}

	// here majortype is 0, for sockettype other than DEFAULT_SOCKET_TYPE, 
	// we didnot support majortype till now.
	const IPacketParser *  parser = m_oParserContainer[ socktype_id ][ majortype ];
	
	return parser->parsePkt(inContent, inContentLen, socket_type);	

}

PPacketFactory::~PPacketFactory()
{

	m_oMajorTypeContainer.clear();

	for( ParserContainer::const_iterator it = m_oParserContainer.begin();
		it != m_oParserContainer.end(); ++ it ) 
	{	
// TODO: delete this packetparser;
	  LOG_WARN("PPacketFactory","~PPacketFactory not implemented.");	
	  //const IPacketParser *  parser =  *it[0];
	  //delete parser;
	}
	
}


PPacketFactory::PPacketFactory()
:m_oParserContainer(boost::extents[Network::PSocketTypeFactory::MAX_SOCKET_TYPE_NUM]
[PPktDef::MAX_MAJOR_TYPE ])
{
	
//	array   A(boost::extents[x][y][z]);   
//	m_oParserContainer.resize(10);
}


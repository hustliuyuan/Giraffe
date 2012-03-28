#include "Base/GHeader.h"
#include "PGRFWatcherEventPkt.h"

PGRFWatcherEventPkt::PGRFWatcherEventPkt(U32 WatchererEvent, String nodeName, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_iWatchererEvent = WatchererEvent;
	m_sNodeName = nodeName;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFWatcherEventPkt::PGRFWatcherEventPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFWatcherEventPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_iWatchererEvent);
	cur_pos += sizeof(U32);

	cur_pos += writeString(buff + cur_pos, m_sNodeName);

	return cur_pos;

}

U32		PGRFWatcherEventPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_iWatchererEvent);
	cur_pos += sizeof(U32);

	cur_pos += readString(buff + cur_pos, m_sNodeName);

	return cur_pos;
}

U32 PGRFWatcherEventPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32);
	iLen += m_sNodeName.size() + 5;

	return iLen;
}

String	PGRFWatcherEventPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "Watcherer event id: "<< m_iWatchererEvent;
	strstr << " node name :"<< m_sNodeName;

	return strstr.str();

}

U32	PGRFWatcherEventPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFWatcherEventPkt::~PGRFWatcherEventPkt(void)
{

}



// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "common/Checksum.h"
#include "common/Exception.h"
#include "os/PUtils.h"
#include "packet/PPacketBase.h"


#ifndef SIMPLE_NETWORK_MODE
const U32	 PPacketBase::getSocketType() const
{
	return m_iSocketType;
}
// added by tuxuping 2008-7-23
void PPacketBase::setSocketType(U32	 socktype)
{
	this->m_iSocketType = socktype;
}
#endif



PPacketBase::PPacketBase(U8 type, U8 subtype, S16 version)
: m_cType(type), m_cSubType(subtype), m_sVersion(version)
{
#ifndef SIMPLE_NETWORK_MODE //added by tuxuping 2008-7-23
	this->m_iSocketType = Network::PSocketTypeFactory::DEFAULT_SOCKET_TYPE ;
#endif

	// commented by tuxuping 2009-5-29, to support message logic debug system ( meld ) 
	// meld assumes 'version' is a random number. 

#ifdef SUPPORT_MELD
	m_sVersion = (S16)rand();
#endif

}


//////////////////////////////////////////////////////////////////////////

String PPacketBase::toString() const
{
	std::ostringstream ss;
	//std::string s;
	//s.append(" type:");
	//s.append(PPktDef::type2String(this->m_cType,this->m_cSubType));
	//s.append(" v:");
	
	//s.append(itoa(this->m_sVersion);

	ss<<" type:"<<PPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;
	std::string str = ss.str();	

	//char sss[1000]={0};

	//memcpy(sss,str.c_str(),str.size());

	////std::string::const_iterator it = str.begin();
	////std::string str2( it, str.end() );

	//return sss;
	return str;
}

//
U32	PPacketBase::toString(S8 * str)const
{

	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;

//	
//	//std::ostringstream ss;
//
//	//String  str1 = PPacketBase::toString();
//
//	//std::ostringstream ss;
//	////std::string s;
//	////s.append(" type:");
//	////s.append(PPktDef::type2String(this->m_cType,this->m_cSubType));
//	////s.append(" v:");
//
//	////s.append(itoa(this->m_sVersion);
//
//	//ss<<" type:"//<<PPktDef::type2String(this->m_cType,this->m_cSubType)	
//	//	<<" v:"<<this->m_sVersion;
//	//std::string str1 = ss.str();	
//
//	////std::string str1 = ss.str();	
//
//	//U32 len = ( str1.size() < total_len - from_pos ) ? str1.size(): total_len - from_pos -1 ;
//	//memcpy(str+from_pos, str1.c_str(),len );
//	//return str1.size();
//	
	//return 0;
}


PPacketBase::~PPacketBase()
{

}


U8			PPacketBase::getType()const {return m_cType;}

U8			PPacketBase::getSubType()const {return m_cSubType;}

S16			PPacketBase::getVersion()const {return m_sVersion;}

void		PPacketBase::setType( U8 x){m_cType = x;}

void		PPacketBase::setSubType( U8 x){m_cSubType = x;}

void		PPacketBase::setVersion( S16 x){m_sVersion = x;}

U32 PPacketBase::getLength() const
{
	return COMMON_HEAD_LENGTH;
}

U32	PPacketBase::serialize( S8 *  buf,  S32 offset ) const
{
	U32 cur_pos = 0;
	S8 * buff = buf + offset;

	writeU16( buff + cur_pos, (U16)(getLength()) );
	cur_pos += sizeof(U16);

	writeU8(buff+cur_pos, m_cType);
	cur_pos += sizeof(U8);

	writeU8(buff+cur_pos, m_cSubType);
	cur_pos += sizeof(U8);

	writeS16(buff+cur_pos, m_sVersion);
	cur_pos += sizeof(S16);

	/************************************************************************/
	/*注意:*/
	/*校验和字段填0, 等子类将包的所有字段填入后*/
	/*由子类的serialize函数在函数末尾生成并填入checksum*/
	//比如
	//writeU16(buff + PACKET_CHECKSUM_OFFSET, checksum);
	/************************************************************************/
	writeU16(buff+cur_pos, 0);
	cur_pos += sizeof(U16);

	assert(cur_pos == COMMON_HEAD_LENGTH);
	return COMMON_HEAD_LENGTH;
}


U32	PPacketBase::deserialize( const S8 * buf, S32 offset ) {

	const S8 *buff = buf + offset;
	S32 cur_pos = 0;
	U16 pkt_len;
	U16 check_sum;

	readU16( buff + cur_pos, pkt_len);
	cur_pos += sizeof(U16);

	readU8( buff + cur_pos, m_cType);
	cur_pos += sizeof(U8);

	readU8(buff + cur_pos, m_cSubType);
	cur_pos += sizeof(U8);

	readS16 ( buff + cur_pos, m_sVersion);
	cur_pos += sizeof(S16);

	//读取校验和字段
	readU16( buff + cur_pos, check_sum);
	cur_pos += sizeof(U16);


	//检查包长度(按照TCP的来计算)
	if(pkt_len > TCPPACKET_MAXSIZE || pkt_len < COMMON_HEAD_LENGTH)
	{
		std::ostringstream ss;
		ss<<"PPacketBase::deserialize, packet header length out of range : "<<pkt_len;
		throw PException(ss.str());
	}
	//检查包版本
	// modified by tuxuping 2009-5-28, to support message logic debug system ( meld ) 
	// meld assumes 'version' is a random number. 
#ifndef SUPPORT_MELD
	if(CURRENT_PKT_VERSION != m_sVersion)
		throw PException("PPacketBase::deserialize, version error");
#endif


	//检查校验和
	if(!PUtils::verifyCheckSum(buff, pkt_len))
		throw PException("PPacketBase::deserialize, checksum error");



	// added by tuxuping 2008-7-23
	// default is my type; if we support additional socket type, 
	// we must define a default socket type for each packet. 2009-4-24
#ifndef SIMPLE_NETWORK_MODE
	setSocketType( Network::PSocketTypeFactory::DEFAULT_SOCKET_TYPE) ;
#endif

	assert(cur_pos == COMMON_HEAD_LENGTH);
	return COMMON_HEAD_LENGTH;
}
//////////////////////////////////////////////////////////////////////////




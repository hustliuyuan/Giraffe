// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2010-8-28
// all rights reserved.

#include "common/Header.h"
#include "packet/PPacketDefinition.h"
#include "packet/internal/PInternalNoDeletePkt.h"

PInternalNoDeletePkt::PInternalNoDeletePkt()
:PPacketBase(PPktDef::INTERNAL_COMMUNICATION, PPktDef::NO_DELETE)
{
	
}

PInternalNoDeletePkt::PInternalNoDeletePkt(S32 reseaon )
:PPacketBase(PPktDef::INTERNAL_COMMUNICATION, PPktDef::NO_DELETE)
{
	m_iReaseaon  = reseaon;
}

PInternalNoDeletePkt::~PInternalNoDeletePkt(void)
{

}

S32	PInternalNoDeletePkt::getReseaon() const
{
	return m_iReaseaon;
}

const PInternalNoDeletePkt * PInternalNoDeletePkt::getInstance() 
{
	static PInternalNoDeletePkt   no_deletePkt; 
	return  &no_deletePkt;
}

U32		PInternalNoDeletePkt::getLength( ) const 
{
		return  (U32)(  
			        COMMON_HEAD_LENGTH + sizeof(S32) 				 
				  );
}

U32		PInternalNoDeletePkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize( buf, offset );

	S8 * buff = buf + offset;

	writeS32( buff + cur_pos , m_iReaseaon );

	cur_pos += sizeof(S32);

	return cur_pos ;
}



U32		PInternalNoDeletePkt::deserialize( const S8 * buf, S32 offset ) 
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset );

	const S8 * buff = buf + offset;

	m_iReaseaon = readS32( buff+ cur_pos );

	cur_pos += sizeof(S32);

	return cur_pos;

}

String  PInternalNoDeletePkt::toString() const 
{
	std::ostringstream  s  ;
	s<< PPacketBase::toString();
	s<< "reason:"<< m_iReaseaon;
	return s.str();
}

U32 PInternalNoDeletePkt::toString( S8 * str ) const
{
	std::ostringstream strstr;
	strstr<<" type:"<<PPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr<< "reason:"<< m_iReaseaon;

	String str1 = strstr.str();	
	U32 len = U32( str1.length() + 1);
	memcpy(str, str1.c_str(), len );
	
	return len;
}

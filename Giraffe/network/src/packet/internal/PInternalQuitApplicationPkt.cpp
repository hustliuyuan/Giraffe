// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "packet/PPacketDefinition.h"
#include "packet/internal/PInternalQuitApplicationPkt.h"

PInternalQuitApplicationPkt::PInternalQuitApplicationPkt()
:PPacketBase(PPktDef::INTERNAL_COMMUNICATION, PPktDef::QUIT_APPLICATION)
{
	
}

PInternalQuitApplicationPkt::PInternalQuitApplicationPkt(S32 reseaon )
:PPacketBase(PPktDef::INTERNAL_COMMUNICATION, PPktDef::QUIT_APPLICATION)
{
	m_iReaseaon  = reseaon;
}

PInternalQuitApplicationPkt::~PInternalQuitApplicationPkt(void)
{

}

S32	PInternalQuitApplicationPkt::getQuitReseaon() const
{
	return m_iReaseaon;
}

U32		PInternalQuitApplicationPkt::getLength( ) const 
{
		return  (U32)(  
			        COMMON_HEAD_LENGTH + sizeof(S32) 				 
				  );
}

U32		PInternalQuitApplicationPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize( buf, offset );

	S8 * buff = buf + offset;

	writeS32( buff + cur_pos , m_iReaseaon );

	cur_pos += sizeof(S32);

	return cur_pos ;
}



U32		PInternalQuitApplicationPkt::deserialize( const S8 * buf, S32 offset ) 
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset );

	const S8 * buff = buf + offset;

	m_iReaseaon = readS32( buff+ cur_pos );

	cur_pos += sizeof(S32);

	return cur_pos;

}

String  PInternalQuitApplicationPkt::toString() const 
{
	std::ostringstream  s  ;
	s<< PPacketBase::toString();
	s<< "reason:"<< m_iReaseaon;
	return s.str();
}

U32 PInternalQuitApplicationPkt::toString( S8 * str ) const
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

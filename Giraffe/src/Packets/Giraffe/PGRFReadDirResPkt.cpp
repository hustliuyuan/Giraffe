#include "Base/GHeader.h"
#include "PGRFReadDirResPkt.h"

PGRFReadDirResPkt::PGRFReadDirResPkt(U32 e, U32 entryCount, vector<String>& entryNameList,U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{

	m_event = e;
	m_iEntryCount = entryCount;
	m_entryNameList = entryNameList;

	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFReadDirResPkt::PGRFReadDirResPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFReadDirResPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iEntryCount);
	cur_pos += sizeof(U32);

	vector<string>::size_type it = 0;

	for(unsigned int i = 0; i < m_iEntryCount; i ++, it ++)
	{
		cur_pos += writeString(buff + cur_pos, m_entryNameList[it]);
	}

	//cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFReadDirResPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iEntryCount);
	cur_pos += sizeof(U32);


	for(unsigned int i = 0; i < m_iEntryCount; i ++)
	{
		String entry_tmp;
		cur_pos += readString(buff + cur_pos, entry_tmp);
		m_entryNameList.push_back(entry_tmp);
	}
	//cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFReadDirResPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32);

	iLen += sizeof(U32);

	vector<string>::size_type it = 0;

	for(unsigned int i = 0; i < m_iEntryCount; i ++, it ++)
	{
		iLen += m_entryNameList[it].size() + 5;
	}
	//iLen = iLen + m_String.size() + 5;

	return iLen;

}

String	PGRFReadDirResPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	//strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFReadDirResPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFReadDirResPkt::~PGRFReadDirResPkt(void)
{

}



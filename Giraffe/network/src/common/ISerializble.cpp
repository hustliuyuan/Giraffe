// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "common/ISerializble.h"
#include "common/Exception.h"
#include <sstream>
//#include "win32.h"

U32 getStringLength(const String& s)
{
	return 5 + (U32)s.size();
}



ISerializble::~ISerializble(void)
{

}




void ISerializble::writeU8(S8 * buff, U8 value)
{
	*(U8*)(buff) = value;
}


void ISerializble::writeU16(S8 * buff, U16 value)
{
	*(U16*)(buff) = value;
}


void ISerializble::writeU32(S8 * buff, U32 value)
{
	*(U32*)(buff) = value;
}

void ISerializble::writeS8(S8 * buff, S8 value)
{
	*(S8*)(buff) = value;
}
void ISerializble::writeS16(S8 * buff, S16 value)
{
	*(S16*)(buff) = value;
}
void ISerializble::writeS32(S8 * buff, S32 value)
{
	*(S32*)(buff) = value;
}




void ISerializble::readU8(const S8 * buff ,U8& value)
{
	value = *(U8*)buff;
}

void ISerializble::readU16(const S8 * buff ,U16& value)
{
	value = *(U16*)(buff);
}

void ISerializble::readU32(const S8 * buff ,U32& value)
{
	value = *(U32*)(buff);
}


void ISerializble::readS8(const S8 * buff ,S8& value)
{
	value = *(S8*)(buff);
}
void ISerializble::readS16(const S8 * buff ,S16& value)
{
	value = *(S16*)(buff);
}
void ISerializble::readS32(const S8 * buff ,S32& value)
{
	value = *(S32*)(buff);
}



U8 ISerializble::readU8(const S8 * buff)
{
	return *(U8*)buff;
}


U16 ISerializble::readU16(const S8 * buff)
{
	return *(U16*)(buff);
}



U32 ISerializble::readU32(const S8 * buff)
{
	return *(U32*)(buff);
}

S8 ISerializble::readS8(const S8 * buff )
{
	return *(S8*)(buff);
}


S16 ISerializble::readS16(const S8 * buff )
{
	return *(S16*)(buff);
}


S32 ISerializble::readS32(const S8 * buff )
{
	return *(S32*)(buff);
}



void ISerializble::writeU64(S8 * buff, U64 value)
{
	*(U64*)(buff) = value;
}
void ISerializble::writeS64(S8 * buff, S64 value)
{
	*(S64*)(buff) = value;
}
U64 ISerializble::readU64(const S8 * buff)
{
	return *(U64*)(buff);
}
S64 ISerializble::readS64(const S8 * buff)
{
	return *(S64*)(buff);
}
void ISerializble::readU64(const S8 * buff, U64& value)
{
	value = *(U64*)(buff);
}
void ISerializble::readS64(const S8 * buff, S64& value)
{
	value = *(S64*)(buff);
}






void ISerializble::writeBytes(S8 * buff, const char* data, std::size_t len)
{
	for(std::size_t i=0; i<len; i++)
		buff[i] = data[i];
}


char* ISerializble::readBytes(const S8 * buff, char* data, std::size_t len)
{
	for(std::size_t i=0; i<len; i++)
		data[i] = buff[i];
	return data;
}


U32 ISerializble::writeString(S8 * buff, const String& str)
{
	U32 len = (U32)str.size();
	if(len > STRING_MAXSIZE)
	{
		std::ostringstream ss;
		ss<<"ISerializble::writeString string length exceeds STRING_MAXSIZE : "
			<<len;
		throw PException(ss.str());
	}


	writeU32(buff, len);
	writeBytes(buff + sizeof(U32), str.data(), len);
	writeU8(buff + sizeof(U32) + len, 0);
	return len + sizeof(U32) + 1;
}


String ISerializble::readString(const S8 * buff)
{
	U32 len = readU32(buff);
	if(len > STRING_MAXSIZE)
	{
		std::ostringstream ss;
		ss<<"ISerializble::writeString string length exceeds STRING_MAXSIZE : "
			<<len;
		throw PException(ss.str());
	}


	String::value_type* pBuffer = new String::value_type[len];
	readBytes(buff + sizeof(U32), pBuffer, len);
	readU8(buff + sizeof(U32) + len);
	String ret( (char*)pBuffer, len);
	delete[] pBuffer;
	return ret;
}


U32 ISerializble::readString(const S8 * buff, String& str)
{
	U32 len = readU32(buff);
	if(len > STRING_MAXSIZE)
	{
		std::ostringstream ss;
		ss<<"ISerializble::writeString string length exceeds STRING_MAXSIZE : "
			<<len;
		throw PException(ss.str());
	}


	String::value_type* pBuffer = new String::value_type[len];
	readBytes(buff + sizeof(U32), pBuffer, len);
	readU8(buff + sizeof(U32) + len);
	str.assign( (char*)pBuffer, len);
	delete[] pBuffer;
	return len + sizeof(U32) + 1;
}




U32 ISerializble::getLength() const
{
	return 0;
}

//Added by Wu
void ISerializble::writeB8(S8 * buff, B8 value)
{
	*(B8*)(buff) = value;
}

B8 ISerializble::readB8(const S8 * buff )
{
	return *(B8*)(buff);
}

U32 ISerializble::getStringLength(const String& s)const
{return ::getStringLength(s);}
// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef  HEAD_H
#define  HEAD_H 

#define _ITERATOR_DEBUG_LEVEL 0
/************************************************************************/
/*Win32*/
/************************************************************************/
#if defined WIN32 || defined _WIN32
//#define _WIN32_WINNT 0x0501
#ifdef VLD
#include "vld.h"
#endif //VLD

#ifdef CUSTOM_FD_SETSIZE
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#define FD_SETSIZE 256
#endif//FD_SETSIZE
#endif//CUSTOM_FD_SETSIZE

// added by tuxuping 2009-6-4, to support meld: a distributed debug system.
#ifdef MELD_SUPPORT
#include "meld.h"
#endif/*MELD_SUPPORT*/

#include <Winsock2.h>


#else
#ifndef NETWORK_EXPORT
#define NETWORK_EXPORT
#endif
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>
#define  SOCKET_ERROR (-1)
typedef long            DWORD;
typedef int             SOCKET;
typedef unsigned int 	UINT;
typedef void* 			LPVOID;
#endif//WIN32
#include "win32.h"

typedef unsigned char		U8;
typedef char				S8;
typedef unsigned short		U16;
typedef signed short		S16;
typedef unsigned int		U32;
typedef signed int			S32;
#ifdef _MSC_VER
typedef __int64				S64;
typedef __int64				U64;
#elif __GNUC__
typedef long long			S64;
typedef S64					U64;
#endif
typedef float				F32;
typedef double				F64;

/************************************************************************/
/*C++*/
/************************************************************************/
#ifdef __cplusplus
#include <iostream>
#include <string>
#include "log.h"
#include <assert.h>
typedef bool				B8;
typedef std::string			String;
#endif /*__cplusplus*/

///************************************************************************/
///*��÷ŵ���Ӧ���ļ�����ȥ����Ϊ��Щ���岻��ȫ�ֹ����*/
///************************************************************************/
//enum
//{
//	MAX_USER_PWD_LENGTH = 32,  // including the '\0', tuxuping added comments. 2008-6-5.
//	USER_PWD_CRYPT_LENGTH = 32, //
//	MAX_USERID_LENGTH = 16 ,  // including the '\0', tuxuping added comments. 2008-5-28.
//	MAX_TEAMID_LENGTH = 33 ,  // including the '\0', tuxuping added comments. 2008-5-28.
//	MAX_CHAT_MSG_LENGTH = 1024, // including the '\0',tuxuping added comments. 2008-5-28.
//};

//typedef S32					RoomIDType;
//#include <boost/array.hpp>
//#include <set>
//typedef boost::array<char, USER_PWD_CRYPT_LENGTH>	EnCryptPwdType;
//typedef std::set<String>		IdsSet;


#endif



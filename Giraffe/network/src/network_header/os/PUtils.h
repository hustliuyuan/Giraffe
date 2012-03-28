// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


//revised by kimi 2008-12-03
//makeCheckSum, verifyCheckSum


#ifndef		UTILS_H
#define		UTILS_H

//#include "common/MD5.h"
#include <time.h>
#include <sys/timeb.h>
#include <string>
#include <set>

//
////	lingle add 2008-7-10

//
//
////added by kimi 2008-11-19
//void				pauseConsole();
////int					delFile(const char* fn);
////added by kimi 2008-11-24
//size_t				getFileLength(const char* fn);
//bool				getRandomBool();
//unsigned char		getRandomUChar();
//char				getRandomChar();
//unsigned int		getRandomUInt();
//int					getRandomInt();
//float				getRandomFloat();
//unsigned int		getRandomUIntRange(unsigned int lo, unsigned int hi);
//int					getRandomIntRange(int lo, int hi);
//void				shuffleBuffer(char *const buf, size_t len);
//void				fillRandomBuffer(char *const buf, size_t len);



class NETWORK_EXPORT PUtils
{
public:

		static S64 	milliSeconds();

		static S32	seconds() ;

		// seconds + 8*60*60. consider the local timezone, added by tuxuping. 2008-10-28
		static S32	localSeconds() ;

		// seconds + 8*60*60*1000. consider the local timezone, added by tuxuping. 2008-10-28
		static S64	localMilliSeconds() ;


		static void				initSocketEnv( ) ;

		static void				uninitSocketEnv( ) ;

		static void				getCurrentDirectory( std::string  & path );

		static std::string		addr2String(const struct sockaddr_in * from);

		// you make sure of enough places prepared in 'to'
		// not tested yet
		// added by tuxuping 2009-7-2
		static void				addr2String(const struct sockaddr_in * from, S8* to);

		static U32				addr2Long( const S8 *  addr );

		//static U32			GetFileID( U32 ip, SInt16 port, std::string file );

		static std::string		time2String( U32 time);

		// added by tuxuping 2009-7-2
		static void				time2String( U32 time, S8 * to, U32 sizeof_to);

		static S32				atoi( const S8 * int_str);

		static  S8 *			itoa ( S32 value, S8* str, S32 radix);

		//TODO: added by tuxuping, 2008-7-25
		static U32				str2Time( const std::string & str);

		static void				setZero( S8 * buf, S32 len) ;

		static void				sleep( U32 millseconds ) ;

		static S32			    getLastErrorCode();

		static void				exitProcess( U32  excode );

		// removed by tuxuping
		//static std::string		long2Addr( U32	ipaddr);

		// added by tuxuping 2009-7-2
		static S8*				long2Addr( U32	ipaddr);

		static std::string      addr2String( U32  ipaddr, U16 inport);
		// added by tuxuping 2009-7-2
		static void			    addr2String( U32  ipaddr, U16 inport, S8* to_str);

		static  std::string     readConfigLine(  std::ifstream&  instream );

		static S32				getFileId( std::string  filename ) ;

		// if found return pos, the first occurrance. or return -1.
		S32						searchStr( const S8* str, U32	size, U32 offset, const S8 * substr,U32  substrLen );

		static U32				getRandomWithin(   U32  to , U32	from = 0 ) ;

		// produce <count> of numbers , each number is no less than <from> and no bigger than <to>.
		static void				getRandomSequence(  U32 to, U32 from, U32 count, std::set< S32 >&  container );

		/************************************************************************/
		/*设置当前工作路径为可执行文件所在的路径*/
		/************************************************************************/
		static B8				setDefaultPath2ModulePath();
		//计算校验和
		static U16				makeCheckSum(const char *data, U16 len);
		//检查校验和。true: 通过，false: 失败。
		static bool				verifyCheckSum(const char *data, U16 len);

		//added by kimi 2008-11-25
		static void				initLog4cplus();
		//added by kimi 2008-12-1
		static void				initLog4cplus(const char* filename);

		//String							toString(const EnCryptPwdType& pwd);
		//EnCryptPwdType					toEnCryptPwdType(const String& strPwd);
		//	lingle add 2008-7-23
		//EnCryptPwdType					encrypt(const String& strInPwd);

};

#endif


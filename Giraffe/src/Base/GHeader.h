#ifndef __GIRAFFE_HEADER_H__
#define __GIRAFFE_HEADER_H__

//define the packet max size
enum{
	TCP_TRANSFER_MAX_SIZE = 5120,//5K
	UDP_TRANSFER_MAX_SIZE = 1020,// 1K
};

//define the max socket connection
#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
//#define FD_SETSIZE      10240
#endif /* FD_SETSIZE */

#ifdef WIN32

#include <tchar.h>
#include <Winsock2.h>
#include <windows.h>
#include <process.h>
#include <Winbase.h>
#include <time.h>

#else				//UNIX

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <error.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <assert.h>
#define UNREFERENCED_PARAMETER(P)  (P)
#endif

#include <set>
#include <map>
#include <deque>
#include <vector>
#include <string>
#include <iostream>


///////////////// Source Types //////////////////////////////
#define UDP_LISTEN_INTERVAL_MILLISECONDS			1
#define TCP_SELECT_TIME_OUT_MICROSECONDS			10000
#define TCP_SELECT_INTERVAL_MILLISECONDS			1

#define ABS(x) (((x) > 0) ? (x) : (-(x)))

#define FOURCC(a, b, c, d)	(UInt32)((((UInt8)a) << 24) + (((UInt8)b) << 16) + (((UInt8)c) << 8) + ((UInt8)d))
#define TWOCC(a, b)			(UInt16)((((UInt8)a) << 8) + ((UInt8)b))

#include "include/network_header/network.h"
#ifndef NETWORK_EXPORT
#define NETWORK_EXPORT __declspec(dllimport)
#endif

#endif // __GIRAFFE_HEADER_H__





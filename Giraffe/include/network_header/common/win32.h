
#ifndef NETWORK_CONFIG_WIN32_HEADER_
#define NETWORK_CONFIG_WIN32_HEADER_

#ifdef _WIN32

/* Define if you have the ftime function.  */
//#define LOG4CPLUS_HAVE_FTIME 1

#ifdef NETWORK_STATIC
#define NETWORK_EXPORT
#else

#ifdef NETWORK_BUILD_DLL
#define NETWORK_EXPORT __declspec(dllexport)
#else
#define NETWORK_EXPORT __declspec(dllimport)
#endif

#endif

#ifndef NETWORK_SINGLE_THREADED
#define NETWORK_USE_WIN32_THREADS
#endif

#if defined(_MSC_VER)
// Warning about: identifier was truncated to '255' characters in the debug information
#pragma warning( disable : 4786 )
// Warning about: <type1> needs to have dll-interface to be used by clients of class <type2>
#pragma warning( disable : 4251 )

#if _MSC_VER >= 1400
#define NETWORK_WORKING_LOCALE
#endif

#endif


#endif // _WIN32
#endif // NETWORK_CONFIG_WIN32_HEADER_



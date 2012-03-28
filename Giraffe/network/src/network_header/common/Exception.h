// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

/************************************************************************/
/*kimi*/
/*2008-11-25*/
/************************************************************************/
#ifndef EXCEPTION
#define EXCEPTION

#include <exception>
#include <string>

/**
“Ï≥£
*/
class PException
	: public std::exception
{
public:
	PException(const String& w)
		:what_(w){}
	~PException() throw()
	{}
	virtual const char* what()const throw(){return what_.c_str();}
private:
	std::string what_;
};


#endif//EXCEPTION


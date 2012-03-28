// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


# ifndef __PUBLIC_CONFIG_H__
# define __PUBLIC_CONFIG_H__

#include "common/Header.h"
#include "config/PIniReader.h"

class NETWORK_EXPORT PPublicConfig :
	public PIniReader
{
public:

	PPublicConfig(const std::string & config_file);

	virtual ~PPublicConfig(void);

};

#endif  // __PUBLIC_CONFIG_H__

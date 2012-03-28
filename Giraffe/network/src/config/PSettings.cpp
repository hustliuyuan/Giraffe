// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "config/PSettings.h"
#ifdef __linux__
#include <sys/ioctl.h>
#include <net/if.h>
#endif




bool   PSettings::loadEnv(  PIniReader & inireader )
{

	//this->m_Configs = &inireader;

	if( ! inireader.readOneConfig() )
	{
		LOG_ERROR("PSettings","read config error:");
		return false;
	}

	for ( Configs::const_iterator it = inireader.m_mapConfig.begin();
		it != inireader.m_mapConfig.end(); ++it ) {

			if( this->m_mapConfig.find(it->first) != this->m_mapConfig.end() ) {

				LOG_WARN("PSettings","loadEnv():"<< it->first <<" was already configured" );
			}
			this->m_mapConfig.insert(*it);
	}

	//this->m_ConfigLeft

	//  todo: verify the configs ;
	LOG_DEBUG("PSettings", "loadEnv() successed. ");

	// added by tuxuping , 2008-7-31.


	// here we can set two time. the performance is low.
	// TODO: will be improved. tuxuping , 2008-5-28.
	setup();

	return true;
}

PConfigValue  PSettings::getConfig( const std::string & key) const
{
	Configs::const_iterator  it ;
	it =  m_mapConfig.find( key ) ;
	if( it == m_mapConfig.end())
	{
		return PConfigValue();
	}
	return  it->second;
}

S32  PSettings::setConfig(const char *name, PConfigValue::configValueType_t  type,
						  const char * value)
{
	//PConfigValue::configValueType_t  type2 = m_ConfigLeft[ name ]  ;

	Configs::const_iterator  it2 = m_mapConfig.find( name ) ;

	if ( it2 != m_mapConfig.end() )
	{
		//return -1;
		LOG_WARN("PSettings", "setConfig() : name[" << name << "] = "
			<< type << " all ready exist.");
	}
	m_ConfigLeft [ name ] = type;

	PConfigValue v( type, value) ;

	m_mapConfig[ name ] =  v;

	LOG_DEBUG("PSettings", "setConfig() : name[" << name << "] = " << value);
	return 0;
}


PSettings::PSettings()
{

}

void PSettings::setup( ){

	// process requests of media data.

	PUtils::initSocketEnv();

	Connection.timeoutselectsecond = 0;
	Connection.timeoutselectmicroseconds = 1000;  // microseconds
#ifdef __linux__
	char ip[16]={0};
	getlocaip(ip);
#else
	hostent* thisHost = gethostbyname("");
	const char* ip = inet_ntoa(*(struct in_addr *)(*(thisHost->h_addr_list)));
#endif

	this->setConfig("local-ip",PConfigValue::STRING_TYPE,ip);
	U32 temp_ip = PUtils::addr2Long(ip);
	U16 local_udp_server_port_base = 
		this->getConfig("local-udp-server-port-base").m_value.intvalue;

	// this line is executed at client environment.
	if( local_udp_server_port_base )
	{
		U16 local_udp_server_port = local_udp_server_port_base + (temp_ip & 0xff);
		char  port_str[16]={0};
		PUtils::itoa(local_udp_server_port, port_str, 10);
		this->setConfig("local-udp-server-port",PConfigValue::INT_TYPE,port_str);
	}	

	//PUtils::uninitSocketEnv();

}
//something wrong in this function 2012-2-23
#ifdef __linux__
int PSettings::getlocaip(char *ip) {
/*
	struct sockaddr_in host;
	bzero(&host,sizeof(struct sockaddr_in));
	host.sin_family=AF_INET;
	host.sin_addr.s_addr=htonl(INADDR_ANY);
	
	strcpy(ip, inet_ntoa(host.sin_addr));
	return 1;
*/
	int sockfd;
//	if (-1 == (sockfd = socket(PF_INET, SOCK_STREAM, 0)))
	if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
	{
		perror("socket");
		return -1;
	}
	struct ifreq req;
	struct sockaddr_in *host;
	bzero(&req, sizeof(struct ifreq));
	strcpy(req.ifr_name, "eth0");
	ioctl(sockfd, SIOCGIFADDR, &req);
	host = (struct sockaddr_in*) &req.ifr_addr;
	//added by linhaohong on 2012-2-24 for test
	std::cout << "inet_addr" << host->sin_addr.s_addr << std::endl;	
	if(host->sin_addr.s_addr == 0)
	{
		bzero(&req, sizeof(struct ifreq));
		strcpy(req.ifr_name, "eth1");
		ioctl(sockfd, SIOCGIFADDR, &req);
		host = (struct sockaddr_in*) &req.ifr_addr;
	}
	
	if(host->sin_addr.s_addr == 0)
	{
		bzero(&req, sizeof(struct ifreq));
		strcpy(req.ifr_name, "wlan0");
		ioctl(sockfd, SIOCGIFADDR, &req);
		host = (struct sockaddr_in*) &req.ifr_addr;
	}
	strcpy(ip, inet_ntoa(host->sin_addr));
	close(sockfd);
	return 1;
}
#endif
PSettings::~PSettings()
{
	this->m_ConfigLeft.clear(); // added by tuxuping. 2008-7-22
	this->m_mapConfig.clear();  // added by tuxuping. 2008-7-22
//	LOG_DEBUG("PSettings","~destructor:"); // added by tuxuping. 2008-7-25, noted by lingle 2008-10-7
}



PSettings& PSettings::getInstance()
{
	static PSettings  instance;
	return instance;
}

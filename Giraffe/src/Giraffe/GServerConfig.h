# ifndef __GIRAFFE_SERVER_CONFIG_H__
# define __GIRAFFE_SERVER_CONFIG_H__

class GServerConfig :
	public PIniReader
{
public:

	GServerConfig(const std::string & config_file);

	virtual ~GServerConfig(void);

};

#endif//__GIRAFFE_SERVER_CONFIG_H__

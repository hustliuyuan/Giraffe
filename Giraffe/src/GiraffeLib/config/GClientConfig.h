# ifndef __GCL_CONFIG_H__
# define __GCL_CONFIG_H__

class GClientConfig :
	public PIniReader
{
public:

	GClientConfig(const std::string & config_file);

	virtual ~GClientConfig(void);

};

#endif//__GCL_CONFIG_H__

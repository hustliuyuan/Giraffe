#ifndef __GCL_COMM_LOGGER__H__
#define __GCL_COMM_LOGGER__H__

class GCLCommLogger
{
public:
static void startLogInfrastrue(const char *path, S32 log_level);
static void setCommConfig();
};

#endif //__GCL_COMM_LOGGER__H__


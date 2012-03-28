#include "common/Header.h"
#include "multithread/job/IJob.h"

IJob::IJob(U32 jobId, U8 subType, U8 type)
{
	m_cType = type;
	m_cSubType = subType;
	m_iJobId = jobId;
}

IJob::~IJob()
{

}

U8 IJob::getJobType()const
{
	return m_cType;
}

U8 IJob::getJobSubType()const
{
	return m_cSubType;
}

U32 IJob::getJobId()const 
{
	return m_iJobId;
}
//
//String	IJob::toString() const
//{
//	std::ostringstream ss;
//
//	ss<<" type:"<<type2String(this->m_cType,this->m_cSubType)
//		<<" job id:"<<m_iJobId;
//
//	return ss.str();
//}

U32	IJob::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}
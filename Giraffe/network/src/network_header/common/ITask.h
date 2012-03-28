// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef   I_TASK_H
#define   I_TASK_H 

#include <string>

class NETWORK_EXPORT ITask{

public:

	virtual void onRun() =0;

	virtual std::string getName() const=0;

};

#endif
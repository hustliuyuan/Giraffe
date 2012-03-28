#ifndef __GIRAFFE_WATCHER_TYPE_H__
#define __GIRAFFE_WATCHER_TYPE_H__

namespace WatcherEventType
{

	enum 
	{
		NODE_ADD,
		NODE_DEL,
		NODE_MODIFIED,
		NODE_EXITS,
		LOCK_GENERATED,
		LOCK_RELEASED,
		//other watching types are remaining!
	};

};

#endif //__GIRAFFE_WATCHER_TYPE_H__


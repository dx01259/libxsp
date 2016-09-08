
#ifndef THREAD_XMUTEX_H_
#define THREAD_XMUTEX_H_

#include<pthread.h>

struct xsp_mutex
{
	bool volatile blocked;
	pthread_mutex_t mutex_t;
};

/**
 * mutex_init -- initialize the mutex object
 * @mt: the structure of xsp_mutex to create
 * 		mutex object
 */
inline void mutex_init(struct xsp_mutex &mt)
{
	mt.blocked = false;
	pthread_mutex_init(&mt.mutex_t, NULL);
}

/**
 * mutex_destroy -- destroy the object mutex
 * @mt: the structure of xsp_mutex to destroy
 * 		mutex object
 */
inline void mutex_destroy(struct xsp_mutex &mt)
{
	pthread_mutex_destroy(&mt.mutex_t);
}

/**
 * mutex_lock -- to lock mutex object
 * @mt: the structure of xsp_mutex to lock
 * 		mutex object
 */
inline int mutex_lock(struct xsp_mutex &mt)
{
	int ret = 0;
	mt.blocked = true;
	ret = pthread_mutex_lock(&mt.mutex_t);
	if(0 != ret)
		mt.blocked = false;
	return ret;
}

/**
 * mutex_unlock -- to unlock mutex object
 * @mt:	the structure of xsp_mutex to unlock
 * 		mutex object
 */
inline int mutex_unlock(struct xsp_mutex &mt)
{
	int ret = 0;
	ret = pthread_mutex_unlock(&mt.mutex_t);
	if(ret == 0)
		mt.blocked = false;
	return ret;
}

/**
 * mutex_trylock -- to try lock mutex object
 * 		if can lock that will locked
 * @mt:	the structure of xsp_mutex to try lock
 * 		mutex object
 */
inline int mutex_trylock(struct xsp_mutex &mt)
{
	int ret = 0;
	ret = pthread_mutex_trylock(&mt.mutex_t);
	if(ret == 0)
		mt.blocked = true;
	return ret;
}

/**
 * mutex_islocked -- to judge the object wheather
 * 		locded, if locked return true,then return
 * 		false
 * @mt:	the structure of xsp_mutex to judge the
 * 		object wheather locked.
 */
inline bool mutex_islocked(struct xsp_mutex &mt)
{
	return mt.blocked;
}


#endif /* THREAD_XMUTEX_H_ */


#ifndef THREAD_XTHREAD_H_
#define THREAD_XTHREAD_H_

#include<pthread.h>
#include<string.h>
#include<vector>
using namespace std;


typedef void *(*pthread_func)(void *);
typedef void (*pthread_cleanup_func)(void *);
typedef void (*pthread_once_func)(void);
typedef void (*pthread_key_func)(void *);

struct xsp_thread
{
	vector<pthread_t> pthrid;  //the id of thread that to be created
	pthread_attr_t attr;       //the attribute of thread that to set
	pthread_func func;         //the function to be called when create
	pthread_cleanup_func cfunc;//the funciton to be called by pthread_cleanup_push
	pthread_mutex_t mutex;     //the mutual exclusion
	pthread_cond_t cond;       //the conditional variable
	pthread_once_func ofunc;   //the function to be called by pthread_once
	pthread_key_func kfunc;    //the function to be called by pthread_key_create
};

/**
 * init_thread -- Initialize the parameter of xsp_thread structure
 * @thrp: the pointer that xsp_thread
 * @mutexattr: mutual exclusion of thread
 * @condattr: conditional varialbe of thread
 * @func: pointer to function be called when create thread
 * @cfunc: pointer to function be called when exit thread
 * @ofunc: pointer to function be called when call the function
 * 			of once_thread
 * @kfunc: pointer to function be called when call the function
 * 			of key_create_thread
 */
inline void init_thread(struct xsp_thread *thrp,
		const pthread_mutexattr_t *mutexattr,
		const pthread_condattr_t *condattr,
		const pthread_func func,
		const pthread_cleanup_func cfunc=NULL,
		const pthread_once_func ofunc=NULL,
		const pthread_key_func kfunc=NULL)
{
	if(thrp)
	{
		thrp->func   = func;
		thrp->cfunc  = cfunc;
		thrp->ofunc  = ofunc;
		thrp->cfunc  = cfunc;
		thrp->pthrid.clear();
		bzero(&thrp->attr, sizeof(thrp->attr));
		pthread_mutex_init(&thrp->mutex, mutexattr);
		pthread_cond_init(&thrp->cond, condattr);
	}
}

/**
 * attr_init_thread -- Initialize the parameter of structure
 * 		               xsp_thread attr
 * @thrp: point to a structure of xsp_thread
 */
inline int attr_init_thread(struct xsp_thread *thrp)
{
	if(thrp)
	{
		return pthread_attr_init(&thrp->attr);
	}
	return -1;
}

/**
 * attr_setscope_thread -- set scope of thread
 * @thrp: pointer to structure of xsp_thread
 * @scope: the values of variable are  PTHREAD_SCOPE_SYSTEM
 * 		or PTHREAD_SCOPE_PROCESS
 */
inline int attr_setscope_thread(struct xsp_thread *thrp, int scope)
{
	if(thrp)
	{
		return pthread_attr_setscope(&thrp->attr, scope);
	}
	return -1;
}

/**
 * attr_getschedparam_thread -- get parameter of thread
 * @thrp: pointer to structure of xsp_thread
 * @param: save the values of parameter
 */
inline int attr_getschedparam_thread(struct xsp_thread *thrp, sched_param &param)
{
	int ret = -1;
	if(thrp)
	{
		ret = pthread_attr_getschedparam(&thrp->attr, &param);
	}
	return ret;
}

/**
 * attr_setschedparam_thread -- set parameter of thread
 * @thrp: pointer to structure of xsp_thread
 * @priority: the values of parameter to set
 */
inline int attr_setschedparam_thread(struct xsp_thread *thrp, int priority)
{
	int ret = -1;
	if(thrp)
	{
		sched_param param;
		param.__sched_priority = priority;
		ret = pthread_attr_setschedparam(&thrp->attr, &param);
	}
	return ret;
}

/**
 * attr_setdetachstate_thread -- set the detach state of thread
 * @thrp: pointer to structure of xsp_thread
 * @state: the values of state is PTHREAD_CREATE_DETACHED or PTHREAD _CREATE_JOINABLE(defualt)
 */
inline int attr_setdetachstate_thread(struct xsp_thread *thrp, int state)
{
	if(thrp)
	{
		return pthread_attr_setdetachstate(&thrp->attr, state);
	}
	return -1;
}

/**
 * attr_getdetachstate_thread -- get the detach state of thread
 * @thrp: pointer to structure of xsp_thread
 * @state: save the value of detach state
 */
inline int attr_getdetachstate_thread(struct xsp_thread *thrp, int *state)
{
	if(thrp)
	{
		return pthread_attr_getdetachstate(&thrp->attr, state);
	}
	return -1;
}

/**
 * key_create_thread -- create a key of thread
 * @thrp: pointer to structur of xsp_thread
 * @key: the value of key to create
 */
inline int key_create_thread(struct xsp_thread *thrp,pthread_key_t key)
{
	if(thrp)
	{
		return pthread_key_create(&key, thrp->kfunc);
	}
	return -1;
}

/**
 * create_thread -- To create a new thread
 * @thrp: pointer to a structure of xsp_thread
 * @arg: the parameter to the function that will be called
 * 			when create a new thread
 */
inline int create_thread(struct xsp_thread *thrp, void *arg)
{
	if(thrp)
	{
		int ret;
		pthread_t threadId;
		ret = pthread_create(&threadId, &thrp->attr, thrp->func, arg);
		return (ret == 0 ? thrp->pthrid.push_back(threadId),threadId:-1);
	}
	return -1;
}

/**
 * once_thread -- Initialize a function to be called once
 * @thrp: pointer to a structure of xsp_thread
 */
inline int once_thread(struct xsp_thread *thrp)
{
	pthread_once_t once = PTHREAD_ONCE_INIT;
	return pthread_once(&once, thrp->ofunc);
}


#ifdef pthread_cleanup_push_defer_np
#define cleanup_push_thread(thrp, arg) \
	pthread_cleanup_push_defer_np(thrp->cfunc, arg)
#define cleanup_pop_thread pthread_cleanup_pop_restore_np
#else
#define cleanup_push_thread(thrp, arg)\
do{\
	if(thrp)\
	{\
		int oldtype, ret;\
		ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);\
		if(0 == ret)\
		{\
			pthread_cleanup_push(thrp->cfunc, arg)

#define cleanup_pop_thread(execute)\
			pthread_cleanup_pop(execute);\
			pthread_setcanceltype(oldtype, NULL);\
		}\
	}\
}while(0)
#endif

/**
 * destroy_thread -- to destroy memory
 * @thrp: pointer to a structure xsp_thread
 */
inline void destroy_thread(struct xsp_thread *thrp)
{
	if(thrp)
	{
		pthread_mutex_destroy(&thrp->mutex);
		pthread_cond_destroy(&thrp->cond);
	}
}
/**
 * exit_thread -- exit a thread
 * @thrp: pointer to a structure of xsp_thread
 */
inline void exit_thread(struct xsp_thread *thrp)
{
	if(thrp)
	{
		pthread_exit((void *)&thrp->pthrid);
	}
}

#endif /* THREAD_XTHREAD_H_ */

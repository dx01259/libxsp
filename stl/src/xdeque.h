#ifndef STL_XDEQUE_H_
#define STL_XDEQUE_H_


#include<deque>
using namespace std;

template<typename V>
struct xsp_deque
{
	deque<V> data;
	size_t   cursize;
	size_t   maxsize;
};

template<typename V>
inline size_t init_deque(struct xsp_deque<V> *dep,
		const size_t msize)
{
	if(dep)
	{
		dep->cursize = 0;
		dep->data.clear();
		dep->maxsize = msize;
		return msize;
	}
	return 0;
}

template<typename V>
inline size_t reset_deque(struct xsp_deque<V> *dep, size_t msize)
{
	if(dep && msize >= dep->data.size())
	{
		dep->maxsize = msize;
		return msize;
	}
	return 0;
}

template<typename V>
inline V *front_deque(struct xsp_deque<V> *dep, V &data)
{
	if(dep && dep->data.size()>0)
	{
		data = dep->data[0];
		return &data;
	}
	return NULL;
}

template<typename V>
inline V *back_deque(struct xsp_deque<V> *dep, V &data)
{
	if(dep && dep->data.size()>0)
	{
		data = dep->data[-1+dep->cursize];
		return &data;
	}
	return NULL;
}

template<typename V>
inline V *pop_front_deque(struct xsp_deque<V> *dep, V &data)
{
	if(dep && dep->cursize > 0 &&
			dep->data.size()>0)
	{
		--dep->cursize;
		data = dep->data[0];
		dep->data.pop_front();
		return &data;
	}
	return NULL;
}

template<typename V>
inline bool push_front_deque(struct xsp_deque<V> *dep, const V data)
{
	if(dep && dep->cursize<dep->maxsize &&
			dep->data.size()<dep->maxsize)
	{
		++dep->cursize;
		dep->data.push_front(data);
		return true;
	}
	return false;
}

template<typename V>
inline V *pop_back_deque(struct xsp_deque<V> *dep, V &data)
{
	if(dep && dep->cursize > 0 &&
			dep->data.size()>0)
	{
		--dep->cursize;
		data = dep->data[-1+dep->cursize];
		dep->data.pop_back();
		return &data;
	}
	return NULL;
}

template<typename V>
inline bool push_back_deque(struct xsp_deque<V> *dep, const V data)
{
	if(dep && dep->cursize<dep->maxsize &&
			dep->data.size()<dep->maxsize)
	{
		++dep->cursize;
		dep->data.push_back(data);
		return true;
	}
	return false;
}

template<typename V>
inline bool insert_deque(struct xsp_deque<V> *dep, typename std::deque<V>::const_iterator where, V *first, V *last)
{
	if(dep)
	{
		int space = last-first;
		if(space < 0)
			space = -space;
		if(space > dep->maxsize-dep->cursize)
			return false;
		dep->data.insert(where, first, last);
		return true;
	}
	return false;
}

template<typename V>
inline size_t size_deque(struct xsp_deque<V> *dep, int offset=0)
{
	if(dep)
	{
		return dep->data.size()+offset;
	}
	return 0;
}

template<typename V>
inline bool empty_deque(struct xsp_deque<V> *dep)
{
	if(dep)
	{
		return dep->data.empty();
	}
	return true;
}

template<typename V>
inline void clear_deque(struct xsp_deque<V> *dep)
{
	if(dep)
	{
		dep->cursize = 0;
		dep->data.clear();
	}
}

#endif /* STL_XDEQUE_H_ */


#ifndef STL_XSPMAP_H_
#define STL_XSPMAP_H_

#include <stl/xmap.h>
#include"thread/XSPMutex.h"

template<class K, class V, class MUTEX=THREAD_MUTEX>
class CXSPMap
{
public:
	CXSPMap(){}
	CXSPMap(const CXSPMap &robj)
	{
		this->operator=(robj);
	}
	CXSPMap &operator=(const CXSPMap &robj)
	{
		// this->m_mutex need not copy
		if(&robj != this)
		{
			this->m_map = robj.m_map;
		}
		return *this;
	}
	virtual ~CXSPMap(){}
public:
	void GetMap(xsp_map<K,V> mapdata)
	{
		AUTO_GUARD(g, MUTEX, this->m_mutex);
		mapdata = this->m_map;
	}

	void SetMap(xsp_map<K,V> mapdata)
	{
		AUTO_GUARD(g, MUTEX, this->m_mutex);
		try
		{
			this->m_map = mapdata;
		}catch(...){

		}
	}

	int Find(const K &key, V &value)
	{
		AUTO_GUARD(g, MUTEX, this->m_mutex);
		return m_map.find(key, value);
	}
	int Set(const K &key, const V &value)
	{
		AUTO_GUARD(g, MUTEX, this->m_mutex);
		return m_map.set(key, value);
	}
	size_t GetAllValues(vector<V> &values)
	{
		AUTO_GUARD(g, MUTEX, this->m_mutex);
		return m_map.get_all_values(values);
	}
	void Remove(const K &key)
	{
		AUTO_GUARD(g, MUTEX, this->m_mutex);
		m_map.remove(key);
	}
	size_t Size()
	{
		AUTO_GUARD(g, MUTEX, this->m_mutex);
		return m_map.size();
	}
	void Clear()
	{
		AUTO_GUARD(g, MUTEX, this->m_mutex);
		m_map.clear();
	}
private:
	struct xsp_map<K,V>  m_map;
	MUTEX  m_mutex;
};

#endif /* STL_XSPMAP_H_ */

#ifndef STL_XMAP_H_
#define STL_XMAP_H_

#include<map>
#include<vector>

using namespace std;

template <class K, class V>
struct xsp_map
{
public:
	int find(const K &key, V &value)
	{
		typename map<K, V>::iterator iter;
		iter = vmap.find(key);
		if(iter == vmap.end())
			return -1;
		value = iter->second;
		return 0;
	}

	V findex(const K &key, const V&value)
	{
		try{
			typename map<K, V>::iterator iter;
			iter = vmap.find(key);
			if(iter != vmap.end())
			{
				return iter->second;
			}
			else{
				return vmap[key] = value;
			}
		}
		catch(...)
		{
		}

		throw "key not found in map";
	}

	int set(const K &key, const V &value)
	{
		try{
			vmap[key] = value;
			return 0;
		}
		catch(...){
			return -1;
		}
	}
	size_t get_all_values(vector<V> &values)
	{
		try{
			values.clear();
			values.reserve(vmap.size());
			typename map<K, V>::iterator iter;
			iter = vmap.begin();
			for(; iter!=vmap.end(); ++iter)
			{
				values.push_back(iter->second);
			}
			return values.size();
		}
		catch(...){
			return -1;
		}

	}
	void remove(const K &key)
	{
		try{
			typename map<K, V>::iterator iter;
			iter = vmap.find(key);
			if(iter != vmap.end())
				vmap.erase(iter);
		}
		catch(...){
			return;
		}
	}

	size_t size()
	{
		return vmap.size();
	}

	void clear()
	{
		try{
			vmap.clear();
		}
		catch(...){
			return;
		}
	}
private:
	map<K, V> vmap;
};



#endif /* STL_XMAP_H_ */

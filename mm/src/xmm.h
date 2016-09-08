#ifndef MM_XMM_H_
#define MM_XMM_H_

#include<comm/xdatatype.h>
#include<stdio.h>
#include<netinet/in.h>
#include<string.h>

#include<string>
#include<deque>
using namespace std;

/**
 * The Structure of xsp_mmset
 * is to save bytes stream data
 */
struct xsp_mmset
{
	int rd_pos;
	int wr_pos;
	int mm_size;
	deque<byte> base;
};

/**
 * init_mmset -- initialise the structure
 * of xsp_mmset object
 * @mmset: pointer to a xsp_mmset object
 */
inline void init_mmset(struct xsp_mmset *mmset)
{
	if(mmset)
	{
		mmset->rd_pos  = 0;
		mmset->wr_pos  = 0;
		mmset->mm_size = 0;
		mmset->base.clear();
	}
}

/**
 * set_mmset_resize -- set the buffer size of xsp_mmset object
 * @mmset: pointer to xsp_mmset object to set buffer size
 * @size: the size value that to be setted
 */
inline int set_mmset_resize(struct xsp_mmset *mmset, int size)
{
	if(mmset)
	{
		mmset->base.resize(size);
		return mmset->base.size();
	}
	return -1;
}

/**
 * get_mmset_size -- get the buffer size of xsp_mmset object
 * @mmset: pointer to xsp_mmset object for get buffer size
 */
inline int get_mmset_size(struct xsp_mmset *mmset)
{
	if(mmset)
	{
		return mmset->base.size();
	}
	return -1;
}

/**
 * get_mmset_length -- get the length of effective data
 * @mmset: pointer to xsp_mmset object for get size of
 * effective data
 */
inline int get_mmset_length(struct xsp_mmset *mmset)
{
	if(mmset)
	{
		return mmset->wr_pos - mmset->rd_pos;
	}
	return -1;
}

/**
 * read_mmset -- read data of xsp_mmset object
 * @value: pointer to a built-in type object
 */
template<class V>
inline int read_mmset(struct xsp_mmset *mmset, V &value)
{
	if(mmset)
	{
		uint32 len = mmset->wr_pos-mmset->rd_pos;
		if(len >= sizeof(value))
		{
			memcpy(&value, &mmset->base.at(mmset->rd_pos), sizeof(value));
			value = htonl(value);
			mmset->rd_pos += sizeof(value);
			return sizeof(value);
		}
	}
	return -1;
}

/**
 * write_mmset -- write data of xsp_mmset object
 * @value: pointer to a built-in type object
 */
template<class V>
inline int write_mmset(struct xsp_mmset *mmset, const V value)
{
	if(mmset)
	{
		if(mmset->base.size()-mmset->wr_pos>=sizeof(value))
		{
			V hval = htonl(value);
			memcpy(&mmset->base[mmset->wr_pos], (char *)&hval, sizeof(hval));
			mmset->wr_pos += sizeof(hval);
		}
	}
	return -1;
}

/**
 * read_mmset -- read data of xsp_mmset object
 * @value: pointer to a string type object
 * @len:   pointer to length that to read
 */
inline int read_mmset(struct xsp_mmset *mmset, string &value, int len)
{
	if(mmset)
	{
		if(len <= mmset->wr_pos-mmset->rd_pos)
		{
			value.resize(len);
			memcpy(&value[0], &mmset->base.at(mmset->rd_pos), len);
			mmset->rd_pos += len;
			return len;
		}
	}
	return -1;
}

/**
 * write_mmset -- write data of xsp_mmset object
 * @value: pointer to a string type object
 */
inline int write_mmset(struct xsp_mmset *mmset, const string value)
{
	if(mmset)
	{
		if(mmset->base.size()-mmset->wr_pos>=value.size())
		{
			memcpy(&mmset->base[mmset->wr_pos], (char *)&value[0], value.size());
			mmset->wr_pos += value.size();
		}
	}
	return -1;
}

/**
 * clear_mmset -- clear the data in xsp_mmset object
 * @mmset: pointer to xsp_mmset object to clear
 */
inline int clear_mmset(struct xsp_mmset *mmset)
{
	if(mmset)
	{
		mmset->base.clear();
		mmset->mm_size = 0;
		mmset->rd_pos = 0;
		mmset->wr_pos = 0;
		return 0;
	}
	return -1;
}
#endif /* MM_XMM_H_ */

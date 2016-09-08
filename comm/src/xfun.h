#ifndef COMM_XFUN_H_
#define COMM_XFUN_H_

#include<comm/xdatatype.h>
#include<string.h>
#include<sys/sysinfo.h>
#include<malloc.h>

#include<vector>
#include<string>
using namespace std;

inline string &replace_all_sdistinct(string &str, const string &old_value, const string &new_value)
{
	for(string::size_type pos(0); pos!=string::npos; pos+=new_value.length())
	{
		if(   (pos=str.find(old_value,pos))!=string::npos   )
			str.replace(pos,old_value.length(),new_value);
		else   break;
	}
	return str;
}

inline char *replace_all_cdistinct(
		char *src,/*the string that to replace*/
		int  srlen,/*the buff size of src pointed*/
		const char *oval,/*the old value that to be replaced by nval*/
		const char *nval /*the new value to replace the oval*/
	)
{
	int  pos=0;
	string szTemp = src;
	char *sPtr = NULL, *pRet=NULL;
	for(pos=0, sPtr=(char *)szTemp.c_str(); pos<srlen; sPtr=NULL)
	{
		char *token = strtok_r(sPtr, oval, &pRet);
		if(NULL == token)
			break;
		memcpy(&src[pos], token, strlen(token));
		pos += strlen(token);
		memcpy(&src[pos], nval, strlen(nval));
		pos += strlen(nval);
	}
	return src;
}

template<typename V>
inline void remove_vector_data(vector<V> &vec, V val)
{
	typename vector<V>::iterator iter = vec.begin();
	for(; iter != vec.end(); ++iter)
	{
		if(*iter == val)
		{
			vec.erase(iter);
			break;
		}
	}
}

inline bool Char2Hex(const char *chr, const size_t len, vector<char> &hex)
{
	if(NULL==chr || len<=0)
		return false;
	hex.reserve( len * 3 + 1 );
	char *p = &hex[0];
	bzero(p, len*3+1);
	for(uint32 i=0,j=0; i<len; i++,j+=3)
	{
		sprintf( p+j, "%02X ", ((byte *)chr)[i] );   /*转换为16进制显示*/
	}
	return true;
}

inline u_long get_cpu_core_num()
{
#ifdef WIN32
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
#else
	return get_nprocs();
#endif
}

#ifndef DELETE_MEMORY
#define DELETE_MEMORY(ptr) do{\
	if(ptr)\
	{\
		delete[] ptr;\
		ptr = NULL;\
	}\
}\
while(0)
#endif

#ifndef FREE_MEMORY
#define FREE_MEMORY(ptr) do{\
	if(ptr)\
	{\
		free(ptr);\
		ptr = NULL;\
	}\
}\
while(0)
#endif


#endif /* COMM_XFUN_H_ */

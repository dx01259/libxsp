#ifndef NET_XCURL_H_
#define NET_XCURL_H_

#include<curl/curl.h>

#include<string>
using namespace std;

typedef enum {
	CPROTOCOL_HTTP = 0,
	CPROTOCOL_FTP,       /* 1 */
	CPROTOCOL_FTPS,      /* 2 */
	CPROTOCOL_GOPHER,    /* 3 */
	CPROTOCOL_TELNET,    /* 4 */
	CPROTOCOL_DICT,      /* 5 */
	CPROTOCOL_FILE,      /* 6 */
	CPROTOCOL_LDAT,      /* 7 */
	CPROTOCOL_SOAP,      /* 8 */
	CPROTOCOL_HTTP,      /* 9 */
}CProtcode;

struct xsp_curl
{
	CURLcode  url_code;
	CProtcode pro_code;
};

int curl_init(struct xsp_curl *curlp, const long flags, CProtcode code)
{
	if(curlp)
	{
		curlp->xsp_curl = curl_global_init(flags);
		if(CURLE_OK == curlp->xsp_curl)
		{
			curlp->pro_code = code;
			return 0;
		}
	}
	return -1;
}

int curl_destroy(struct xsp_curl *curlp)
{
	if(NULL != curlp &&
			CURLE_OK==curlp->xsp_curl)
	{
		curl_global_cleanup();
		return 0;
	}
	return -1;
}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}

inline int curl_get(struct xsp_curl *curlp, const string &szURL, string &szResponse)
{
	if(curlp)
	{
		CURLcode res;
		CURL *curl = curl_easy_init();
		if(NULL != curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, szURL.c_str());
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&szResponse);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			/**
			 * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
			 * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
			 */
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);

			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			return res;
		}
		else
		{
			return CURLE_FAILED_INIT;
		}
	}
	return -1;
}

inline int curl_post(
		struct xsp_curl *curlp,
		const string &szURL,
		const string &szPost,
		string &szResponse)
{
	if(curlp)
	{
		CURLcode res;
		CURL *curl = curl_easy_init();
		if(curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, szURL.c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szPost.c_str());
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&szResponse);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			return res;
		}
		else{
			return CURLE_FAILED_INIT;
		}
	}
	return -1;
}


inline int curl_gets(
		struct xsp_curl *curlp,
		const string &szURL,
		string &szResponse,
		const char * csslpath)
{
	if(curlp)
	{
		CURLcode res;
		CURL *curl = curl_easy_init();
		if(NULL != curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, szURL.c_str());
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&szResponse);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
			/**
			 * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
			 * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
			 */
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

			if(!csslpath)
			{
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
			}
			else
			{
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
				curl_easy_setopt(curl, CURLOPT_CAINFO, csslpath);
			}

			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);

			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			return res;
		}
		else
		{
			return CURLE_FAILED_INIT;
		}
	}
	return -1;
}

inline int curl_posts(
		struct xsp_curl *curlp,
		const string &szURL,
		const string &szPost,
		string &szResponse,
		const char * csslpath)
{
	if(curlp)
	{
		CURLcode res;
		CURL *curl = curl_easy_init();
		if(curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, szURL.c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szPost.c_str());
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&szResponse);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

			if(!csslpath)
			{
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
			}
			else
			{
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
				curl_easy_setopt(curl, CURLOPT_CAINFO, csslpath);
			}
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			return res;
		}
		else{
			return CURLE_FAILED_INIT;
		}
	}
	return -1;
}





#endif /* NET_XCURL_H_ */

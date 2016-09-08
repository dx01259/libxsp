#ifndef MM_XSPIOBUFFERPTR_H_
#define MM_XSPIOBUFFERPTR_H_

#include <comm/xdatatype.h>
#include"xmm.h"
#include"thread/XSPMutex.h"

class CXSPIOBufferPtr
{
public:
	CXSPIOBufferPtr(int size=0);
	virtual ~CXSPIOBufferPtr();
public:
	int Resize(int size);
	int GetSize();
	int GetLength();

	//read data
	int Read(int8 &value);
	int Read(int16 &value);
	int Read(int32 &value);
	int Read(int64 &value);
	int Read(long &value);
	int Read(char &value);
	int Read(string &value, int len);

	//write data
	int Write(const int8 value);
	int Write(const int16 value);
	int Write(const int32 value);
	int Write(const int64 value);
	int Write(const long value);
	int Write(const char value);
	int Write(const string value);

	int Clear();
private:
	struct xsp_mmset m_mmset;
	CXSPMutex m_mutex;
};

#endif /* MM_XSPIOBUFFERPTR_H_ */

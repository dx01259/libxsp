#include"xdef.h"
#include<string>
using namespace std;

static string xsp_error[ERROR_ALL_NUMBER] = {
		"",
		"The value of parameter is null",
		"The type of socket is not SOCK_SERVICE",
		"The socket object is invalid",
		"The address of socket is invalid",
		"Errored when calling the function of socket to create a socket",
		"Errored when calling the function of bind to bind a socket",
		"Errored when calling the function of fcntl to change attribute of socket"
		"Errored when calling the function of setsockopt to set option of socket is error",
		"Errored when calling the function of listen to listen a socket",
		"Errored when calling the function of accept to accept a client's socket",
		"Errored when calling the function of connect to connect a service's socket",
		"Errored when calling the function of close to close a socket",
		"Errored when calling the function of recvfrom to receive datas",
		"Errored when calling the function of recv to receive datas",
		"Errored when calling the function of sendto to send datas",
		"Errored when calling the function of send to send datas",
		"Errored when create directory",
		"Errored when calling the funciton of fopen to open a file",
		"Errored when calling the function of fclose to close a file",
		"Errored when create instance by epoll_create funciton",
		"Errored when calling the function of epoll_ctl",
		"Errored when calling the function of epoll_pwait",
		"Errored when calling the function of fcntl",
		"Errored when change the size of object",
		"Errored when calling the function of mutex_lock",
		"Errored when calling the function of mutex_unlock",
		"Errored when calling the function of mutex_trylock"
};

char *GetLastError(const int index)
{
	if(index < ERROR_ALL_NUMBER)
	{
		return (char *)xsp_error[index].c_str();
	}
	return NULL;
}

#ifndef ERRO_XXDEF_H_
#define ERRO_XXDEF_H_

enum xsp_error_type
{
	ERROR_NOT_VALUE = 0,      /*The mean is without error*/
	ERROR_PARAMS_VALUE,       /*The value of parameter is null*/
	ERROR_SOCK_MEDTH,         /*The socket methed is not right*/
	ERROR_SOCK_OBJECT,        /*The object of socket is invalid*/
	ERROR_ADDRESS_SOCK,       /*The address of socket is invalid*/
	ERROR_CREATE_SOCK,        /*Errored when calling the function of socket to create a socket*/
	ERROR_BIND_SOCK,          /*Bind the socket is error*/
	ERROR_FCNTL_SOCKET,       /*The error from Call the function of fcntl*/
	ERROR_SETOPT_SOCK,        /*Set the socket option is error*/
	ERROR_LISTEN_SOCK,        /*Listen the socket is error*/
	ERROR_ACCEPT_SOCK,        /*Accept the client's socket is error*/
	ERROR_CONNECT_SOCK,       /*Connect the service socket is error*/
	ERROR_CLOSE_SOCK,         /*Close the socket is error*/
	ERROR_RECVFROM_SOCK,      /*Recevie data from UDP socket is error*/
	ERROR_RECVF_SOCK,         /*Recevie data from TCP socket is error*/
	ERROR_SENDTO_SOCK,        /*Send data from UDP socket is error*/
    ERROR_SEND_SOCK,          /*Send data from TCP socket is error*/
	ERROR_CREATE_DIRECTORY,   /*Create the directory is failed*/
	ERROR_OPEN_FILE,          /*Open file by fopen is failed*/
	ERROR_CLOSE_FILE,         /*Close file by fclose is failed*/
	ERROR_EPOLL_CREATE,       /*Create a instance by epoll_create function*/
	ERROR_EPOLL_CTL,          /*The error from Call the function of epoll_ctl*/
	ERROR_EPOLL_WAIT,         /*The error from Call the function of epoll_pwait*/
	ERROR__FCNTL,             /*The error form Call the function of fcntl*/
	ERROR_RESIZE,             /*Change the size of object is error*/
	ERROR_MUTEX_LOCK,         /*The error from using mutex lock*/
	ERROR_MUTEX_UNLOCK,       /*The error from using mutex unlock*/
	ERROR_MUTEX_TRYLOCK,      /*The error from using mutex trylock*/

	ERROR_ALL_NUMBER
};

extern char *GetLastError(const int index);

#endif /* ERRO_XXDEF_H_ */

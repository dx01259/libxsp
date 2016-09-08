#ifndef LIBDB_XDB_H_
#define LIBDB_XDB_H_

#include<mysql/mysql.h>
#include<sqlora.h>
#include<sys/types.h>
#include<stdint.h>

#include<string>

using namespace std;

enum DBTYPE{
	DB_ORACLE=0,
	DB_MYSQL=1
};

enum LIBTYPE{
	LIB_SQL_CLIENT=0,
	LIB_SQL_MYSQLD,
};

#define DB_HOST   "Host"          //the name of host
#define DB_USER   "User Id"       //the name of user
#define DB_PASSWD "Password"      //the password of user
#define DB_SQLDB  "DataBase"      //the database
#define DB_PORT   "Port"          //the port of database
#define DB_SOCK   "Socket"        //the socket

struct oracle
{
	int ora_mode;         //SQLO_ON is start thread, SQLO_OFF is shutdown thread
	int ora_num;          //the numbers of database for oracle
	int ora_cur;          //the numbers of cursor for oracle
	int ora_dbhp;         //the id of cursor for oracle
};

struct rdset_oracle
{
	sqlo_stmt_handle_t rd_orap; //the handle of oracle' select
};

struct mysql
{
	MYSQL      *mys_sqlp;               //the handle of MySql
	int         mys_lib_type;           /*the connection type of mysqlclient is
		                                     *use client library function of c, and
		                                     *the connection type of MySql is use
		                                     *embedded library function of c
		                                     */
	char       *mys_hostp;              //the name of host
	char       *mys_userp;              //the name of user
	char       *mys_pwdp;               //the password of user
	char       *mys_dbp;                //the name of database
	int32_t     mys_port;               //the port of database
	char       *mys_socketp;            //the socket for communication
	uint64_t    mys_flags;              //the flags of client
};

struct rdset_mysql
{
	MYSQL_RES  *rd_resp;        //Query value corresponding to the results set
	MYSQL_ROW   rd_rowp;        //Query value of column which result set
};

struct libdb
{
	int       db_type;        //save the type of database, such as oracle or MySql
	char     *db_connp;       //save the string to connect to database
	string    db_errorp;      //the error for operate database
	struct oracle *db_orap;   //a pointer to a attribute of oracle
	struct mysql  *db_sqlp;   //the object of MySql to connect database
};

struct rdset
{
	short                rd_db_type;
	struct rdset_oracle *rd_orap;
	struct rdset_mysql  *rd_mysqlp;
};

/**
 * init - initialize the parameter of structure
 * @dbp: pointer to structure of libdb
 * @dbtype: initialize the database type that is
 *          DB_ORACLE or DB_MYSQL
 * @pbase: pointer to structure of database through
 * 		   parameter of dbtype
 */
int init(struct libdb *dbp, int dbtype, void *pbase);
/**
 * set_conn_str -  set the string to connect the database
 * @dbp: pointer to structure of libdb
 * @pstr: pointer character string to connect the database
 */
void set_conn_str(struct libdb *dbp, const char *pstr);

/*
 * analyse - parse the information of the connection string
 * @dbp: pointer to structure of libdb
 * @pstr: pointer character string to connect the database
 */
void analyse(const struct libdb *dbp, char *pstr);

/*
 * get_value - get the right value of '=' by the string
 * 			   that used connect database
 * @pstr: pointer character string to connect the database
 * @value: pointer the value that the right of the '='
 */
char *get_value(const char *pstr, char *value);

/*
 * is_open - ensure the database is opened
 * @dbp: pointer to structure of libdb
 */
int is_open(const struct libdb *dbp);

/*
 * open - open the database
 * @dbp: pointer to structure of libdb
 */
int open(struct libdb *dbp);

/*
 * execute - Execute the SQL statements
 * @dbp: pointer to structure of libdb
 * @qcmd: pointer to the SQL character string
 */
int execute(struct libdb *dbp, const char *qcmd);

/*
 * execute2 - Execute the SQL statements
 * @dbp: pointer to structure of libdb
 * @len: the length of character string
 * @format: pointer the character string
 */
int execute2(struct libdb *dbp, int len, const char *format, ...);

/*
 * select2 - Query the SQL statements
 * @dbp: pointer to structure of libdb
 * @rdsetp: pointer the result of records
 * @qcmd: pointer the SQL character string
 */
int select2(struct libdb *dbp, struct rdset *rdsetp, const char *qcmd);

/*
 * select3 - Query the SQL statements
 * @dbp: pointer to structure of libdb
 * @rdsetp: pointer the result of records
 * @len: the length of character string
 * @format: pointer the character stirng
 */
int select3(struct libdb *dbp, struct rdset *rdsetp, int len, const char *format, ...);

/*
 * is_eof - judge it is read over for records
 * @rdsetp: pointer the result of records
 */
int is_eof(struct rdset *rdsetp);

/*
 * get_record_count - get the numbers of record
 * @rdsetp: pointer the result of records
 */
int get_record_count(struct rdset *rdsetp);

/*
 * get_index_value - get value by index from records
 * @rdsetp: pointer the result of records
 * @index: the index of the records
 * @value: the value of index from records
 */
void get_index_value(struct rdset *rdsetp, const int index, char *value);

/*
 * get_field_value - get value by field from records
 * @rdsetp: pointer the result of records
 * @field: pointer the value of field's address
 * @value: the value of the field
 */
void get_field_value(struct rdset *rdsetp, const char *field, char *value);

/*
 * close_db - close the database
 * @dbp: pointer to structure of libdb
 */
int close_db(struct libdb *dbp);

/*
 * close_rdset - free memory
 * @rdsetp: pointer the record's address
 */
int close_rdset(struct rdset *rdsetp);

/*
 * set_error - informations of error
 * @dbp: pointer to structure of libdb
 * @str: pointer to character string of error
 */
void set_error_d(struct libdb *dbp, const char *str);
/*
 * get_last_error - get the last of error
 * @dbp: pointer to structure of libdb
 */
char *get_last_error(const struct libdb *dbp);




#endif /* LIBDB_XDB_H_ */

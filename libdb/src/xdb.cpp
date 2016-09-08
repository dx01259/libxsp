#include"xdb.h"
#include"comm/xfun.h"
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>

int init(struct libdb *dbp, int dbtype, void *pbase)
{
	if(NULL == dbp)
	{
		set_error_d(dbp, "The parameter of pointer is null.");
		return -1;
	}
	dbp->db_connp  = (char *)malloc(sizeof(char));
	dbp->db_errorp = (char *)malloc(sizeof(char));
	dbp->db_type   = -1;
	switch(dbtype)
	{
	case DB_ORACLE:
		{
			dbp->db_orap = (oracle *)malloc(sizeof(struct oracle));
			dbp->db_sqlp = NULL;
			if(NULL == dbp->db_orap)
			{
				set_error_d(dbp, "Apply memory space is failure");
				return -2;
			}
			if(NULL == pbase)
			{
				dbp->db_orap->ora_mode = SQLO_ON;
				dbp->db_orap->ora_num  = 1;
				dbp->db_orap->ora_cur  = 100;
				dbp->db_orap->ora_dbhp = -1;
			}
			else
			{
				dbp->db_orap->ora_mode = ((struct oracle *)pbase)->ora_mode;
				dbp->db_orap->ora_num  = ((struct oracle *)pbase)->ora_num;
				dbp->db_orap->ora_cur  = ((struct oracle *)pbase)->ora_cur;
				dbp->db_orap->ora_dbhp = ((struct oracle *)pbase)->ora_dbhp;
			}
		}
		break;
	case DB_MYSQL:
		{
			dbp->db_orap = NULL;
			dbp->db_sqlp = (mysql *)malloc(sizeof(struct mysql));
			if(NULL == dbp->db_sqlp)
			{
				set_error_d(dbp, "Apply memory space is failure");
				return -2;
			}
			if(NULL == pbase)
			{
				dbp->db_sqlp->mys_sqlp = (MYSQL *)malloc(sizeof(MYSQL));
				if(NULL == dbp->db_sqlp->mys_sqlp)
				{
					set_error_d(dbp, "Apply memory space is failure");
					return -2;
				}
				dbp->db_sqlp->mys_lib_type = LIB_SQL_CLIENT;
				dbp->db_sqlp->mys_hostp    = NULL;
				dbp->db_sqlp->mys_userp    = NULL;
				dbp->db_sqlp->mys_pwdp     = NULL;
				dbp->db_sqlp->mys_port     = 0;
				dbp->db_sqlp->mys_dbp      = NULL;
				dbp->db_sqlp->mys_socketp  = NULL;
				dbp->db_sqlp->mys_flags    = 0;
			}
			else
			{
				dbp->db_sqlp->mys_sqlp = ((struct mysql *) pbase)->mys_sqlp;
				if (NULL == dbp->db_sqlp->mys_sqlp)
				{
					set_error_d(dbp, "Apply memory space is failure");
					return -2;
				}
				dbp->db_sqlp->mys_lib_type = ((struct mysql *) pbase)->mys_lib_type;
				dbp->db_sqlp->mys_hostp    = ((struct mysql *) pbase)->mys_hostp;
				dbp->db_sqlp->mys_userp    = ((struct mysql *) pbase)->mys_userp;
				dbp->db_sqlp->mys_pwdp     = ((struct mysql *) pbase)->mys_pwdp;
				dbp->db_sqlp->mys_port     = ((struct mysql *) pbase)->mys_port;
				dbp->db_sqlp->mys_dbp      = ((struct mysql *) pbase)->mys_dbp;
				dbp->db_sqlp->mys_socketp  = ((struct mysql *) pbase)->mys_socketp;
				dbp->db_sqlp->mys_flags    = ((struct mysql *) pbase)->mys_flags;
			}
		}
		break;
	default:
		{
			set_error_d(dbp, "The database type is not supported.");
			return -3;
		}
		break;
	}
	return 0;
}

void set_conn_str(struct libdb *dbp, const char *pstr)
{
	if (NULL != pstr)
	{
		int len = strlen(pstr);
		dbp->db_connp = (char *)realloc(dbp->db_connp, len);
		memset(dbp->db_connp, '\0', len);
		memcpy(dbp->db_connp, pstr, len);
	}
}

void analyse(const struct libdb *dbp, char *pstr)
{
	const int type = dbp->db_type;
	char *ptr = NULL, *dest = NULL;
	switch (type)
	{
	case DB_ORACLE:
		{
			char *userp=NULL, *pwdp=NULL;
			dest = strtok_r(dbp->db_connp, ";", &ptr);
			while (dest)
			{
				if (strstr(dest, DB_USER))
				{
					get_value(dest, userp);
				}
				else if (strstr(dest, DB_PASSWD))
				{
					get_value(dest, pwdp);
				}
				dest = strtok_r(NULL, ";", &ptr);
			}
			int len = strlen(userp) + strlen(pwdp) + 2;
			if(NULL != pstr)
				pstr = (char *)realloc(pstr, len);
			else
				pstr = (char *)malloc(len);
			snprintf(pstr, len, "%s/%s", userp, pwdp);
		}
		break;
	case DB_MYSQL:
		{
			char *tmp = NULL;
			dest = strtok_r(dbp->db_connp, ";", &ptr);
			while (dest)
			{
				struct mysql *sqlp = dbp->db_sqlp;
				if (strstr(dest, DB_HOST))
				{
					if (get_value(dest, tmp))
					{
						int len = strlen(tmp);
						sqlp->mys_hostp = (char *)malloc(sizeof(char)*(len+1));
						memset(sqlp->mys_hostp, 0, len + 1);
						memcpy(sqlp->mys_hostp, tmp, len);
					}
				}
				else if (strstr(dest, DB_USER))
				{
					if (get_value(dest, tmp))
					{
						int len = strlen(tmp);
						sqlp->mys_userp = (char *)malloc(sizeof(char)*(len+1));
						memset(sqlp->mys_userp, 0, len + 1);
						memcpy(sqlp->mys_userp, tmp, len);
					}
				}
				else if (strstr(dest, DB_PASSWD))
				{
					if (get_value(dest, tmp))
					{
						int len = strlen(tmp);
						sqlp->mys_pwdp = (char *)malloc(sizeof(char)*(len+1));
						memset(sqlp->mys_pwdp, 0, len + 1);
						memcpy(sqlp->mys_pwdp, tmp, len);
					}
				}
				else if (strstr(dest, DB_SQLDB))
				{
					if (get_value(dest, tmp))
					{
						int len = strlen(tmp);
						sqlp->mys_dbp = (char *)malloc(sizeof(char)*(len+1));
						memset(sqlp->mys_dbp, 0, len + 1);
						memcpy(sqlp->mys_dbp, tmp, len);
					}
				}
				else if (strstr(dest, DB_PORT))
				{
					if (get_value(dest, tmp))
					{
						if (strlen(tmp) > 0)
							sqlp->mys_port = atol(tmp);
					}
				}
				dest = strtok_r(NULL, ";", &ptr);
			}
		}
		break;
	}
}

char *get_value(const char *pstr, char *value)
{
	int len = 0;
	char *tmp = NULL;
	tmp = strstr((char *)pstr, "=");
	len = strlen(tmp);
	if(len <= 1)
		return NULL;
	++tmp;
	if(NULL == value)
		value = (char *)malloc(len);
	else
		value = (char *)realloc(value, len);
	memcpy(value, tmp, len);
	replace_all_cdistinct(value, strlen(value), " ", "");
	value = (char *)realloc(value, strlen(value));
	return value;
}

int is_open(const struct libdb *dbp)
{
	if (NULL != dbp)
	{
		short res = 0;
		int dbType = dbp->db_type;
		switch (dbType)
		{
		case DB_ORACLE:
			{
				if (NULL != dbp->db_orap)
				{
					int ret = sqlo_isopen(dbp->db_orap->ora_dbhp);
					if (SQLO_SUCCESS == ret)
					{
						res = 1;
					}
				}
			}
			break;
		case DB_MYSQL:
			{
				if (NULL != dbp->db_sqlp)
				{
					int ret = mysql_ping(dbp->db_sqlp->mys_sqlp);
					if (0 == ret)
					{
						res = 1;
					}
				}
			}
			break;
		}
		return res;
	}
	return 0;
}

int open(struct libdb *dbp)
{
	if(NULL != dbp)
	{
		int dbType = dbp->db_type;
			int result = 0;
			switch(dbType)
			{
			case DB_ORACLE://oracle database connect
				{
					if(is_open(dbp))
					{
						result = 1;
						break;//The connection is built up, you do not need to repeat the connection
					}
					if(dbp->db_orap != NULL)
					{
						int ret, len;
						struct oracle *orap = dbp->db_orap;
						ret = sqlo_init(orap->ora_mode, orap->ora_num, orap->ora_cur);
						if(SQLO_SUCCESS != ret)
						{
							result = 0;
							set_error_d(dbp, "Failed to initialize the libsqlora8 Library:");
							break;
						}
						char *connp = NULL;
						analyse(dbp, connp);
						if((len=strlen(connp))<=0)
							break;
						connp[len-1]='\0';
						sqlo_db_handle_t *dbhp = &dbp->db_orap->ora_dbhp;
						ret = sqlo_connect(dbhp, connp);
						if(SQLO_SUCCESS != ret)
						{
							const char *error = sql_geterror();
							result = 0;
							dbp->db_errorp = "Failed to initialize the oracle Library:%s";
							int alen = len+strlen(error);
							dbp->db_errorp.resize(alen+1);
							snprintf(&dbp->db_errorp[0], alen, "Failed to initialize the oracle Library:%s", error);
							break;
						}
						sqlo_set_autocommit(dbp->db_orap->ora_dbhp, SQLO_OFF);
						result = 1;
					}
				}
				break;
			case DB_MYSQL://MySql database connect
				{
					if(is_open(dbp))
					{
						result = 1;
						break;//The connection is built up, you do not need to repeat the connection
					}
					if(dbp->db_sqlp != NULL)
					{
						MYSQL *retp = NULL ;
						retp = mysql_init(dbp->db_sqlp->mys_sqlp);
						if(NULL == retp)
						{
							result = 0;
							set_error_d(dbp, "mysql_init");
							break;
						}
						char *connp = NULL;
						analyse(dbp, connp);
						struct mysql *sqlp = dbp->db_sqlp;
						retp = mysql_real_connect(
								sqlp->mys_sqlp,              //pointer to MySql
								sqlp->mys_hostp,             //host name
								sqlp->mys_userp,             //user name
								sqlp->mys_pwdp,              //user password
								sqlp->mys_dbp,               //database name
								sqlp->mys_port,              //the port
								sqlp->mys_socketp,           //the socket
								sqlp->mys_flags              //the flags of client
							);
						if(NULL == retp)
						{
							result = 0;
							set_error_d(dbp, "mysql_real_connect");
							break;
						}
						int ret = -1;
						ret = mysql_autocommit(dbp->db_sqlp->mys_sqlp, 0);
						if(0 != ret)
						{
							result = 0;
							set_error_d(dbp, "mysql_autocommit");
							break;
						}
						result = 1;
					}
				}
				break;
			default:
				{
					result = 0;
					set_error_d(dbp, "The system is not support the database.");
				}
				break;
			}
			return result;
	}
	return 0;
}

int execute(struct libdb *dbp, const char *qcmd)
{
	if (!is_open(dbp) && !open(dbp))
	{
		return 0;
	}
	int result = 0;
	switch (dbp->db_type)
	{
	case DB_ORACLE:
		{
			int ret;
			ret = sqlo_prepare(dbp->db_orap->ora_dbhp, qcmd);
			if (ret < 0)
			{
				set_error_d(dbp, "sqlo_prepare");
				result = 0;
				break;
			}
			void *param_addr = NULL;
			ret = sqlo_bind_by_name(dbp->db_orap->ora_dbhp, ":1", SQLOT_STR, param_addr, 64, NULL, 0);
			if (SQLO_SUCCESS != ret)
			{
				set_error_d(dbp, "sqlo_bind_by_name");
				result = 0;
				break;
			}
			ret = sqlo_execute(dbp->db_orap->ora_dbhp, 1);
			if (SQLO_SUCCESS != ret)
			{
				set_error_d(dbp, "sqlo_execute");
				result = 0;
				break;
			}
			ret = sqlo_commit(dbp->db_orap->ora_dbhp);
			if (SQLO_SUCCESS != ret)
			{
				sqlo_rollback(dbp->db_orap->ora_dbhp);
				result = 0;
				break;
			}
			result = 1;
		}
		break;
	case DB_MYSQL:
		{
			MYSQL_RES *tmp = NULL;
			int ret = -1;
			unsigned long len = strlen(qcmd);
			ret = mysql_real_query(dbp->db_sqlp->mys_sqlp, qcmd, len);
			if (0 != ret)
			{
				result = 0;
				set_error_d(dbp, "mysql_query");
				break;
			}
			//Although needs no results, but still need to call the following two API function
			tmp = mysql_use_result(dbp->db_sqlp->mys_sqlp);
			if (NULL == tmp)
			{
				result = 0;
				set_error_d(dbp, "mysql_use_result");
				break;
			}
			mysql_free_result(tmp);
			ret = mysql_commit(dbp->db_sqlp->mys_sqlp);
			if (0 != ret)
			{
				result = 0;
				mysql_rollback(dbp->db_sqlp->mys_sqlp);
			}
			tmp = NULL;
			result = 1;
		}
		break;
	default:
		{
			result = 0;
			set_error_d(dbp,"the database type is not supported");
		}
		break;
	}
	return result;
}

int execute2(struct libdb *dbp, int len, const char *format, ...)
{
	int ret = 0;
	if (!is_open(dbp) && !open(dbp))
	{
		return 0;
	}
	va_list ap;
	va_start(ap, format);
	if(len<2014)
	{
		char temp[1024];
		bzero(temp, 2014);
		vsnprintf(temp, len, format, ap);
		va_end(ap);
		ret = execute(dbp, temp);
	}
	else
	{
		char strTemp[len];
		vsnprintf(strTemp, len, format, ap);
		va_end(ap);
		ret = execute(dbp, strTemp);
	}

	return ret;
}

int select2(struct libdb *dbp, struct rdset *rdsetp, const char *qcmd)
{
	if (!is_open(dbp) && !open(dbp))
	{
		return 0;
	}
	rdsetp->rd_db_type = dbp->db_type;
	int result = 0;
	const int type = dbp->db_type;
	switch (type)
	{
	case DB_ORACLE:
		{
			int ret = -1;
			if (NULL == rdsetp->rd_orap)
			{
				rdsetp->rd_orap = (rdset_oracle *)malloc(sizeof(struct rdset_oracle));
			}
			ret = sqlo_prepare(rdsetp->rd_orap->rd_orap, qcmd);
			if (ret < 0)
			{
				result = 0;
				set_error_d(dbp, "sqlo_prepare");
				break;
			}
		}
		break;
	case DB_MYSQL:
		{
			int ret = -1;
			ret = mysql_real_query(dbp->db_sqlp->mys_sqlp, qcmd, strlen(qcmd));
			if (0 != ret)
			{
				result = 0;
				set_error_d(dbp, "mysql_query");
				break;
			}
			if (NULL == rdsetp->rd_mysqlp)
				rdsetp->rd_mysqlp = (rdset_mysql *)malloc(sizeof(struct rdset_mysql));

			rdsetp->rd_mysqlp->rd_resp = mysql_store_result(dbp->db_sqlp->mys_sqlp);
			if (NULL == rdsetp->rd_mysqlp->rd_resp)
			{
				result = 0;
				set_error_d(dbp, "mysql_store_result");
				break;
			}
		}
		break;
	default:
		{
			result = 0;
			set_error_d(dbp, "the database type is not supported");
		}
		break;
	}
	return 0;
}

int select3(struct libdb *dbp, struct rdset *rdsetp, int len, const char *format, ...)
{
	int ret = 0;
	if (!is_open(dbp) && !open(dbp))
	{
		return 0;
	}
	va_list ap;
	va_start(ap, format);

	if (len < 2014)
	{
		char temp[1024];
		bzero(temp, 2014);
		vsnprintf(temp, len, format, ap);
		va_end(ap);
		ret = select2(dbp, rdsetp, temp);
	}
	else
	{
		char strTemp[len];
		vsnprintf(strTemp, len, format, ap);
		va_end(ap);
		ret = select2(dbp, rdsetp, strTemp);
	}

	return ret;
}

int is_eof(struct rdset *rdsetp)
{
	int result = 0;
	int type = rdsetp->rd_db_type;
	switch (type)
	{
	case DB_ORACLE:
		break;
	case DB_MYSQL:
		{
			if (rdsetp->rd_mysqlp->rd_resp == NULL)
				return 0;
			MYSQL_ROW *rowp = &rdsetp->rd_mysqlp->rd_rowp;
			if (mysql_eof(rdsetp->rd_mysqlp->rd_resp)) //it is always true when use mysql_store_result
			{
				*rowp = mysql_fetch_row(rdsetp->rd_mysqlp->rd_resp);
				if (*rowp)
				{
					result = 1;
					break;
				}
			}
		}
		break;
	default:
		break;
	}
	return result;
}

int get_record_count(struct rdset *rdsetp)
{
	int numbers = 0;
	int type = rdsetp->rd_db_type;
	switch (type)
	{
	case DB_ORACLE:
		{

		}
		break;
	case DB_MYSQL:
		{
			if (NULL != rdsetp->rd_mysqlp->rd_resp)
			{
				numbers = rdsetp->rd_mysqlp->rd_resp->field_count;
			}
		}
		break;
	}
	return numbers;
}

void get_index_value(struct rdset *rdsetp, const int index, char *value)
{
	int type = rdsetp->rd_db_type;
	switch (type)
	{
	case DB_ORACLE:
		{

		}
		break;
	case DB_MYSQL:
		{
			if (rdsetp->rd_mysqlp->rd_resp && rdsetp->rd_mysqlp->rd_rowp)
			{
				int num = rdsetp->rd_mysqlp->rd_resp->field_count;
				if (index < num)
				{
					int len = strlen(rdsetp->rd_mysqlp->rd_rowp[index]);
					if(NULL == value)
					{
						value = (char *)malloc(sizeof(char)*(len+1));
						bzero(value, len+1);
						memcpy(value, rdsetp->rd_mysqlp->rd_rowp[index], len);
					}
					else
					{
						value = (char *)realloc(value, sizeof(char)*(len+1));
						bzero(value, len+1);
						memcpy(value, rdsetp->rd_mysqlp->rd_rowp[index], len);
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

void get_field_value(struct rdset *rdsetp, const char *field, char *value)
{
	int type = rdsetp->rd_db_type;
	switch (type)
	{
	case DB_ORACLE:
		{
		}
		break;
	case DB_MYSQL:
		{
			MYSQL_RES *resp = rdsetp->rd_mysqlp->rd_resp;
			if (resp && rdsetp->rd_mysqlp->rd_rowp)
			{
				uint32_t i = 0;
				for (i = 0; i < resp->field_count; ++i)
				{
					int ret = -1;
					const char *name = resp->fields[i].name;
					int len = resp->fields[i].name_length;
					ret = memcmp(field, name, len);
					if (ret == 0)
					{
						if(NULL == value)
						{
							value = (char *)malloc(sizeof(char)*(len+1));
							bzero(value, len+1);
							memcpy(value, rdsetp->rd_mysqlp->rd_rowp[i], len);
						}
						else
						{
							value = (char *)realloc(value, sizeof(char)*(len+1));
							bzero(value, len + 1);
							memcpy(value, rdsetp->rd_mysqlp->rd_rowp[i], len);
						}
						break;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

int close_db(struct libdb *dbp)
{
	int dbType = dbp->db_type;
	int result = 0;
	switch (dbType)
	{
	case DB_ORACLE:
		{
			if (is_open(dbp))
			{
				int ret;
				ret = sqlo_finish(dbp->db_orap->ora_dbhp);
				if (SQLO_SUCCESS != ret)
				{
					result = 0;
					set_error_d(dbp, "shutdown oracle database is failure.");
					break;
				}
				free(dbp->db_orap);
				dbp->db_orap = NULL;
				result = 1;
			}
		}
		break;
	case DB_MYSQL:
		{
			if (is_open(dbp))
			{
				mysql_close(dbp->db_sqlp->mys_sqlp);
				free(dbp->db_sqlp);
				dbp->db_sqlp = NULL;
				result = 1;
			}
		}
		break;
	default:
		{
			result = 0;
			set_error_d(dbp, "the database type is not supported");
		}
		break;
	}
	return result;
}

int close_rdset(struct rdset *rdsetp)
{
	int type = rdsetp->rd_db_type;
	switch (type)
	{
	case DB_ORACLE:
		{

		}
		break;
	case DB_MYSQL:
		{
			if (NULL != rdsetp->rd_mysqlp->rd_resp)
			{
				mysql_free_result(rdsetp->rd_mysqlp->rd_resp);
				rdsetp->rd_mysqlp->rd_resp = NULL;
				return 1;
			}
		}
		break;
	}
	return 0;
}

void set_error_d(struct libdb *dbp, const char *str)
{
	if(NULL != dbp)
	{
		const char *errop=NULL;
		int type = dbp->db_type;
		switch(type)
		{
		case DB_ORACLE:errop = sql_geterror();                     break;
		case DB_MYSQL: errop = mysql_error(dbp->db_sqlp->mys_sqlp);break;
		}
		int len = strlen(str)+strlen(errop)+1;
		dbp->db_errorp.resize(len);
		snprintf(&dbp->db_errorp[0], len, "%s:%s", str, errop);
	}
}

char *get_last_error(const struct libdb *dbp)
{
	if(NULL != dbp)
	{
		return (char *)dbp->db_errorp.c_str();
	}
	return NULL;
}




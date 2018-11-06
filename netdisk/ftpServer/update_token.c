#include"head.h"
int update_token(char *name,char *token)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";
	char query[200]="update account set token='"; //update account set path='/dir1',curcode=1 where name='admin';
	sprintf(query,"%s%s%s%s%s",query,token,"' where name='",name,"'");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("update token success\n");
	}
	mysql_close(conn);
	return 0;
}

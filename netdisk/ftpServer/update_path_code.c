#include"head.h"
int update_path_code(char *name,char *path,int curcode)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";
	char query[200]="update account set path='"; //update account set path='/dir1',curcode=1 where name='admin';
	sprintf(query,"%s%s%s%d%s%s%s",query,path,"',curcode=",curcode," where name='",name,"'");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("update path success\n");
	}
	mysql_close(conn);
	return 0;
}

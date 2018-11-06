#include"head.h"
int delete_filedown(char *name,char *filename)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";
	char query[200]="delete from filedown where name='";
	sprintf(query,"%s%s%s%s%s",query,name,"' and filename='",filename,"'");
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
		printf("initialize filedown successfully\n");
	}
	mysql_close(conn);
	return 0;
}

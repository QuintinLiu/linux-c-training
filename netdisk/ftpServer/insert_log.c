#include"head.h"
int insert_log(char *ip,short port,char *name,char *act)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";
	char query[200]="insert into log(ip,port,id,operation) values";
	sprintf(query,"%s%s%s%s%d%s%s%s%s%s",query,"('",ip,"',",port,",'",name,"','",act,"')");
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
		printf("insert log success\n");
	}
	mysql_close(conn);
	return 0;
}

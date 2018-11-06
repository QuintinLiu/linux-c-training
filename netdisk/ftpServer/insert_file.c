#include"head.h"
int insert_file(char *name,int precode,char *filename,char *md5)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";
	char query[200]="insert into vdir(name,precode,filename,filetype,md5) values";
	sprintf(query,"%s%s%s%s%d%s%s%s%s%s%s%s",query,"('",name,"',",precode,",'",filename,"','","file","','",md5,"')");
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
		printf("puts file success\n");
	}
	mysql_close(conn);
	return 0;
}

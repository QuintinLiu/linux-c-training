#include"head.h"
int update_filedown(char *name,char *filename,off_t offset,off_t point)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";
	char query[200]="update filedown set point="; //update account set path='/dir1',curcode=1 where name='admin';
	sprintf(query,"%s%ld%s%s%s%s%s%ld",query,point," where name='",name,"' and filename='",filename,"' and offset=",offset);
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
		printf("update file_point success\n");
	}
	mysql_close(conn);
	return 0;
}

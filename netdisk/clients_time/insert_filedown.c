#include"head.h"
int insert_filedown(char *name,char *filename,off_t offset,off_t size,off_t point)
{
	MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";
	char query[200]="insert into filedown(name,filename,offset,size,point) values(";
	sprintf(query,"%s%s%s%s%s%s%ld%s%ld%s%ld%s",query,"'",name,"','",filename,"',",offset,",",size,",",point,")");
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
		printf("insert file_point success\n");
	}
	mysql_close(conn);
	return 0;
}

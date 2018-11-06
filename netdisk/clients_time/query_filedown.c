#include"head.h" 
int query_filedown(char *name,char *filename,off_t offset,off_t *point)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";//要访问的数据库名称
	char query[300]={0};
	strcpy(query,"select * from filedown where name=");
	sprintf(query,"%s%s%s%s%s%s%ld",query,"'",name,"' and filename='",filename,"' and offset=",offset);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		//	printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			row=mysql_fetch_row(res);
			if(row==NULL)
			{
				mysql_free_result(res);
				mysql_close(conn);
				return -1;
			}
			*point=atoi(row[3]);
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}

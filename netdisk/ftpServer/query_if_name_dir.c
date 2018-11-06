#include"head.h" 
int query_if_name_dir(char *name,char *filename)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="fengdou";
	char* database="test25";//要访问的数据库名称
	char query[300]={0};
	strcpy(query,"select * from vdir where name=");
	sprintf(query,"%s%s%s%s%s%s%s",query,"'",name,"'","and filename='",filename,"'");
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
			while((row=mysql_fetch_row(res))!=NULL)
			{
				if(strcmp(row[4],"dir")==0)
				{
					mysql_free_result(res);
					mysql_close(conn);
					return -1;
				}
			}
			mysql_free_result(res);
		}
		mysql_close(conn);
		return 0;
	}
}

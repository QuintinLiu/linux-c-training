#include"head.h" 
int query_file(int new_fd,char *name,int precode)
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
	sprintf(query,"%s%s%s%s%s%d",query,"'",name,"'"," and precode=",precode);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}
	t=mysql_query(conn,query);
	data d;
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		res=mysql_use_result(conn);
		if(res)
		{
			int r=0;
			while((row=mysql_fetch_row(res))!=NULL)
			{
				if(r==0)
				{
					r=1;
					strcpy(d.buf,"transfer");
					d.len=strlen(d.buf);
					send_n(new_fd,(char *)&d,4+d.len);
				}
				bzero(&d,sizeof(d));
				sprintf(d.buf,"%-30s%-8s",row[3],row[4]);
				d.len=strlen(d.buf);
				send_n(new_fd,(char *)&d,4+d.len);
				//发送文件总大小
				/*if(strcmp(row[4],"dir")==0)
				{
					bzero(&d,sizeof(d));
					off_t size=256;
					d.len=sizeof(off_t);
					memcpy(d.buf,&size,sizeof(off_t));
					ret=send_n(new_fd,(char *)&d,d.len+4);
				}
				else
				{
					bzero(&d,sizeof(d));
					sprintf(d.buf,"%s%s","../file_pool/",row[5]);
					int fd=open(d.buf,O_RDONLY);
					error_check(-1,fd,"open");
					struct stat buf;
					fstat(fd,&buf);
					bzero(&d,sizeof(d));
					d.len=sizeof(buf.st_size);
					memcpy(d.buf,&buf.st_size,sizeof(off_t));
					ret=send_n(new_fd,(char *)&d,d.len+4);
				}*/

			}
			bzero(&d,sizeof(d));
			if(r==0)
			{
				strcpy(d.buf,"empty");
				d.len=strlen(d.buf);
				send_n(new_fd,(char *)&d,4+d.len);
			}
			else 
			{
				d.len=0;
				send_n(new_fd,(char *)&d.len,4);
			}
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}

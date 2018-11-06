#include"work_que.h"
int que_insert(que_t *pque,node *pnew)
{
	if(pque->que_size==0)
	{
		pque->que_head=pnew;
		pque->que_tail=pnew;
	}
	else
	{
		pque->que_tail->next=pnew;
		pque->que_tail=pnew;
	}
	pque->que_size++;
	return 0;
}
int que_get(que_t *pque,node **pcur)
{
	if(pque->que_size==0)
	{
		*pcur=NULL;
		return -1;
	}
	if(pque->que_head->new_fd==-1)
	{
		*pcur=pque->que_head;
	}
	else
	{
		*pcur=pque->que_head;
		pque->que_head=pque->que_head->next;
		if(pque->que_head==NULL)
		{
			pque->que_tail=NULL;
		}
		pque->que_size--;
	}
	return 0;
}
int que_insert_exit(que_t*pque,node *pnew)
{
	if(!pque->que_size)
	{
		pque->que_head=pnew;
		pque->que_tail=pnew;
	}
	else
	{
		pnew->next=pque->que_head;
		pque->que_head=pnew;
	}
	pque->que_size++;
	return 0;
}

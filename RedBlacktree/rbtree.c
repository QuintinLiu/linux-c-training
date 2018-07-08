#include"fun.h"

tn *grandparent(tn *p)
{
	if(p->parent!=0)
	{
		return p->parent->parent;
	}
	else
		return p->parent;
}
tn *uncle(tn *p)
{
	if(p->parent!=0)
	{
		if(grandparent(p)!=0)
		{
			if(p->parent==grandparent(p)->left)
				return grandparent(p)->right;
			else
				return grandparent(p)->left;
		}
	}
	return 0;
}
tn *sibling(tn *p)
{
	if(p==p->parent->left)
		return p->parent->right;
	else
		return p->parent->left;
}
void rotate_right(tn *p)
{
	p->left->parent=p->parent;
	if(p->parent!=0)
	{
		if(p==p->parent->left)
			p->parent->left=p->left;
		else
			p->parent->right=p->left;
	}
	p->parent=p->left;
	p->left=p->parent->right;
	if(p->parent->right!=0)
	{
		p->parent->right->parent=p;
	}
	p->parent->right=p;
}
void rotate_left(tn *p)
{
	p->right->parent=p->parent;
	if(p->parent!=0)
	{
		if(p==p->parent->right)
			p->parent->right=p->right;
		else
			p->parent->left=p->right;
	}
	p->parent=p->right;
	p->right=p->parent->left;
	if(p->parent->left!=0)
	{
		p->parent->left->parent=p;
	}
	p->parent->left=p;
}
void insert_case_4(tn *tnew)
{//进行最后的左旋或者右旋，父亲与祖父交换颜色
	tnew->parent->color=black;
	grandparent(tnew)->color=red;
	if(tnew==tnew->parent->left&&tnew->parent==grandparent(tnew)->left)
	{
		rotate_right(grandparent(tnew));
	}
	else 
		rotate_left(grandparent(tnew));
}
void insert_case_3(tn *tnew)
{//进入这里则叔父可能没有，也有可能存在为黑色,此时都视为一种情况
	if(tnew==tnew->parent->right&&tnew->parent==grandparent(tnew)->left)
	{//进入这里则先左旋为case4中右旋做准备
		rotate_left(tnew->parent);
		tnew=tnew->left;
	}
	else if(tnew==tnew->parent->left&&tnew->parent==grandparent(tnew)->right)
	{//进入这里则先右旋为case4中左旋做准备
		rotate_right(tnew->parent);
		tnew=tnew->right;
	}
	insert_case_4(tnew);
}
void insert_case_2(tn *tnew)
{//进入这里说明父结点为红色，祖父必为黑色
	if(uncle(tnew)!=0&&uncle(tnew)->color==red)	//父亲和叔父（存在）都为红色，和祖父交换
	{											//颜色,再从祖父开始向上遍历
		tnew->parent->color=black;
		uncle(tnew)->color=black;
		grandparent(tnew)->color=red;
		insert_case_1(grandparent(tnew));
	}
	else
		insert_case_3(tnew);
}
void insert_case_1(tn *tnew)
{
	if(tnew->parent==0)	//插入结点为根结点,变为黑色，返回
	{
		tnew->color=black;
		return;
	}
	if(tnew->parent->color==black)	//插入结点的父亲颜色为黑色，直接返回
		return;
	else
		insert_case_2(tnew);
}
void insert(tn *tnew,tn *root)
{//找到插入的位置，并插入新节点，必定是叶子结点
	tn *twork;
	twork=root;
	while(twork!=0)
	{
		if(tnew->data<twork->data)
		{
			if(twork->left==0)
			{
				twork->left=tnew;
				tnew->parent=twork;
				break;
			}
			else
			{
				twork=twork->left;
			}
		}
		else 
		{
			if(twork->right==0)
			{
				twork->right=tnew;
				tnew->parent=twork;
				break;
			}
			else
			{
				twork=twork->right;
			}
		}
	}
	insert_case_1(tnew);
}
void delete_case_7(tn *p)
{//进入这里，兄弟靠外的孩子必为红色，则兄弟换成父亲颜色，父亲变为黑色，最后旋转父亲，结束
	tn *s=sibling(p);
	s->color=p->parent->color;
	p->parent->color=black;
	if(p=p->parent->left)
	{
		rotate_left(p->parent);
	}
	else 
		rotate_right(p->parent);
}
void delete_case_6(tn *p)
{//进入这里则说明兄弟必有孩子，且其中有红色孩子
	tn *s=sibling(p);
	if(p=p->parent->left)
	{
		if(s->left!=0&&s->left->color==red)
		{
			rotate_right(s);//p为父亲左孩子，兄弟左孩子为红色，先右旋，为case7左旋准备	
		}
	}
	else if(p=p->parent->right)
	{
		if(s->right!=0&&s->right->color==red)
		{
			rotate_left(s);//p为父亲右孩子，兄弟右孩子为红色，先左旋，为case7右旋准备
		}
	}
	delete_case_7(p);
}
void delete_case_5(tn *p)
{
	tn *s=sibling(p);
	if(p->parent->color==red)	//父亲为红色
	{
		if(s==0)
		{
			p->parent->color=black;
			return;
		}
		else if((s->right==0&&s->left==0)||
			!((s->left!=0&&s->left->color==red)||(s->right!=0&&s->right->color==red)))				//父亲为红色且兄弟（必为黑）无孩子或均为黑色，父亲兄弟换色，结束返回
		{
			p->parent->color=black;
			s->color=red;
			return;
		}
	}
	delete_case_6(p);
}
void delete_case_4(tn *p)
{
	tn *s=sibling(p);
	if(p->parent->color==black&&s->color==black)
	{									//父亲为黑色且兄弟也为黑色
		if((s->right==0&&s->left==0)||
			!((s->left!=0&&s->left->color==red)||(s->right!=0&&s->right->color==red)))		//兄弟无孩子或孩子都为黑色,兄弟变为红色，直接返回
		{
			s->color=red;
			delete_case_2(p->parent);
		}
	}
	delete_case_5(p);
}
void delete_case_3(tn *p)//p有父亲,先判断父亲为黑色情况
{
	tn *s=sibling(p);
	if(p->parent->color==black)
	{
		if(s==0)
		{
			delete_case_2(p->parent);
		}
		else if(s->color==red)//兄弟为red
		{
			s->color=black;
			p->parent->color=red;
			if(p=p->parent->left)
				rotate_left(p->parent);
			else rotate_right(p->parent);
		}
	}
	delete_case_4(p);
}
void delete_case_2(tn *p)
{
	if(p->parent!=0)	//p没有父亲直接返回，有父亲进入case3
		delete_case_3(p);
}
void delete_case_1(tn *p,tn **root)//p就是要删除的结点
{//进入这里要删除结点没有孩子或只有一个孩子
	tn *q;
	if(p->left==0&&p->right==0)	//要删除结点没有孩子,简单操作即可删除
	{
		if(p==*root)
		{
			*root=0;
			return;
		}
		if(p==p->parent->left)
		{
			p->parent->left=0;
			free(p);
			return;
		}
		else
		{
			p->parent->right=0;
			free(p);
			return;
		}
	}
	else if(p->left!=0)
	{
		if(p->parent!=0)
		{
			if(p==p->parent->left)
				p->parent->left=p->left;
			else
				p->parent->right=p->left;
			p->left->parent=p->parent;
		}
		else {
			p->left->parent=0;
			*root=p->left;
		}
		//到这一步结点已经删除	
		if(p->color==red||(p->color==black&&p->left->color==red))
		{								//删除结点为红或删除结点为黑且孩子为红，调整返回
			if(p->left->color==red)
				p->left->color=black;
			free(p);
			return;
		} 
		q=p;p=p->left;free(q);
	}
	else if(p->right!=0)
	{
		if(p->parent!=0)
		{
			if(p==p->parent->left)
				p->parent->left=p->right;
			else
				p->parent->right=p->right;
			p->right->parent=p->parent;
		}
		else {
			p->right->parent=0;
			*root=p->right;
		}
		//走到这一步就把结点已经删除了
		if(p->color==red||(p->color==black&&p->right->color==red))	//删除结点为红或删除
		{									//结点为黑且其孩子为红，调整返回
			if(p->right->color==red)
				p->right->color=black;
			free(p);
			return;
		}
		q=p;p=p->right;free(q);
	}
	delete_case_2(p);
}
void delete(int i,tn **root)
{//查找要删除的结点，可以进行替换值，使要删除的结点只有一个孩子或没有孩子
	tn *p=*root;
	while(p!=0)
	{
		if(i<p->data)
		{
			if(p->left==0)
			{
				printf("no such node\n");
				return;
			}
			else
				p=p->left;
		}
		else if(i>p->data)
		{
			if(p->right==0)
			{
				printf("no such node\n");
				return;
			}
			else 
				p=p->right;
		}
		else
		{
			if(p->left!=0&&p->right!=0)
			{
				tn *q=p;p=p->right;
				while(p->left!=0)
				{
					p=p->left;
				}
				swap(q->data,p->data);
			}
			delete_case_1(p,root);
			return;
		}
	}
}
void main()
{
	int aa[10]={8,4,13,6,16,7,1,9,15,10};
	tn *root;
	tn *tnew;
	int i;
	root=0;
	for(i=0;i<10;i++)
	{
		tnew=(tn *)calloc(1,sizeof(tn));
		tnew->color=red;
		tnew->data=aa[i];
		if(root==0)
		{
			root=tnew;
			root->color=black;
			continue;
		}
		else
			insert(tnew,root);
	}
	while(scanf("%d",&i)!=EOF)
	{
		delete(i,&root);
	}
	system("pause");
}
s
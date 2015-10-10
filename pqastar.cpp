
#include "pqastar.h"

Queue::Queue()
{
	header=nullptr;
	number=0;
}

Queue::~Queue()
{
	/*	2 parts memory leak:
	1. number will decrease while i increase, 
	i will easily greater than number but queue node have not delete completely
	2. Astar BFS will not empty the priority queue when it found the target.
	there are a lot of step node: nodeptr remain in the priority queue and no one will delete these nodeptr

	for(int i=0;i<number;i++) {
		DeQueue();
	}*/

	while (number > 0) {
		delete DeQueue();
	}
}

void Queue::EnQueue(nodeptr l)
{
	qptr p,n,prev;
	bool dupflag=false;
	bool addflag=false;
	n=new qnode;
	n->lptr=l;
	n->next=nullptr;

	if(header==nullptr)
	{
		header=n;
		number++;
	}
	else
	{
		p=header;
		prev=nullptr;
		while(p!=nullptr)
		{	if(p->lptr->info.x==l->info.x && p->lptr->info.y==l->info.y)
				{
				dupflag=true;
				if(p->lptr->info.f>l->info.f)
					{
					//delete p->lptr;
					//p->lptr=NULL;
					p->lptr=l;
					}
				delete n;
				break;
				}
			if(n->lptr->info.f<p->lptr->info.f)
			{
				if(p!=header)
				{
				n->next=p;
				prev->next=n;
				}
				else
				{
				header=n;
				n->next=p;
				}
				number++;
				addflag=true;
				break;
			}
			prev=p;
			p=p->next;
		}

		if(dupflag==false&&addflag==false)
		{
			prev->next=n;
			number++;
		}
	}


}

nodeptr Queue::DeQueue()
{
	nodeptr r;
	if(header==nullptr)
	{
		//MessageBox(NULL,"The Queue is empty and it cannot be popped!","Dequeue Error",MB_ICONERROR|MB_OK);
		r=nullptr;
		return r; 
	}
	else
	{
		qptr p=header;
		header=p->next;
		r=p->lptr;
		p->next=nullptr;
		delete p;
		p=nullptr;
		number--;
		return r;
	}
}

bool Queue::IsEmpty()
{
	if(number>0)
		return false;
	else
		return true;
}


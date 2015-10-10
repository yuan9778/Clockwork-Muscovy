#ifndef PQASTAR_H
#define PQASTAR_H

typedef struct
{
	int x;
	int y;
	double cost;
	double f;
}cell;

typedef struct node
{
	cell info;
	struct node* next;
}* nodeptr;


typedef struct qnode
{
	nodeptr lptr;//this is the information the queue stores
	struct qnode* next;
}* qptr;

class Queue
{
	private:
		qptr header;
		int number;
		
	public:
		Queue();
		~Queue();
		void EnQueue(nodeptr n);
		nodeptr DeQueue();
		bool IsEmpty();
};

#endif


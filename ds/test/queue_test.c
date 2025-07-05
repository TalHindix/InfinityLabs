#include <stdio.h>
#include "queue.h"

static void TestQueueCreateDestroy(void)
{
    queue_t *q = QueueCreate();
    printf("QueueCreate returned %p\n", (void *)q);
    QueueDestroy(q);
    printf("QueueDestroy completed\n");
}

static void TestQueueIsEmpty(void)
{
	{
		queue_t *q = QueueCreate();
		printf("QueueIsEmpty:(Expected Yes): ---------- %s\n",(QueueIsEmpty(q)) ? "Yes" : "No");
	}
	{
		int x = 5;
		queue_t* q = QueueCreate();
		QueueEnqueue(q,(void *)&x);
		printf("QueueIsEmpty:(Expeceted No): -------  %s\n",(QueueIsEmpty(q)) ? "Yes" : "No");
	}
}

static void TestQueueSize(void)
{
	{
		queue_t *q = QueueCreate();
		printf("QueueSize:(Expected 0): ---------- %lu\n",QueueSize(q));
		QueueDestroy(q); 
	}
	{
		int x = 5;
		queue_t* q = QueueCreate();
		QueueEnqueue(q,(void *)&x);
		printf("QueueSize:(Expected 1): ---------- %lu\n",QueueSize(q));
		QueueDestroy(q); 
	}
	{
		int x = 5;
		int y = 3;
		int z = 2;
		queue_t* q = QueueCreate();
		QueueEnqueue(q,(void *)&x);
		QueueEnqueue(q,(void *)&y);
		QueueEnqueue(q,(void *)&z);
		printf("QueueSize:(Expected 3): ---------- %lu\n",QueueSize(q));
		QueueDestroy(q); 
	}
}

static void TestQueuePeek(void)
{
    int x = 5;
    queue_t *q = QueueCreate();
    QueueEnqueue(q, &x);
    printf("QueuePeek: (expected 5): ---------- %d\n", *(int *)QueuePeek(q));
    QueueDestroy(q);   
}

static void TestQueueEnqueueDequeue()
{
	int x = 10;
	int y = 20;
    queue_t *q = QueueCreate();
    QueueEnqueue(q, &x);
    QueueEnqueue(q, &y);
   	printf("QueuePeek: (expected 10): ---------- %d\n", *(int *)QueuePeek(q));  /* 10 */
	QueueDequeue(q);
	printf("QueuePeek: After Dequeue (expected 20): ---------- %d\n", *(int *)QueuePeek(q));  /* 20 */
}

static void TestQueueAppend()
{
	int x = 5;
	int y = 3;
	int z = 2;
	
	int a = 1;
	int b = 2;
	queue_t* q1 = QueueCreate();
	queue_t* q2 = QueueCreate();
	QueueEnqueue(q1,(void *)&x);
	QueueEnqueue(q1,(void *)&y);
	QueueEnqueue(q1,(void *)&z);
	
	QueueEnqueue(q2,(void *)&a);
	QueueEnqueue(q2,(void *)&b);
	
	printf("before append - QueueSize:(Expected 3): ---------- %lu\n",QueueSize(q1));
	printf("before append - QueueSize:(Expected 2): ---------- %lu\n",QueueSize(q2));
	
	QueueAppend(q1,q2);
	printf("After append - QueueSize:(Expected 5): ---------- %lu\n",QueueSize(q2));
	printf("After append - QueueSize:(Expected 0): ---------- %lu\n",QueueSize(q1));

}







void TestQueue(void)
{
    TestQueueCreateDestroy();
    TestQueueIsEmpty();
    TestQueueSize();
    TestQueuePeek();
    TestQueueEnqueueDequeue();
    TestQueueAppend();
}



int main()
{

	TestQueue();

    return 0;
}

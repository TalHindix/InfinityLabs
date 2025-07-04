#include <stdio.h>
#include "sll.h"
#include "queue.h"

#define SUCCESS 1
#define FAILURE 0


static int PrintElement(void *data, void *unused)
{
    (void)unused;
    printf("%d -> ", *(int *)data);
    return 0;
}

static int CompareInt(const void *data, void *target)
{
    return (*(int *)data == *(int *)target);
}

static void ShowResult(const char *desc, int expected, int actual)
{
    printf("%s | Expected: %d | Actual: %d | %s\n", desc, expected, actual,(expected == actual) ? "SUCCESS" : "FAILURE");
}


static void TestSLLAppend()
{
    sll_t *src = NULL;
    sll_t *dst = NULL;
    sll_iter_t iter = NULL;

    int a = 1, b = 2, c = 3;
    int d = 10, e = 20; 
    int expected_after[] = {10, 20, 1, 2, 3};
    size_t i = 0;
    int ok = SUCCESS;
	
	printf("\n==X Append SLL X==\n");
    /* build src list: 1 -> 2 -> 3 */
    src  = SLLCreate();
    iter = SLLBegin(src);
    iter = SLLInsert(iter, &a);            
    iter = SLLNext(iter);
    iter = SLLInsert(iter, &b);              
    SLLInsert(SLLNext(iter), &c);          

    /* build dst list: 10 -> 20 */
    dst  = SLLCreate();
    
    iter = SLLBegin(dst);
    iter = SLLInsert(iter, &d);          
    SLLInsert(SLLNext(iter), &e);
	
    ShowResult("Src count before append", 3, SLLCount(src));
    ShowResult("Dst count before append", 2, SLLCount(dst));

    SLLAppend(src, dst);

    ShowResult("Dst count after append", 5, SLLCount(dst));


    iter = SLLBegin(dst);
    for (i = 0; i < 5; ++i, iter = SLLNext(iter))
    {
        if (iter == SLLEnd(dst) || *(int *)SLLGetData(iter) != expected_after[i])
        {
            ok = FAILURE;
            break;
        }
    }
    
    ShowResult("Order validation", SUCCESS, ok);

    printf("Dst list after append: ");
    SLLForEach(SLLBegin(dst), SLLEnd(dst), PrintElement, NULL);
    printf("(E)NULL\n");

    SLLDestroy(src);
}

static void TestSLL()
{
    sll_t *list = NULL;
    sll_iter_t iter = NULL;
    sll_iter_t result = NULL;

    int x = 5, y = 15, z = 25, w = 35, target = 15;
    int total = 0;
	
    printf("\n==X Create and Insert X==\n");
    list = SLLCreate();
    iter = SLLBegin(list);
    iter = SLLInsert(iter, &x);      
    SLLInsert(SLLNext(iter), &y);       
    SLLInsert(SLLEnd(list), &z);
    
    total = SLLCount(list);
    ShowResult("Total Nodes after Insert", 3, total);

    printf("Current Single list: ");
    SLLForEach(SLLBegin(list), SLLEnd(list), PrintElement, NULL);
    printf(" (E)\n");

    printf("\n==X GetData and SetData X==\n");
    iter = SLLBegin(list);
    ShowResult("First node (GetData):", x, *(int *)SLLGetData(iter));

    SLLSetData(iter, &w);
    ShowResult("First node after SetData:", w, *(int *)SLLGetData(iter));

    printf("\n==X Find an node X==\n");
    result = SLLFind(SLLBegin(list), SLLEnd(list), CompareInt, &target);
    if (result != SLLEnd(list))
    {
        ShowResult("Found target value:", target, *(int *)SLLGetData(result));
    }
    else
    {
        printf("Find result: Not Found (FAILURE)\n");
    }

    printf("\n==X Remove Node X==\n");
    SLLRemove(SLLBegin(list));
    total = SLLCount(list);
    ShowResult("Nodes after one removal", 2, total);

    printf("Single List now: ");
    SLLForEach(SLLBegin(list), SLLEnd(list), PrintElement, NULL);
    printf("NULL\n");

    printf("\n==X Final Clean-Up Test X==\n");
    SLLRemove(SLLBegin(list));
    SLLRemove(SLLBegin(list));

    ShowResult("IsEmpty Func check", 1, SLLIsEmpty(list));
    ShowResult("Final count check", 0, SLLCount(list));

    SLLDestroy(list);
    printf("Single List successfully destroyed.\n");
   
}


static void TestQueueCreateDestroy(void)
{
    queue_t *q = QueueCreate();

    printf("QueueCreate returned %p\n", (void *)q);
    QueueDestroy(q);
    printf("QueueDestroy completed\n");
}

static void TestQueueIsEmpty()
{
	/* Empty Queue */
	{
		queue_t *q = QueueCreate();
		printf("QueueIsEmpty:(Expected Yes): ---------- %s\n",(QueueIsEmpty(q)) ? "Yes" : "No");
	}
	/* 1 Node in queue */
	{
		int x = 5;
		queue_t* q = QueueCreate();
		QueueEnqueue(q,(void *)&x);
		printf("QueueIsEmpty:(Expeceted No): -------  %s\n",(QueueIsEmpty(q)) ? "Yes" : "No");
	}
}

static void TestQueueSize()
{
	/* Empty Queue */
	{
		queue_t *q = QueueCreate();
		printf("QueueSize:(Expected 0): ---------- %lu\n",QueueSize(q));
		QueueDestroy(q); 
	}
	/* 1 Node in queue */
	{
		int x = 5;
		queue_t* q = QueueCreate();
		QueueEnqueue(q,(void *)&x);
		printf("QueueSize:(Expected 1): ---------- %lu\n",QueueSize(q));
		QueueDestroy(q); 
	}
	
	/* 1 Node in queue */
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

    TestSLL();
    TestSLLAppend();
	TestQueue();

    return 0;
}


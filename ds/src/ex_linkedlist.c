

typedef struct node
{
	void* data;
	struct node* next;
} node_t;

node_t* Flip(node_t *head);
int HasLoop(const node_t *head);
node_t* FindIntersection(node_t* head_1,node_t head_2);


int main(void)
{
	return 0;
}


node_t* Flip(node_t *head)
{
	node_t* curr = head;
	node_t* prev = NULL;
	node_t* next = NULL;
	
	while(NULL != curr)
	{
		next = curr->next;		
		curr->next = prev;
		
		prev = curr;
		curr = next;		
	}
	
	return prev;
}

int HasLoop(const node_t *head);
{
	node_t* slow = head;
	node_t* fast = head;
	
	while(slow && fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
		
		
		if(slow == fast)
		{
			return 1;
		}
	}
	
	return 0;	
}

node_t* FindIntersection(node_t* head_1,node_t head_2)
{
	while (NULL != head_2)
	{
		node_tmp* tmp = head_1;
		while(NULL != tmp)
		{
			if(tmp == head_2)
			{
				return tmp;
			}
			tmp = tmp->next;
		}
		head_2 = head_2->next;		
	}
	return NULL;
}

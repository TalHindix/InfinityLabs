#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

int JosephusWithArray(size_t soldiers_count)
{

	int *arr = (int *)malloc(soldiers_count * sizeof(int));
    size_t  soldiers_alive = soldiers_count;
    size_t  sword_holder   = 0;
    size_t  i;
    size_t next_victim;


 	if(NULL == arr)
 	{
 		return -1;
 	}
 	
 	for (i=0; i < soldiers_count; ++i)
 	{
 		arr[i] = i + 1;
 	} 
    
 	while (soldiers_alive > 1)
 	{
 		next_victim = (sword_holder + 1) % soldiers_alive; /* Its a circle */
 		
 		for (i = next_victim; i < soldiers_alive - 1; ++i) /* Loop to delete */
 		{
 			arr[i] = arr[i+1];
 		}
 		
 		--soldiers_alive;
 		
 		
 		sword_holder = next_victim; /* Pass the sword */
 		
 	}
 	
 	i = arr[0];
	free(arr);
	return i;	
}



int main()
{
	size_t num = JosephusWithArray(SIZE);
	printf("The winner of size %u is: %lu\n",SIZE,num);
	return 0;
}

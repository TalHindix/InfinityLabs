#include <stdio.h>
#include <assert.h>

#define COLS 5


void SumRows(int a[][COLS], size_t rows, int res[])
{
	size_t i = 0;
	size_t j = 0;
	int sum_row;
	
	printf("the sizeof() of the recieved array is:%lu\n",sizeof(a)[COLS]);
	
	assert(a != NULL);
	assert(res != NULL);
	
	for (i = 0 ; i < rows ; i++)
	{
		sum_row = 0;		
		for (j = 0; j < COLS ; j++)
		{
			sum_row += a[i][j];
		}
		res[i] = sum_row;
	}
	
}

int main()
{
	
	int matrix[3][COLS] = {{1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15}};
	int sum[3]= {0};
	int i;
	
	SumRows(matrix,3,sum);
	
	for (i = 0 ; i < 3 ; i++)
	{
		printf("Sum row %d: = %d!\n",i,sum[i]);
	}
	
	return 0;
}

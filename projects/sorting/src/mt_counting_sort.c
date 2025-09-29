/**************************************
Exercise: 	SA - Multithreaded Counting Sort
Date:		29/9/2025
Developer:	Tal Hindi
Reviewer: 	Guy Argaman
Status:		Wait For Review
**************************************/

#include <pthread.h> /* pthread_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* FILE */

#include "mt_counting_sort.h"   /* MTCountingSort */

#define ASCII (256)

typedef struct
{
	char* arr;
	size_t start_index;
	size_t end_index;
	size_t* global_count;
	pthread_mutex_t* lock;
} thread_data_t;

static void* CountSegment(void* arg);

int MTCountingSort(char* arr, size_t length, size_t num_of_threads)
{
	size_t global_count[ASCII] = {0};
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	thread_data_t* thread_data = NULL;
	pthread_t* threads = NULL;
	size_t segment = length / num_of_threads;
	size_t i = 0;
	size_t j = 0;
	size_t pos = 0;

	assert(NULL != arr);
	assert(0 < length);
	assert(0 < num_of_threads);

	thread_data = malloc(sizeof(thread_data_t) * num_of_threads);
	threads = malloc(sizeof(pthread_t) * num_of_threads);
	if (NULL == thread_data || NULL == threads)
	{
		free(thread_data);
		free(threads);
		return -1;
	}

	for (i = 0; i < num_of_threads; ++i)
	{
		thread_data[i].arr = arr;
		thread_data[i].start_index = i * segment;
		thread_data[i].end_index = (i == num_of_threads - 1) ? length : (i + 1) * segment;
		thread_data[i].global_count = global_count;
		thread_data[i].lock = &lock;

		if (0 != pthread_create(&threads[i], NULL, &CountSegment, &thread_data[i]))
		{
			pthread_mutex_destroy(&lock);
			free(threads);
			free(thread_data);
			return -1;
		}
	}

	for (i = 0; i < num_of_threads; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	for (i = 0; i < ASCII; ++i)
	{
		for (j = 0; j < global_count[i]; ++j)
		{
			arr[pos++] = (char)i;
		}
	}

	pthread_mutex_destroy(&lock);
	free(threads);
	free(thread_data);

	return 0;
}

static void* CountSegment(void* arg)
{
	thread_data_t* data = (thread_data_t*)arg;
	size_t local_count[ASCII] = {0};
	size_t i = 0;
	
	for (i = data->start_index; i < data->end_index; ++i)
	{
		++local_count[(unsigned char)data->arr[i]];
	}

	for (i = 0; i < ASCII; ++i)
	{
		if (0 != local_count[i])
		{
			pthread_mutex_lock(data->lock);
			data->global_count[i] += local_count[i];
			pthread_mutex_unlock(data->lock);
		}
	}
	
	return NULL;
}

char* ReadDictionary(size_t* length)
{
	char dir[] = "/usr/share/dict/words";
	FILE* fp = NULL;
	char* buffer = NULL;
	size_t bytes_read = 0;

	assert(NULL != length);

	fp = fopen(dir, "r");
	if (NULL == fp)
	{
		perror("fopen");
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	*length = ftell(fp);
	rewind(fp);

	buffer = malloc(*length + 1);
	if (NULL == buffer)
	{
		perror("malloc");
		fclose(fp);
		return NULL;
	}

	bytes_read = fread(buffer, 1, *length, fp);
	if (bytes_read != *length)
	{
		perror("fread");
		free(buffer);
		fclose(fp);
		return NULL;
	}

	buffer[*length] = '\0';
	fclose(fp);

	return buffer;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 18:40:39 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/14 18:35:26 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <strings.h>
#include <time.h>

//https://www.ibm.com/docs/pl/i/7.5.0?topic=threads-example-using-mutexes-in-pthread-programs
// https://www.geeksforgeeks.org/linux-unix/mutex-lock-for-linux-thread-synchronization/


// typedef	struct s_data
// {
// 	pthread_mutex_t *mutex;
// 	int mails;
// } t_data;

// void *routine(void *arg)
// {
// 	t_data *data;
// 	data = (t_data *)arg;
// 	int	i;

// 	i = 0;
// 	while (i < 1000000)
// 	{
// 		pthread_mutex_lock(data->mutex);
// 		i++;
// 		(data->mails)++;
// 		pthread_mutex_unlock(data->mutex);
// 	}
// }

// int	main(int argc, char *argv[])
// {
// 	t_data data;
// 	bzero(&data, sizeof(t_data));
// 	pthread_t t1;
// 	pthread_t t2;
// 	pthread_mutex_t mutex;

// 	pthread_mutex_init(&mutex, NULL);
// 	data.mutex = &mutex;
// 	if (pthread_create(&t1, NULL, &routine, &data) != 0)
// 		return (1);
// 	if (pthread_create(&t2, NULL, &routine, &data) != 0)
// 		return (1);
// 	if (pthread_join(t1, NULL) != 0)
// 		return (1);
// 	if (pthread_join(t2, NULL) != 0)
// 		return (1);
// 	pthread_mutex_destroy(&mutex);
// 	printf("Number of mails: %d\n", data.mails);
// 	return (0);
// }


//GET 	RETURN VALUE FROM THREAD

// void *roll_dice()
// {
// 	int	*value = malloc(sizeof(int));
// 	*value = ((rand() % 6) + 1);
// 	// printf("Value: %d\n", value);
// 	return ((void*) value);
// }

// int 	main(int argc, char *argv[])
// {
// 	pthread_t th;
// 	int *res;
	
// 	srand(time(NULL));
// 	if (pthread_create(&th, NULL, &roll_dice, NULL) != 0)
// 		return (1);
// 	if (pthread_join(th, (void **) &res) != 0)
// 		return (2);	

// 	printf("Result %d\n", (int) (*res));
// 	free (res);
// 	return (0);
// }

// // 	how to pass args to threads


// int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

// void *routine(void* arg)
// {
// 	t_data *data = (t_data *) arg;
// 	pthread_mutex_lock(data->mutex);
// 	printf("i: %d, prime: %d\n", data->mails, primes[data->mails]);
// 	pthread_mutex_unlock(data->mutex);
// }

// int	main(int argc, char *argv[])
// {
// 	pthread_t th[10];
// 	int i;
// 	t_data	*data;
// 	pthread_mutex_t mutex;

// 	data = malloc(10 * sizeof(t_data));
// 	pthread_mutex_init(&mutex, NULL);
// 	i = 0;
// 	while (i < 10)
// 	{	
// 		data[i].mails = i;
// 		data[i].mutex = &mutex;
// 		if (pthread_create(&th[i], NULL, &routine, &data[i]) != 0)
// 			perror("Failed to create thread");
// 		i++;
// 	}
// 	i = 0;
// 	while (i < 10)
// 	{
// 		if (pthread_join(th[i], NULL) != 0)
// 			perror("Failed to join thread");
// 		i++;
// 	}
// 	pthread_mutex_destroy(&mutex);
// 	free(data);
// }

// detached threads

// #define THREAD_NUM 2

// void *routine(void *args)
// {
// 	sleep(1);
// 	printf("Finished execution\n");
// }

// int main(int argc, char *argv[])
// {
// 	pthread_t th[THREAD_NUM];
// 	int i;

// 	i = 0; 
// 	while (i < THREAD_NUM)
// 	{
// 		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
// 			perror("Failed to create thread");
// 		pthread_detach(th[i]);
// 		i++;
// 	}
// 	// i = 0;
// 	// while (i < THREAD_NUM)
// 	// {
// 	// 	if (pthread_join(th[i], NULL) != 0)
// 	// 		perror("Failed to join thread");
// 	// 	i++;
// 	// }
// 	pthread_exit(0);
// }
#include "../includes/philo.h"
int	get_time(long time);

int	main(void)
{
	struct timeval start;
	struct timeval end;
	long	time_elapsed;

	gettimeofday(&start, NULL);
	// usleep(10000000);
	get_time(120);
	gettimeofday(&end, NULL);
	time_elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + ((end.tv_usec - start.tv_usec));

	printf("%ld\n", time_elapsed);
}
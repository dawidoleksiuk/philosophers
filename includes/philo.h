/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:51 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/12 20:58:54 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_PHILO 200

typedef struct s_philo t_philo;
 
typedef struct s_data
{
	long num_of_philos;
	long time_to_die;
	long time_to_eat;
	long time_to_sleep;
	long num_of_times_to_eat;
	pthread_mutex_t mutex_print;
	pthread_mutex_t mutex_deathcheck;
	pthread_mutex_t mutex_time_elapsed;
	pthread_t checking_thread;
	struct timeval start;
	struct timeval current_time;
	long	time_elapsed;
	int	someone_died;
	t_philo *philo_array;
} t_data;

struct s_philo
{
	int	philo_id;
	pthread_t thread_id;
	pthread_mutex_t mutex_fork;
	pthread_mutex_t mutex_deathtime;
	long	death_time;
	int	eat_count;
	t_data *data;
	int first_fork;
	int second_fork;
};

//init.c
int	data_validation(int argc, char *argv[]);
int	data_init(int argc, char *argv[], t_data *data, t_philo **philo_array);

//utils.c

//clean_exit
void	clean_exit(t_data *data, t_philo *philo_array);

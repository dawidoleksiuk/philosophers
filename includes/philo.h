/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:51 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/07 20:55:58 by doleksiu         ###   ########.fr       */
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

typedef struct s_data
{
	long num_of_philos;
	long time_to_die;
	long time_to_eat;
	long time_to_sleep;
	long num_of_times_to_eat;
	long	time_elapsed;
	pthread_mutex_t *mutex_print;
} t_data;

typedef struct s_philo
{
	int	philo_id;
	pthread_t thread_id;
	pthread_mutex_t *mutex_fork;
	long	death_time;
	int	eat_count;
	t_data *data;
} t_philo;

//init.c
int	data_validation(int argc, char *argv[]);
int	data_init(int argc, char *argv[], t_data *data);

//utils.c
int	get_time(long time);
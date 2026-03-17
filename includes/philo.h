/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:51 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/17 20:34:55 by doleksiu         ###   ########.fr       */
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
	pthread_t death_th;
	struct timeval start;
	struct timeval current_time;
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
	int fork_one;
	int fork_two;
};

//simulation.c
void	sleeping(t_data *data, int philo_id);
void	eating(t_data *data, t_philo *philo_array, int philo_id);
void	*philo_simulation(void *arg);
void	*death_checker(void *arg);

//init.c
int	run_simulation(t_data *data, t_philo *philo_array);
int	data_validation(int argc, char *argv[]);
int	data_init(int argc, char *argv[], t_data *data, t_philo **philo_array);

//utils.c
void	ft_sleep(long time);
int	str_to_int(char *str, long *arg);
int	data_validation(int argc, char *argv[]);
int	print(t_data *data, int philo_id, char *str);

//clean_exit
void	clean_exit(t_data *data, t_philo *philo_array);


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:51 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/23 22:38:48 by doleksiu         ###   ########.fr       */
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

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				stop_simulation;
	long long		num_of_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		num_of_times_to_eat;
	struct timeval	start_time;
	pthread_t		death_th;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_stop;
	t_philo			*philo_array;
}				t_data;

struct s_philo
{
	int				philo_id;
	pthread_t		thread_id;
	int				eat_count;
	long long		death_time;
	pthread_mutex_t	mutex_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	mutex_deathtime;
	t_data			*data;
};

//simulation.c
void		sleeping(t_data *data, int philo_id);
void		eating(t_data *data, t_philo *philo_array, int philo_id);
void		*philo_routine(void *arg);
void		*monitor_death(void *arg);

//init.c
int			run_simulation(t_data *data, t_philo *philo_array);
int			data_validation(int argc, char *argv[]);
int			init_program_data(int argc, char *argv[],
				t_data *data, t_philo **philo_array);

//utils.c
void		ft_sleep_ms(long long time);
long long	get_current_time(t_data *data);
int			str_to_int(char *str, long long *arg);
int			data_validation(int argc, char *argv[]);
int	check_stop(t_data *data);
int			print_state(t_data *data, int philo_id, char *str);

//clean_exit
void		clean_exit(t_data *data, t_philo *philo_array);

//monitor.c
void		*monitor_routine(void *arg);

//actions.c
int			philo_sleep(t_data *data, int philo_id);
void		philo_eat(t_data *data, t_philo *philo_array, int philo_id);
int			philo_think(t_data *data, int philo_id);

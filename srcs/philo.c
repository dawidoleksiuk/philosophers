/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:34 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/14 14:37:25 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print(t_data *data, int philo_id, char *str)
{
	struct timeval current_time;
	long time;
	int dead;

	

	pthread_mutex_lock(&data->mutex_deathcheck);
	dead = data->someone_died;
	pthread_mutex_unlock(&data->mutex_deathcheck);
	pthread_mutex_lock(&data->mutex_print);
	if (!dead)
	{
		gettimeofday(&current_time, NULL);
		time = ((current_time.tv_sec - data->start.tv_sec) * 1000) + ((current_time.tv_usec - data->start.tv_usec) / 1000);
		printf("%ld %d %s\n", time, philo_id, str);
	}
	pthread_mutex_unlock(&data->mutex_print);
}	

void	sleeping(t_data *data, int philo_id)
{
	print(data, philo_id, "is sleeping");
	get_time(data->time_to_sleep);
}

void	eating(t_data *data, t_philo *philo_array, int philo_id)
{
	int i;
	long time;
	struct timeval current_time;

	
	i = philo_id - 1;
	if (data->num_of_philos == 1)
	{
		pthread_mutex_lock(&philo_array[i].mutex_fork);
		print(data, philo_id, "has taken a fork");
		get_time(data->time_to_die);
		pthread_mutex_unlock(&philo_array[i].mutex_fork);
		return ;
	}
	pthread_mutex_lock(&philo_array[philo_array[i].first_fork].mutex_fork);
	print(data, philo_id, "has taken a fork");
	pthread_mutex_lock(&philo_array[philo_array[i].second_fork].mutex_fork);
	print(data, philo_id, "has taken a fork");
	pthread_mutex_lock(&data->mutex_death_time);
	gettimeofday(&current_time, NULL);
	time = ((current_time.tv_sec - data->start.tv_sec) * 1000) + ((current_time.tv_usec - data->start.tv_usec) / 1000);
	philo_array[i].death_time = time + data->time_to_die;
	pthread_mutex_unlock(&data->mutex_death_time);
	print(data, philo_id, "is eating");
	get_time(data->time_to_eat);
	pthread_mutex_unlock(&philo_array[philo_array[i].second_fork].mutex_fork);
	pthread_mutex_unlock(&philo_array[philo_array[i].first_fork].mutex_fork);
	philo_array[i].eat_count++;
}

void	*philo_simulation(void *arg)
{
	t_philo *philo;
	t_data *data;
	int time_to_think;
		long time;
	struct timeval current_time;

	
	philo = (t_philo *) arg;
	data = philo->data;
	if (philo->philo_id % 2 == 0)
		get_time(data->time_to_eat * 0.5);
	if (philo->philo_id == data->num_of_philos && data->num_of_philos % 2 != 0)
		get_time(data->time_to_eat * 0.8);
	while (1)
	{

		eating(data, data->philo_array, philo->philo_id);
		if (data->num_of_philos == 1)
			break ;
		pthread_mutex_lock(&data->mutex_deathcheck);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->mutex_deathcheck);
			break ;
		}
		pthread_mutex_unlock(&data->mutex_deathcheck);
		sleeping(data, philo->philo_id);
		pthread_mutex_lock(&data->mutex_deathcheck);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->mutex_deathcheck);
			break ;
		}
		pthread_mutex_unlock(&data->mutex_deathcheck);
		pthread_mutex_lock(&data->mutex_death_time);
		gettimeofday(&current_time, NULL);
		time = ((current_time.tv_sec - data->start.tv_sec) * 1000) + ((current_time.tv_usec - data->start.tv_usec) / 1000);
		time_to_think = ((data->time_to_die - (philo->death_time - time)) * 0.5);
		pthread_mutex_unlock(&data->mutex_death_time);
		print(data, philo->philo_id, "is thinking");
		get_time(time_to_think);
		pthread_mutex_lock(&data->mutex_deathcheck);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->mutex_deathcheck);
			break ;
		}
		pthread_mutex_unlock(&data->mutex_deathcheck);
	}
	return (0);
}


void	*death_checker(void *arg)
{
	struct timeval current_time;
	t_philo *philo_array;
	t_data *data;
	int i;
	int	died;
	long time;

	philo_array = (t_philo *) arg;
	data = philo_array[0].data;
	i = 0;
	died = 0;
	while (1)
	{
		if (i == data->num_of_philos)
		{
			get_time(2);
			// usleep(5000);
			i = 0;
		}
		pthread_mutex_lock(&data->mutex_death_time);
		gettimeofday(&current_time, NULL);
		time = ((current_time.tv_sec - data->start.tv_sec) * 1000) + ((current_time.tv_usec - data->start.tv_usec) / 1000);
		if (philo_array[i].death_time != 0 && philo_array[i].death_time <= time)
		{
			died = 1;
			break ; 
		}
		pthread_mutex_unlock(&data->mutex_death_time);
		i++;
	}
	pthread_mutex_unlock(&data->mutex_death_time);
	if (died)
	{
		pthread_mutex_lock(&data->mutex_deathcheck);
		data->someone_died = died;
		pthread_mutex_unlock(&data->mutex_deathcheck);
		pthread_mutex_lock(&data->mutex_print);
		printf("%ld %d died\n", time, philo_array[i].philo_id);
		pthread_mutex_unlock(&data->mutex_print);
	}
	return (0);
}

int	run_simulation(t_data *data, t_philo *philo_array)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_create(&philo_array[i].thread_id, NULL, &philo_simulation, &philo_array[i]) != 0)
 			return (1);
		i++;
	}
	if (pthread_create(&data->checking_thread, NULL, &death_checker, philo_array) != 0)
 		return (1);
	// printf("sim: %ld\n", data->time_elapsed);
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_join(philo_array[i].thread_id, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_join(data->checking_thread, NULL) != 0)
			return (1);
	return (0);
}



int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo *philo_array;

	philo_array = NULL;
	if (data_validation(argc, argv) != 0 || data_init(argc, argv, &data, &philo_array) != 0)
	{
		if (philo_array)
			clean_exit(&data, philo_array);
		return (1);
	}
	// pthread_mutex_lock(&data.mutex_deathcheck);
	// pthread_mutex_unlock(&data.mutex_deathcheck);
	// printf("%ld\n", data.num_of_philos);
	// printf("%ld\n", data.time_to_die);
	// printf("%ld\n", data.time_to_eat);
	// printf("%ld\n", data.time_to_sleep);
	// printf("%ld\n", data.num_of_times_to_eat);
	// printf("%d\n", philo_array[3].philo_id);
	// if (run_simulation(&data, philo_array) == 1)
	// 	free(philo_array);
	run_simulation(&data, philo_array);
	clean_exit(&data, philo_array);
}

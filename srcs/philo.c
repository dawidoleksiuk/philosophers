/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:34 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/11 20:43:13 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print(t_data *data, int philo_id, char *str)
{
	int	died;


	pthread_mutex_lock(&data->mutex_deathcheck);
	died = data->someone_died;
	pthread_mutex_unlock(&data->mutex_deathcheck);
	pthread_mutex_lock(&data->mutex_print);
	pthread_mutex_lock(&data->mutex_time_elapsed);
	if (!died)
		printf("%ld %d %s\n", data->time_elapsed, philo_id, str);
	pthread_mutex_unlock(&data->mutex_print);
	pthread_mutex_unlock(&data->mutex_time_elapsed);
}	

void	sleeping(t_data *data, int philo_id)
{
	print(data, philo_id, "is sleeping");
	usleep(data->time_to_sleep * 1000);
}

void	eating(t_data *data, t_philo *philo_array, int philo_id)
{
	int i;	
	i = philo_id - 1;
	if (philo_id % 2 == 0)
	{
		pthread_mutex_lock(&philo_array[i].mutex_fork);
		print(data, philo_id, "has taken a fork");
	}
	if (philo_id == data->num_of_philos)
		pthread_mutex_lock(&philo_array[0].mutex_fork);
	else if (data->num_of_philos != 1)
		pthread_mutex_lock(&philo_array[i + 1].mutex_fork);
	print(data, philo_id, "has taken a fork");
	if (philo_id % 2 != 0)
	{
		pthread_mutex_lock(&philo_array[i].mutex_fork);
		print(data, philo_id, "has taken a fork");
	}
	pthread_mutex_lock(&philo_array[i].mutex_deathtime);
	pthread_mutex_lock(&data->mutex_time_elapsed);
	philo_array[i].death_time = data->time_elapsed + data->time_to_die;
	pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
	pthread_mutex_unlock(&data->mutex_time_elapsed);
	print(data, philo_id, "is eating");
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&philo_array[i].mutex_fork);
	if (philo_id == data->num_of_philos)
		pthread_mutex_unlock(&philo_array[0].mutex_fork);
	else
		pthread_mutex_unlock(&philo_array[i + 1].mutex_fork);
	philo_array[i].eat_count++;
}

void	*philo_simulation(void *arg)
{
	t_philo *philo;
	t_data *data;

	philo = (t_philo *) arg;
	data = philo->data;
	philo->death_time = data->time_to_die;
	usleep(1000);
	if (philo->philo_id % 2 == 0  || philo->philo_id == data->num_of_philos)
		usleep(300);
	while (1)
	{
		pthread_mutex_lock(&data->mutex_time_elapsed);
		if (data->time_elapsed > 0)
		{
			pthread_mutex_unlock(&data->mutex_time_elapsed);
			if (philo->philo_id % 2 != 0 && philo->philo_id != data->num_of_philos)
			{
				eating(data, data->philo_array, philo->philo_id);
			}
			if (philo->philo_id % 2 == 0  || philo->philo_id == data->num_of_philos)
			{
				eating(data, data->philo_array, philo->philo_id);
			}				
		}
		else
			pthread_mutex_unlock(&data->mutex_time_elapsed);
		pthread_mutex_lock(&data->mutex_deathcheck);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->mutex_deathcheck);
			return (0);
		}
		pthread_mutex_unlock(&data->mutex_deathcheck);
	}
	return (0);
}

int	death_checker_loop(int i, t_data *data, t_philo *philo_array)
{
	int	died;

	died = 0;
	while (i < data->num_of_philos)
	{
		pthread_mutex_lock(&philo_array[i].mutex_deathtime);
		if (philo_array[i].death_time != 0 && philo_array[i].death_time <= data->time_elapsed)
		{
			died = 1;
			print(data, philo_array[i].philo_id, "died");
			printf("death time %ld\n", philo_array[i].death_time);
			pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
			break ; 
		}
		pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
		i++;
	}
	if (died)
	{
		pthread_mutex_lock(&data->mutex_deathcheck);
		data->someone_died = died;
		pthread_mutex_unlock(&data->mutex_deathcheck);
		return (1);
	}
	return (0);
}

void	*death_checker(void *arg)
{
	t_philo *philo_array;
	t_data *data;
	int	died;

	philo_array = (t_philo *) arg;
	data = philo_array[0].data;
	// long time_elapsed = 5;
	died = 0;
	gettimeofday(&data->start, NULL);
	while (1)
	{
		gettimeofday(&data->current_time, NULL);
		pthread_mutex_lock(&data->mutex_time_elapsed);
		data->time_elapsed = ((data->current_time.tv_sec - data->start.tv_sec) * 1000) + ((data->current_time.tv_usec - data->start.tv_usec) / 1000);
		// printf("dch: %ld\n", data->time_elapsed);
		pthread_mutex_unlock(&data->mutex_time_elapsed);
		if (death_checker_loop(0, data, philo_array) == 1)
		{
			break ;
		}
		usleep(750);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:34 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/12 21:11:30 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print(t_data *data, int philo_id, char *str)
{
	int	died;
	long time;


	pthread_mutex_lock(&data->mutex_deathcheck);
	died = data->someone_died;
	pthread_mutex_unlock(&data->mutex_deathcheck);
	pthread_mutex_lock(&data->mutex_time_elapsed);
	time = data->time_elapsed;
	pthread_mutex_unlock(&data->mutex_time_elapsed);
	pthread_mutex_lock(&data->mutex_print);
	if (!died)
		printf("%ld %d %s\n", time, philo_id, str);
	pthread_mutex_unlock(&data->mutex_print);
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
	if (data->num_of_philos == 1)
	{
		pthread_mutex_lock(&philo_array[i].mutex_fork);
		print(data, philo_id, "has taken a fork");
		usleep(data->time_to_die * 1000);
		pthread_mutex_unlock(&philo_array[i].mutex_fork);
	}
	pthread_mutex_lock(&philo_array[philo_array[i].first_fork].mutex_fork);
	print(data, philo_id, "has taken a fork");
	pthread_mutex_lock(&philo_array[philo_array[i].second_fork].mutex_fork);
	print(data, philo_id, "has taken a fork");
	print(data, philo_id, "is eating");
	pthread_mutex_lock(&philo_array[i].mutex_deathtime);
	pthread_mutex_lock(&data->mutex_time_elapsed);
	philo_array[i].death_time = data->time_elapsed + data->time_to_die;
	pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
	pthread_mutex_unlock(&data->mutex_time_elapsed);
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
	int time_to_think;

	philo = (t_philo *) arg;
	data = philo->data;
	philo->death_time = data->time_to_die;
	if (philo->philo_id % 2 == 0)
		usleep(data->time_to_eat * 1000 * 0.5);
	if (philo->philo_id == data->num_of_philos && data->num_of_philos % 2 != 0)
		usleep(data->time_to_eat * 1000 * 0.8);
	while (1)
	{
		if (philo->philo_id % 2 != 0 && philo->philo_id != data->num_of_philos)
		{
			eating(data, data->philo_array, philo->philo_id);
		}
		if (philo->philo_id % 2 == 0  || philo->philo_id == data->num_of_philos)
		{
			eating(data, data->philo_array, philo->philo_id);
		}
		sleeping(data, philo->philo_id);
		print(data, philo->philo_id, "is thinking");
		pthread_mutex_lock(&philo->mutex_deathtime);
		pthread_mutex_lock(&data->mutex_time_elapsed);
		if (philo->philo_id % 2 == 0)
			time_to_think = ((data->time_to_die - (data->time_elapsed - philo->death_time)) * 0.5);
		// printf("time to think %d philo %d \n", time_to_think, philo->philo_id);
		pthread_mutex_unlock(&philo->mutex_deathtime);
		pthread_mutex_unlock(&data->mutex_time_elapsed);
		usleep(time_to_think);
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


int	is_dead(int i, t_data *data, t_philo *philo_array)
{
	int	died;

	died = 0;
	while (i < data->num_of_philos)
	{
		pthread_mutex_lock(&philo_array[i].mutex_deathtime);
		pthread_mutex_lock(&data->mutex_time_elapsed);
		if (philo_array[i].death_time != 0 && philo_array[i].death_time <= data->time_elapsed)
		{
			pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
			pthread_mutex_unlock(&data->mutex_time_elapsed);
			died = 1;
			print(data, philo_array[i].philo_id, "died");
			break ; 
		}
		pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
		pthread_mutex_unlock(&data->mutex_time_elapsed);
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

	philo_array = (t_philo *) arg;
	data = philo_array[0].data;
	gettimeofday(&data->start, NULL);
	while (1)
	{
		gettimeofday(&data->current_time, NULL);
		pthread_mutex_lock(&data->mutex_time_elapsed);
		data->time_elapsed = ((data->current_time.tv_sec - data->start.tv_sec) * 1000) + ((data->current_time.tv_usec - data->start.tv_usec) / 1000);
		pthread_mutex_unlock(&data->mutex_time_elapsed);
		if (is_dead(0, data, philo_array))
			break ;
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

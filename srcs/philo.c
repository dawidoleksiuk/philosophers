/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:34 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/07 20:55:57 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// int	clean_exit()
// {
// }


	// printf("%ld\n", data.num_of_philo);
	// printf("%ld\n", data.time_to_die);
	// printf("%ld\n", data.time_to_eat);
	// printf("%ld\n", data.time_to_sleep);
	// printf("%ld\n", data.num_of_times_to_eat);


void	print(t_data *data, int philo_id, char *str)
{
	pthread_mutex_lock(data->mutex_print);
	printf("%ld %d %s", data->time_elapsed, philo_id, str);
	pthread_mutex_unlock(data->mutex_print);
}	

void	sleeping(t_data *data, int philo_id)
{
	print(data, philo_id, "is sleeping");
	get_time(data->time_to_sleep);
}

void	eating(t_data *data, t_philo *philo_array, int philo_id)
{
	pthread_mutex_lock(philo_array[philo_id].mutex_fork);
	print(data, philo_id, "has taken a fork");
	if (philo_id == data->num_of_philos && data->num_of_philos != 1)
		pthread_mutex_lock(philo_array[1].mutex_fork);
	else
		pthread_mutex_lock(philo_array[philo_id + 1].mutex_fork);
	print(data, philo_id, "has taken a fork");
	print(data, philo_id, "is eating");
	get_time(data->time_to_eat);
	pthread_mutex_unlock(philo_array[philo_id].mutex_fork);
	if (philo_id == data->num_of_philos && data->num_of_philos != 1)
		pthread_mutex_unlock(philo_array[1].mutex_fork);
	else
		pthread_mutex_unlock(philo_array[philo_id + 1].mutex_fork);
	philo_array[philo_id].eat_count++;
	philo_array[philo_id].death_time = data->time_elapsed + data->time_to_die;
}

void	*philo_simulation(void *arg)
{
	t_data *data;
	t_philo *philo_array;
	int philo_id;

	data = (t_data *) arg;
	philo_array = data->philo_array;
	philo_id = data->philo_id;
	while (1)
	{
		if (philo_id % 2 != 0 || philo_id == data->num_of_philos)
		{
			eating(data, philo_array, philo_id);
		}
		sleeping(data, philo_id);
		if (philo_id % 2 == 0  && philo_id != data->num_of_philos)
		{
			eating(data, philo_array, philo_id);
		}
		print(data, philo_id, "is thinking");
	}
}

void	run_simulation(t_data *data)
{
	int	i;
	t_philo *philo_array;

	i = 0;
	philo_array = data->philo_array;
	pthread_mutex_init(data->mutex_print, NULL);
	while (i <= data->num_of_philos)
	{
		data->philo_id = i;
		pthread_mutex_init(philo_array[i].mutex_fork, NULL);
		if (pthread_create(&philo_array[i].thread_id, NULL, &philo_simulation, &data) != 0)
 			perror("Failed to create thread");
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (data_validation(argc, argv) != 0 || data_init(argc, argv, &data) != 0)
		return (1);
	run_simulation(&data);
	
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 18:14:49 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/22 18:27:51 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	check_if_dead(t_philo *philo_array, t_data *data, int *i)
{
	struct timeval	current_time;
	long long		time;

	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec - data->start_time.tv_sec) * 1000;
	time += (current_time.tv_usec - data->start_time.tv_usec) / 1000;
	pthread_mutex_lock(&philo_array[*i].mutex_deathtime);
	if (philo_array[*i].death_time <= time)
	{
		pthread_mutex_unlock(&philo_array[*i].mutex_deathtime);
		pthread_mutex_lock(&data->mutex_print);
		data->someone_died = 1;
		printf("%lld %d died\n", time, philo_array[*i].philo_id);
		pthread_mutex_unlock(&data->mutex_print);
		return (1);
	}
	pthread_mutex_unlock(&philo_array[*i].mutex_deathtime);
	if (data->num_of_times_to_eat == philo_array[*i].eat_count)
		return (2);
	return (0);
}

void	*monitor_routine(void *arg)
{
	int		i;
	t_philo	*philo_array;
	t_data	*data;

	i = 0;
	philo_array = (t_philo *)arg;
	data = philo_array[0].data;
	while (1)
	{
		if (i == data->num_of_philos)
		{
			usleep(500);
			i = 0;
		}
		if (check_if_dead(philo_array, data, &i))
			break ;
		i++;
	}
	return (NULL);
}

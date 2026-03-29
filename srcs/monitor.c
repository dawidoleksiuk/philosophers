/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 18:14:49 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/29 18:14:19 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	set_stop_flag(t_data *data)
{
	pthread_mutex_lock(&data->mutex_stop);
	data->stop_simulation = 1;
	pthread_mutex_unlock(&data->mutex_stop);
}

static int	check_if_dead(t_philo *philo_array, t_data *data, int *i)
{
	long long		time;

	pthread_mutex_lock(&philo_array[*i].mutex_deathtime);
	time = get_current_time(data);
	if (philo_array[*i].death_time <= time)
	{
		pthread_mutex_unlock(&philo_array[*i].mutex_deathtime);
		set_stop_flag(data);
		return (1);
	}
	pthread_mutex_unlock(&philo_array[*i].mutex_deathtime);
	return (0);
}

int	check_if_full(t_philo *philo_array, t_data *data, int *i)
{
	pthread_mutex_lock(&data->mutex_stop);
	if (philo_array[*i].eat_count == data->num_of_times_to_eat)
	{
		data->finished_eating_count++;
		if (data->finished_eating_count == data->num_of_philos)
		{
			pthread_mutex_unlock(&data->mutex_stop);
			set_stop_flag(data);
			return (1);
		}
	}
	pthread_mutex_unlock(&data->mutex_stop);
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
			data->finished_eating_count = 0;
		}
		if (check_if_dead(philo_array, data, &i))
		{
			print_state(data, philo_array[i].philo_id, "died");
			break ;
		}
		if (data->num_of_times_to_eat > 0)
		{
			if (check_if_full(philo_array, data, &i))
			{
				print_state(data, philo_array[i].philo_id, "all philos finished eating");
				break ;
			}
		}
		i++;
	}
	return (NULL);
}

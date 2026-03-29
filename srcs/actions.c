/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 18:27:46 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/29 18:47:32 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	philo_sleep(t_data *data, int philo_id)
{
	if (!check_stop(data))
		print_state(data, philo_id, "is sleeping");
	ft_sleep_ms(data->time_to_sleep);
	// ft_sleep_ms(data->time_to_sleep / 2);
	return (0);
}

int	philo_eat(t_data *data, t_philo *philo_array, int philo_id)
{
	int				i;
	long long		time;

	i = philo_id - 1;
	pthread_mutex_lock(philo_array[i].left_fork);
	if (!check_stop(data))
		print_state(data, philo_id, "has taken a fork");
	if (data->num_of_philos == 1)
	{
		ft_sleep_ms(data->time_to_die);
		pthread_mutex_unlock(philo_array[i].left_fork);
		return (1);
	}
	pthread_mutex_lock(philo_array[i].right_fork);
	if (!check_stop(data))
		print_state(data, philo_id, "has taken a fork");
	if (!check_stop(data))
	{
		pthread_mutex_lock(&philo_array[i].mutex_deathtime);
		time = get_current_time(data);
		philo_array[i].death_time = time + data->time_to_die;
		pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
		print_state(data, philo_id, "is eating");
		ft_sleep_ms(data->time_to_eat);
	}
	pthread_mutex_unlock(philo_array[i].right_fork);
	pthread_mutex_unlock(philo_array[i].left_fork);
	if (check_stop(data))
		return (1);
	if (data->num_of_times_to_eat > 0)
	{
		pthread_mutex_lock(&data->mutex_stop);
		philo_array[i].eat_count++;
		pthread_mutex_unlock(&data->mutex_stop);
	}
	return (0);
}

int	philo_think(t_data *data, int philo_id)
{
	if (!check_stop(data))
		print_state(data, philo_id, "is thinking");
	if (data->num_of_philos % 2 != 0)
		ft_sleep_ms(1);
	return (0);
}

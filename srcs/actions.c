/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 18:27:46 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/22 18:40:43 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	philo_sleep(t_data *data, int philo_id)
{
	if (print_state(data, philo_id, "is sleeping"))
		return (1);
	ft_sleep_ms(data->time_to_sleep);
	return (0);
}

void	philo_eat(t_data *data, t_philo *philo_array, int philo_id)
{
	int				i;
	long long		time;
	struct timeval	current_time;

	i = philo_id - 1;
	pthread_mutex_lock(philo_array[i].left_fork);
	print_state(data, philo_id, "has taken a fork");
	if (data->num_of_philos == 1)
	{
		ft_sleep_ms(data->time_to_die);
		pthread_mutex_unlock(philo_array[i].left_fork);
		return ;
	}
	pthread_mutex_lock(philo_array[i].right_fork);
	print_state(data, philo_id, "has taken a fork");
	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec - data->start_time.tv_sec) * 1000;
	time += (current_time.tv_usec - data->start_time.tv_usec) / 1000;
	pthread_mutex_lock(&philo_array[i].mutex_deathtime);
	philo_array[i].death_time = time + data->time_to_die;
	pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
	print_state(data, philo_id, "is eating");
	ft_sleep_ms(data->time_to_eat);
	pthread_mutex_unlock(philo_array[i].right_fork);
	pthread_mutex_unlock(philo_array[i].left_fork);
}

int	philo_think(t_data *data, int philo_id)
{
	if (print_state(data, philo_id, "is thinking") == 1)
		return (1);
	if (data->num_of_philos % 2 != 0)
		ft_sleep_ms(1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 20:07:41 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/22 18:39:00 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->philo_id % 2 == 0)
		ft_sleep_ms(data->time_to_eat / 2);
	while (1)
	{
		philo_eat(data, data->philo_array, philo->philo_id);
		philo->eat_count++;
		if (data->num_of_philos == 1
			|| data->num_of_times_to_eat == philo->eat_count)
			break ;
		if (philo_sleep(data, philo->philo_id) != 0)
			break ;
		if (philo_think(data, philo->philo_id) != 0)
			break ;
	}
	return (NULL);
}

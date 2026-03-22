/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 18:19:22 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/22 18:07:32 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	mutex_destroy(t_data *data, t_philo *philo_array)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->mutex_print);
	pthread_mutex_destroy(&data->mutex_deathcheck);
	while (i < data->num_of_philos)
	{
		pthread_mutex_destroy(&philo_array[i].mutex_fork);
		pthread_mutex_destroy(&philo_array[i].mutex_deathtime);
		i++;
	}
}

void	clean_exit(t_data *data, t_philo *philo_array)
{
	mutex_destroy(data, philo_array);
	free(philo_array);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 21:11:28 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/07 20:55:59 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	data_validation(int argc, char *argv[])
{
	int	i;
	int	j;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die[ms] "
			"time_to_eat[ms] time_to_sleep[ms] "
			"[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (printf("All arguments must be positive numbers.\n"), 1);
			j++;
		}
		i++;
	}
	return (0);
}
static int	str_to_int(char *str, long *arg)
{
	int	i;

	i = 0;
	*arg = 0;
	while (str[i])
	{
		if (*arg > (UINT_MAX - (str[i] - '0')) / 10)
			return (1);
		*arg *= 10;
		*arg += (str[i] - '0');
		i++;
	}
	if (*arg > UINT_MAX / 1000)
		return (1);
	return (0);
}

int	data_init(int argc, char *argv[], t_data *data)
{
	memset(data, 0, sizeof(t_data));
	if (str_to_int(argv[1], &data->num_of_philos) != 0
		|| data->num_of_philos > MAX_PHILO)
		return (printf("Number of philosophers too high."
				"It can be max 200\n"), 1);
	if (str_to_int(argv[2], &data->time_to_die) != 0)
		return (printf("Time to die too high. It can be max 4294967\n"), 1);
	if (str_to_int(argv[3], &data->time_to_eat) != 0)
		return (printf("Time to eat too high. It can be max 4294967\n"), 1);
	if (str_to_int(argv[4], &data->time_to_sleep) != 0)
		return (printf("Time to sleep too high. It can be max 4294967\n"), 1);
	if (argc == 6)
	{
		if (str_to_int(argv[5], &data->num_of_times_to_eat) != 0)
		{
			return (printf("Number of times to eat too high. "
					"It can be max 4294967\n"), 1);
		}
	}
	else
		data->num_of_times_to_eat = -1;
	data->philo_array = malloc((data->num_of_philos + 1) * sizeof(t_philo));
	if (!data->philo_array)
		return (1);
	return (0);
}

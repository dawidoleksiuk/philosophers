/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 21:11:28 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/11 20:35:46 by doleksiu         ###   ########.fr       */
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

int	argv_init(int argc, char *argv[], t_data *data)
{
	if (str_to_int(argv[1], &data->num_of_philos) != 0
		|| data->num_of_philos > MAX_PHILO || data->num_of_philos == 0)
		return (printf("Wrong number of philosophers. "
				"It can be 1 to 200\n"), 1);
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
	return (0);
}

int	philo_array_init(t_data *data, t_philo *philo_array)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		memset(&philo_array[i], 0, sizeof(t_philo));
		philo_array[i].philo_id = i + 1;
		philo_array[i].data = data;
		// printf("dt: %ld i: %d\n", (philo_array[i]).death_time, i);
		i++;
	}
	return (0);
}

void	mutex_init(t_data *data, t_philo *philo_array)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->mutex_print, NULL);
	pthread_mutex_init(&data->mutex_deathcheck, NULL);
	pthread_mutex_init(&data->mutex_time_elapsed, NULL);
	while (i < data->num_of_philos)
	{	
		pthread_mutex_init(&philo_array[i].mutex_fork, NULL);
		pthread_mutex_init(&philo_array[i].mutex_deathtime, NULL);
		i++;
	}
}

int	data_init(int argc, char *argv[], t_data *data, t_philo **philo_array)
{
	memset(data, 0, sizeof(t_data));
	if (argv_init(argc, argv, data) != 0)
		return (1);
	*philo_array = malloc(data->num_of_philos * sizeof(t_philo));
	if (!*philo_array)
		return (1);
	data->philo_array = *philo_array;
	if (philo_array_init(data, *philo_array) != 0)
		return (1);
	mutex_init(data, *philo_array);
	return (0);
}

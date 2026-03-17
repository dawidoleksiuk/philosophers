/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 21:11:28 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/17 20:35:16 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	run_simulation(t_data *data, t_philo *philo_array)
{
	int		i;
	int		res;
	t_philo	*p;

	i = 0;
	while (i < data->num_of_philos)
	{
		p = &philo_array[i];
		if (pthread_create(&p->thread_id, NULL, &philo_simulation, p) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&data->death_th, NULL, &death_checker, philo_array) != 0)
		return (1);
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_join(philo_array[i].thread_id, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_join(data->death_th, NULL) != 0)
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
		philo_array[i].death_time = data->time_to_die;
		philo_array[i].data = data;
		philo_array[i].fork_one = i;
		philo_array[i].fork_two = (i + 1) % data->num_of_philos;
		if (philo_array[i].philo_id == data->num_of_philos)
		{
			philo_array[i].fork_one = (i + 1) % data->num_of_philos;
			philo_array[i].fork_two = i;
		}
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
	gettimeofday(&data->start, NULL);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:34 by doleksiu          #+#    #+#             */
/*   Updated: 2026/02/08 18:10:08 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// int	clean_exit()
// {
// }

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

int	str_to_int(char *str, long *arg)
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
	if (str_to_int(argv[1], &data->num_of_philo) != 0
		|| data->num_of_philo > MAX_PHILO)
		return (printf("Number of philosophers too high."
				"It can be max 200\n"), 1);
	if (str_to_int(argv[2], &data->time_to_die) != 0)
		return (printf("Time to die too high. It can be max 4294967\n"), 1);
	if (str_to_int(argv[3], &data->time_to_eat) != 0)
		return (printf("Time to eat too high. It can be max 4294967\n"), 1);
	if (str_to_int(argv[4], &data->time_to_sleep) != 0)
		return (printf("Time to slepp too high. It can be max 4294967\n"), 1);
	if (argc == 6)
		if (str_to_int(argv[5], &data->num_of_times_to_eat) != 0)
			return (printf("Time of times to eat too high."
					"It can be max 4294967\n"), 1);
	else
		data->num_of_times_to_eat = -1;
	return (0);
}

	// printf("%lu\n", data->num_of_philo);
	// printf("%lu\n", data->time_to_die);
	// printf("%lu\n", data->time_to_eat);
	// printf("%lu\n", data->time_to_sleep);
	// printf("%lu\n", data->num_of_times_to_eat);

int	main(int argc, char *argv[])
{
	t_data	data;

	if (data_validation(argc, argv) != 0 || data_init(argc, argv, &data) != 0)
		return (1);
}

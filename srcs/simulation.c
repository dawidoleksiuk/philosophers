/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 20:07:41 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/17 20:29:46 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	sleeping(t_data *data, int philo_id)
{
	if (print(data, philo_id, "is sleeping"))
		return ;
	ft_sleep(data->time_to_sleep);
}

void	eating(t_data *data, t_philo *philo_array, int philo_id)
{
	int				i;
	long			time;
	struct timeval	current_time;

	i = philo_id - 1;
	pthread_mutex_lock(&philo_array[philo_array[i].fork_one].mutex_fork);
	print(data, philo_id, "has taken a fork");
	if (data->num_of_philos == 1)
	{
		ft_sleep(data->time_to_die);
		pthread_mutex_unlock(&philo_array[philo_array[i].fork_one].mutex_fork);
		return ;
	}
	pthread_mutex_lock(&philo_array[philo_array[i].fork_two].mutex_fork);
	print(data, philo_id, "has taken a fork");
	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec - data->start.tv_sec) * 1000;
	time += (current_time.tv_usec - data->start.tv_usec) / 1000;
	pthread_mutex_lock(&philo_array[i].mutex_deathtime);
	philo_array[i].death_time = time + data->time_to_die;
	pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
	print(data, philo_id, "is eating");
	ft_sleep(data->time_to_eat);
	pthread_mutex_unlock(&philo_array[philo_array[i].fork_two].mutex_fork);
	pthread_mutex_unlock(&philo_array[philo_array[i].fork_one].mutex_fork);
}

void	*philo_simulation(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int		time_to_think;

	philo = (t_philo *) arg;
	data = philo->data;
	if (philo->philo_id % 2 == 0)
		ft_sleep(data->time_to_eat / 2);
	while (1)
	{
		eating(data, data->philo_array, philo->philo_id);
		philo->eat_count++;
		if (data->num_of_philos == 1
			|| data->num_of_times_to_eat == philo->eat_count)
			break ;
		sleeping(data, philo->philo_id);
		if (print(data, philo->philo_id, "is thinking") == 1)
			break ;
		usleep(500);
	}
	return (NULL);
}

static int	check_if_dead(t_philo *philo_array, t_data *data, int *i)
{
	struct timeval	current_time;
	int				time;

	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec - data->start.tv_sec) * 1000;
	time += (current_time.tv_usec - data->start.tv_usec) / 1000;
	pthread_mutex_lock(&philo_array[*i].mutex_deathtime);
	if (philo_array[*i].death_time <= time)
	{
		pthread_mutex_unlock(&philo_array[*i].mutex_deathtime);
		pthread_mutex_lock(&data->mutex_print);
		data->someone_died = 1;
		printf("%d %d died\n", time, philo_array[*i].philo_id);
		pthread_mutex_unlock(&data->mutex_print);
		return (1);
	}
	pthread_mutex_unlock(&philo_array[*i].mutex_deathtime);
	if (data->num_of_times_to_eat == philo_array[*i].eat_count)
		return (2);
	return (0);
}

void	*death_checker(void *arg)
{
	t_philo	*philo_array;
	t_data	*data;
	int		i;

	philo_array = (t_philo *) arg;
	data = philo_array[0].data;
	i = 0;
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

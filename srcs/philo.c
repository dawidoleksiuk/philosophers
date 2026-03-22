/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:34 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/22 17:19:24 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philo_array;

	philo_array = NULL;
	if (data_validation(argc, argv) != 0
		|| init_program_data(argc, argv, &data, &philo_array) != 0)
	{
		if (philo_array)
			clean_exit(&data, philo_array);
		return (1);
	}
	run_simulation(&data, philo_array);
	clean_exit(&data, philo_array);
}

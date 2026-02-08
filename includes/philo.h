/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:51 by doleksiu          #+#    #+#             */
/*   Updated: 2026/02/08 17:55:37 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_PHILO 200

typedef struct s_data
{
	long num_of_philo;
	long time_to_die;
	long time_to_eat;
	long time_to_sleep;
	long num_of_times_to_eat;
} t_data;
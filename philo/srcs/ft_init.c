/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgaulti <asgaulti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/19 15:27:35 by asgaulti          #+#    #+#             */
/*   Updated: 2021/12/02 18:11:31 by asgaulti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_data(t_data *data, char **av, int ac)
{
	memset(data, 0, sizeof(t_data));
	data->nb = ft_atoi(av[1]);
	data->die = ft_atoi(av[2]) * 1000;
	data->eat = ft_atoi(av[3]) * 1000;
	data->sleep = ft_atoi(av[4]) * 1000;
	data->life = 0;
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	else
		data->must_eat = 0;
	if (ft_check_arg(ac, av, data) == 1)
		return (2);
	data->philo = malloc(sizeof(t_philo) * data->nb);
	if (!data->philo || !data)
		return (ft_print("Error init\n", 1));
	memset(data->philo, 0, sizeof(t_philo) * data->nb);
	if (ft_init_mutex(data) == 1)
		return (1);
	if (ft_init_philo(data) == 1)
		return (1);
	return (0);
}

int	ft_init_philo(t_data *data)
{
	int	i;

	i = 0;
	gettimeofday(&data->start_time, NULL);
	pthread_mutex_lock(data->synchro);
	if (ft_init_data_phi(data) == 1)
	{
		pthread_mutex_unlock(data->synchro);
		return (1);
	}
	pthread_mutex_unlock(data->synchro);
	while (!ft_check_end(data)) // = ft_check_end(data) == 0
	{
		i = 0;
		while (i < data->nb)
		{
			if (ft_gettime_lasteat(i, data) == 1) // ft_gettime_lasteat(i, data) == 1
			{
				pthread_mutex_lock(data->dead);
				data->life = 1;
				pthread_mutex_unlock(data->dead);
				ft_print_action(&data->philo[i], data, "died");
				ft_join_thread(data);
				return (1);
			}
			else if (ft_check_count(data))
			{
				return (ft_reach_count(data));
			}
			i++;
		}
	}
	return (0);
}

int	ft_init_data_phi(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb)
	{
		data->philo[i].philo_nb = i + 1;
		data->philo[i].data = data;
		data->philo[i].count = 1;
		if (pthread_create(&data->philo[i].philo_thread, NULL,
				ft_routine, &data->philo[i]))
			return (1);
		i++;
	}
	return (0);
}

int	ft_init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb)
	{
		data->philo[i].m_last_eat = malloc(sizeof(pthread_mutex_t));
		data->philo[i].left_f = malloc(sizeof(pthread_mutex_t));
		data->philo[i].m_count = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(data->philo[i].left_f, NULL)
			|| pthread_mutex_init(data->philo[i].m_last_eat, NULL)
			|| pthread_mutex_init(data->philo[i].m_count, NULL))
			return (ft_print("Error in mutex\n", 1));
		i++;
	}
	ft_init_mutex_rfork(data);
	data->write = malloc(sizeof(pthread_mutex_t));
	data->m_time = malloc(sizeof(pthread_mutex_t));
	data->dead = malloc(sizeof(pthread_mutex_t));
	data->synchro = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(data->write, NULL)
		|| pthread_mutex_init(data->dead, NULL)
		|| pthread_mutex_init(data->m_time, NULL)
		|| pthread_mutex_init(data->synchro, NULL))
		return (ft_print("Error in mutex\n", 1));
	return (0);
}

void	ft_init_mutex_rfork(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb)
	{
		data->philo[i].right_f = (data->philo[(i + 1) % data->nb].left_f);
		i++;
	}
}
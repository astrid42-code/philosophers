/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgaulti <asgaulti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/19 15:27:35 by asgaulti          #+#    #+#             */
/*   Updated: 2021/09/26 16:42:57 by asgaulti         ###   ########.fr       */
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
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	else
		data->must_eat = 0;
	if (ft_check_arg(ac, av, data) == 1)
		return (1);
	data->philo = malloc(sizeof(t_philo) * data->nb);
	memset(data->philo, 0, sizeof(t_philo) * data->nb);
	if (!data->philo || !data)
	{
		ft_print("Error init\n");
		return (1);
	}
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
	//printf("start = %ld %d\n", data->start_time.tv_sec, data->start_time.tv_usec);
	//pthread_mutex_lock(data->m_init);
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
	//while (1)
	while (data->life == 0 && data->must_eat != data->nb)
	{
		i = 0;
		while (i < data->nb)
		{
			if (ft_gettime_lasteat(data->philo[i].last_eat, data)
				> (unsigned long)data->die)
			{
				pthread_mutex_lock(data->dead);
				data->life = 1;
				ft_print_action(&data->philo[i], data, "died"); // &data->philo[i] > ou data->philo + i
				pthread_mutex_unlock(data->dead);
				//ft_exit(data);
				//break ;
				return (1);
			}
			if (data->philo->count == data->must_eat)
			{
				ft_join_thread(data);
				ft_print("count reached\n");
				ft_exit(data);
				return (1);
			}
			i++;
		}
	}
	// ft_exit(data);
	return (0);
}

// pt de depart : timestamp (ie current)
// a chaque fois qu ils mangent tu recuperes le timestamp
// ensuite tu soustrais le start_time (on ne le touche jms) du last_eat

// si le last_eat - start_time est > au time_to_die alos le philo est mort
// dans ce cas variable life = 1 (au lieu de 0) et dire aux philos que si life == 1 > ca s arrete (ft_exit)

int	ft_init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb)
	{
		data->philo[i].left_f = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(data->philo[i].left_f, NULL))
		{
			ft_print("Error in mutex\n");
			return (1);
		}
		i++;
	}
	ft_init_mutex_rfork(data);
	data->write = malloc(sizeof(pthread_mutex_t));
	data->dead = malloc(sizeof(pthread_mutex_t));
	// data->m_init = malloc(sizeof(pthread_mutex_t));
	// data->m_count = malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(data->write, NULL)
		|| pthread_mutex_init(data->dead, NULL))
	{
		ft_print("Error in mutex\n");
		return (1);
	}
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

// a faire au debut de chaque repas / dodo
//unsigned long	ft_gettime(t_timeval *start_time, t_timeval *start_action)
long int	ft_gettime(t_timeval *start_time)
{
	// unsigned long	time;

	// //printf("start_time = %ld %d\n start_eat = %ld %d\n", start_time->tv_sec, start_time->tv_usec, start_eat->tv_sec, start_eat->tv_usec);
	// time = (unsigned long)(((start_action->tv_sec * 1000)
	// 			+ (start_action->tv_usec * 0.001))
	// 		- ((start_time->tv_sec * 1000) + (start_time->tv_usec * 0.001)));
	t_timeval	current;
	long int	time;

	gettimeofday(&current, NULL);
	time = (((current.tv_sec - start_time->tv_sec) * 1000000)
			+ (current.tv_usec - start_time->tv_usec));
	// time = (((start_action->tv_sec - start_time->tv_sec) * 1000000)
	// 		+ (start_action->tv_usec - start_time->tv_usec));
	// time en usec
	//printf("time = %ld\n", time);
	return (time);
}

unsigned long	ft_gettime_lasteat(unsigned long last_eat, t_data *data)
{
	long int	time;
	
	time = ft_gettime(&data->start_time);
	return (time - last_eat);
}

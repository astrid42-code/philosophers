/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asgaulti <asgaulti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 13:33:20 by asgaulti          #+#    #+#             */
/*   Updated: 2021/12/04 10:25:51 by asgaulti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

typedef void			*(*t_routine)(void*);

typedef struct timeval	t_timeval;

typedef struct s_philo
{
	int					philo_nb;
	int					phi_life;
	int					count;
	unsigned long		last_eat;
	pthread_mutex_t		*left_f;
	pthread_mutex_t		*right_f;
	pthread_mutex_t		*m_last_eat;
	pthread_mutex_t		*m_count;
	pthread_t			one;
	pthread_t			philo_thread;
	struct s_data		*data;
}						t_philo;

typedef struct s_data
{
	int					nb;
	int					die;
	int					eat;
	int					sleep;
	int					must_eat;
	int					life;
	pthread_mutex_t		*write;
	pthread_mutex_t		*m_time;
	pthread_mutex_t		*dead;
	pthread_mutex_t		*synchro;
	t_timeval			start_time;
	unsigned long		time;
	t_philo				*philo;
}						t_data;

int						main(int ac, char **av);

// routine
void					*ft_routine(t_philo *philo);
int						ft_launch_philo(t_philo *philo, t_data *data);
int						ft_time_to_eat(t_philo *philo, t_data *data);
int						ft_take_fork(t_philo *philo, t_data *data);
int						ft_special_one(t_data *data, t_philo *philo);
void					ft_special(t_data *data);
unsigned long			ft_gettime(t_timeval *start_time);
unsigned long			ft_gettime_lasteat(int i, t_data *data);
int						ft_check_count_rout(t_philo *philo_cp, t_data *data);

// init / exit
int						ft_init_data(t_data *data, char **av, int ac);
int						ft_init_mutex(t_data *data);
void					ft_init_mutex_rfork(t_data *data);
int						ft_init_philo(t_data *data);
int						ft_init_data_phi(t_data *data);
int						ft_monitor(t_data *data);
int						ft_check_count_init(t_data *data);
int						ft_reach_count(t_data *data);
int						ft_simple_exit(t_data *data);
void					ft_exit(t_data *data);
void					ft_join_thread(t_data *data);

// utils
int						ft_print(char *str, int ret);
void					ft_print_action(t_philo *philo, t_data *data,
							char *str);
int						ft_strlen(char *str);
int						ft_atoi(char *str);
void					ft_free(t_data *data, t_philo *philo);
void					ft_usleep(unsigned int wait);
int						ft_check_end(t_data *data);

// check
int						ft_check_arg(int ac, char **av, t_data *data);
int						ft_check_data(t_data *data);

#endif
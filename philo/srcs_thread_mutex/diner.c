/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysonmez <ysonmez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/15 18:44:14 by ysonmez           #+#    #+#             */
/*   Updated: 2021/12/21 13:55:24 by ysonmez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_t_m.h"

/*	Sleeping function, after finishing the meal */

static int	sleeping(t_ph *ph)
{
	if (*(ph->alive) == true)
		printer(ph, SLEEP);
	if (ft_sleep(ph->param->time_to_sleep, ph))
		return (1);
	return (0);
}

/*	Taking forks and eating, drop fork at the end */

static int	eating(t_ph *ph)
{
	pthread_mutex_lock(ph->lfork);
	if (*(ph->alive) == true)
		printer(ph, FORK);
	pthread_mutex_lock(ph->rfork);
	if (*(ph->alive) == true)
		printer(ph, FORK);
	if (*(ph->alive) == true)
		printer(ph, EAT);
	if (ft_sleep(ph->param->time_to_eat, ph))
	{
		pthread_mutex_unlock(ph->rfork);
		pthread_mutex_unlock(ph->lfork);
		return (1);
	}
	ph->last_meal = get_time() - ph->param->start_time;
	pthread_mutex_unlock(ph->rfork);
	pthread_mutex_unlock(ph->lfork);
	ph->meal++;
	return (0);
}

/*	Thinking state, after eating and before getting a fork
*	Small sleep for the first thinkers of the diner for synchronisation
*/

static int	thinking(t_ph *ph)
{
	if (*(ph->alive) == true)
		printer(ph, THINK);
	if (ph->meal == 0)
	{
		if (ft_sleep(100, ph))
			return (1);
	}
	return (0);
}

/*	Routine for threads */

void	*schedule(void *ph)
{
	if (ph == NULL || ((t_ph *)ph)->param->nb_philo == 1)
		return (NULL);
	if (((t_ph *)ph)->i % 2 == ODD)
		thinking(ph);
	while (1)
	{
		if (eating(ph))
			break ;
		if (((t_ph *)ph)->meal == ((t_ph *)ph)->param->meals_per_philo)
			break ;
		if (sleeping(ph))
			break ;
		if (thinking(ph))
			break ;
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:14:29 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/06 11:20:49 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Might need to add sum signals handling in the exec part

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	get_ms()->last_exit = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
void	sigquit_handler(int sig)
{
	int	status;

	(void)sig;
	status = get_ms()->last_exit;
	write(1, "exit\n", 5);
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(status);
}*/

void	child_sigint_handler(int sig)
{
	if (sig == 2)
		get_ms()->last_exit = 130;
	else
	{
		ft_putstr_fd("Quit", 1);
		get_ms()->last_exit = 131;
	}
	write(1, "\n", 1);
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

/* TODO : exit by pressing Ctrl D should
	1. print "exit\n"
	2. keep the last exit status in $?
*/

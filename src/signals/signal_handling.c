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
extern int		g_signal;

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	get_ms()->last_exit = 130;
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

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
	g_signal = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	update_heredoc_signal(int sig)
{
	g_signal = sig;
	get_ms()->last_exit = 130;
}

void	handle_heredoc_signal(int sig)
{
	g_signal = sig;
	ft_printf("\n");
	child_free_exit(130);
}

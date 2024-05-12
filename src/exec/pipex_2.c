/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:06:53 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/09 16:19:25 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parent_process(int *fd, int *output_fd)
{
	close(fd[1]);
	if (*output_fd != STDIN_FILENO)
		close(*output_fd);
	*output_fd = fd[0];
}

void	child_process(int i, int nb_cmds, t_cmd *cmd, int *output_fd)
{
	int		fd[2];
	t_ms	*ms;

	ms = get_ms();
	handle_pipe(fd, ms, i);
	cmd->i = &i;
	if (ms->pids[i] == 0)
	{
		handle_child_input(cmd, fd, output_fd);
		handle_child_output(nb_cmds, cmd, fd, output_fd);
	}
	else
	{
		handle_parent_process(fd, output_fd);
	}
}

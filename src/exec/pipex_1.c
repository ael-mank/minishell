/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:04:56 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/09 21:05:04 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipex(t_ms *ms, t_list *cmds, int nb_cmds)
{
	int		i;
	int		output_cache;
	t_cmd	*curr_cmd;
	int		status;

	(void)ms;
	i = 1;
	output_cache = STDIN_FILENO;
	while (cmds)
	{
		curr_cmd = (t_cmd *)cmds->content;
		child_process(i, nb_cmds, curr_cmd, &output_cache);
		cmds = cmds->next;
		i++;
	}
	i = -1;
	while (++i < nb_cmds)
		waitpid(ms->pipe[i].pid, &status, 0);
	if (WIFEXITED(status))
		get_ms()->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		get_ms()->last_exit = WTERMSIG(status);
}

void	handle_pipe(int *fd, t_ms *ms, int i)
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	ms->pids[i] = fork();
	if (ms->pids[i] == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	handle_last_command(t_cmd *cmd, int *fd)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->fd_out);
	}
	close(fd[1]);
}

void	handle_child_input(t_cmd *cmd, int *fd, int *output_fd)
{
	close(fd[0]);
	if (cmd->fd_in != STDIN_FILENO)
	{
		close(*output_fd);
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->fd_in);
	}
	else if (*cmd->i != 1)
	{
		if (dup2(*output_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(*output_fd);
	}
}

void	handle_child_output(int nb_cmds, t_cmd *cmd, int *fd, int *output_fd)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		close(*output_fd);
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->fd_out);
	}
	else if (*cmd->i < nb_cmds)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fd[1]);
	}
	else
		handle_last_command(cmd, fd);
	execute_child(cmd);
	perror("minishell");
	exit(EXIT_FAILURE);
}

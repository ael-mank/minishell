/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:23:50 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/03 13:41:30 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_manager(void)
{
	t_ms	*ms;
	t_list	*cmds;
	int		nb_cmds;

	ms = get_ms();
	cmds = ms->cmds;
	nb_cmds = ft_lstsize(cmds);
	if (nb_cmds == 0)
		return ;
	handle_redirections(cmds);
	expand_fullpath(cmds);
	if (nb_cmds == 1)
		single_cmd_exec(cmds->content);
	else
		pipex(ms, cmds, nb_cmds);
}

void	single_cmd_exec(t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (cmd->fd_in == -1 || cmd->fd_out == -1)
	{
		get_ms()->last_exit = 1;
		return ;
	}
	if (is_builtin(cmd->cmd_arr[0]))
	{
		get_ms()->last_exit = exec_builtin(cmd);
		return ;
	}
	pid = fork();
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigint_handler);
	if (pid == 0)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		dup2(cmd->fd_out, STDOUT_FILENO);
		execute_child(cmd);
	}
	waitpid(pid, &status, 0);
	update_last_exit_status(status);
	setup_signals();
}

void	update_last_exit_status(int status)
{
	if (WIFEXITED(status) && get_ms()->last_exit == 0)
		get_ms()->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status) && get_ms()->last_exit == 0)
		get_ms()->last_exit = WTERMSIG(status);
}

bool	is_builtin(char *cmd_name)
{
	if (!ft_strncmp(cmd_name, "echo", 5))
		return (1);
	if (!ft_strncmp(cmd_name, "cd", 3))
		return (1);
	if (!ft_strncmp(cmd_name, "pwd", 4))
		return (1);
	if (!ft_strncmp(cmd_name, "export", 7))
		return (1);
	if (!ft_strncmp(cmd_name, "unset", 6))
		return (1);
	if (!ft_strncmp(cmd_name, "env", 4))
		return (1);
	if (!ft_strncmp(cmd_name, "exit", 5))
		return (1);
	return (0);
}

void	child_free_exit(int exit_code)
{
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(exit_code);
}

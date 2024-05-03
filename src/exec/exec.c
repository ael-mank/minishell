/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:23:39 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/03 11:34:16 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cmd_exists(t_cmd *child)
{
	if (!child->fullpath)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (0);
	}
	return (1);
}

bool	cmd_is_executable(t_cmd *child)
{
	if (access(child->fullpath, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(child->cmd_arr[0]);
		return (0);
	}
	return (1);
}

void	execute_child(t_cmd *child)
{
	int	builtin_exit_code;

	if (child->fd_in == -1 || child->fd_out == -1)
		child_free_exit(1);
	if (is_builtin(child->cmd_arr[0]))
	{
		builtin_exit_code = exec_single_builtin(child);
		child_free_exit(builtin_exit_code);
	}
	if (!cmd_exists(child))
		child_free_exit(127);
	if (!cmd_is_executable(child))
		child_free_exit(126);
	execve(child->fullpath, child->cmd_arr, NULL);
	perror("minishell");
	child_free_exit(1);
}

void	child_free_exit(int exit_code)
{
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(exit_code);
}

int	exec_builtin(t_cmd *child)
{
	int	exit_code;
	int	original;

	exit_code = 0;
	original = 0;
	if (child->fd_out != STDOUT_FILENO)
	{
		original = dup(STDOUT_FILENO);
		dup2(child->fd_out, STDOUT_FILENO);
	}
	exit_code = exec_single_builtin(child);
	if (child->fd_out != STDOUT_FILENO)
		dup2(original, STDOUT_FILENO);
	return (0 || exit_code);
}

int	exec_single_builtin(t_cmd *cmd)
{
	char	*cmd_name;
	t_ms	*ms;
	int		exit_code;

	cmd_name = cmd->cmd_arr[0];
	ms = get_ms();
	if (!ft_strncmp(cmd_name, "echo", 5))
		exit_code = ft_echo(cmd->cmd_arr);
	else if (!ft_strncmp(cmd_name, "cd", 3))
		exit_code = ft_cd(cmd->cmd_arr, ms->env);
	else if (!ft_strncmp(cmd_name, "pwd", 4))
		exit_code = ft_pwd();
	else if (!ft_strncmp(cmd_name, "export", 7))
		exit_code = ft_export(cmd->cmd_arr, ms->env);
	else if (!ft_strncmp(cmd_name, "unset", 6))
		exit_code = ft_unset(cmd->cmd_arr, ms->env);
	else if (!ft_strncmp(cmd_name, "env", 4))
		exit_code = print_env(ms->env);
	else if (!ft_strncmp(cmd_name, "exit", 5))
		exit_code = ft_exit(cmd->cmd_arr);
	return (exit_code);
}

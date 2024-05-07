/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 04:24:10 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/07 04:24:13 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child(t_cmd *child)
{
	int	exit_code;

	if (child->fd_in == -1 || child->fd_out == -1)
		child_free_exit(1);
	if (!child->cmd_arr[0] || child->cmd_arr[0][0] == '\0')
		child_free_exit(0);
	if (is_builtin(child->cmd_arr[0]))
	{
		exit_code = exec_single_builtin(child);
		child_free_exit(exit_code);
	}
	if (ft_strchr("./", child->cmd_arr[0][0]) && !valid_path(child, &exit_code))
		child_free_exit(exit_code);
	if (!child->fullpath)
	{
		ft_putstr_fd(child->cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		child_free_exit(127);
	}
	execve(child->fullpath, child->cmd_arr, NULL);
	perror("minishell");
	child_free_exit(1);
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
	exit_code = 0;
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
		exit_code = ft_env(ms->env);
	else if (!ft_strncmp(cmd_name, "exit", 5))
		exit_code = ft_exit(cmd->cmd_arr);
	return (exit_code);
}

bool	valid_path(t_cmd *child, int *exit_code)
{
	if (access(child->cmd_arr[0], F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(child->cmd_arr[0]);
		*exit_code = 127;
		return (0);
	}
	if (access(child->cmd_arr[0], X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(child->cmd_arr[0]);
		*exit_code = 126;
		return (0);
	}
	if (is_directory(child->cmd_arr[0]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(child->cmd_arr[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		*exit_code = 126;
		return (0);
	}
	child->fullpath = child->cmd_arr[0];
	return (1);
}

int	is_directory(char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
			return (1);
		else
			return (0);
	}
	return (0);
}

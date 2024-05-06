/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 09:57:00 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/06 09:57:06 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

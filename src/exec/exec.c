/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 07:38:11 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/18 13:00:23 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_exec_builtin(t_ms *shell)
{
	t_cmd	*cmd = shell->cmds->content;
	char	**args = cmd->cmd_arr;

	if (!args)
		return (1);
	if (strcmp(args[0], "pwd") == 0)
		ft_pwd();
	else if (strcmp(args[0], "env") == 0)
		print_env(shell->env);
	else if (strcmp(args[0], "cd") == 0)
		ft_cd(args, shell->env);
	else if (strcmp(args[0], "echo") == 0)
		ft_echo(args);
	else if (strcmp(args[0], "export") == 0)
		ft_export(args, shell->env);
	else if (strcmp(args[0], "exit") == 0)
	{
		free_cmd_list();
		free_env(shell->env);
		exit(0);
	}
	else
	{
		ft_free_args(args);
		return (1);
	}
	// ft_free_args(args);
	return (0);
}

void	exec_cmd(t_ms *shell)
{


	(void)shell;

}

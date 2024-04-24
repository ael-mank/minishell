/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 07:38:11 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/24 12:35:15 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_exec_builtin(t_ms *shell)
{
	t_cmd	*cmd = shell->cmds->content;
	char	**args = cmd->cmd_arr;

	if (!args)
		return (1);
	if (ft_strncmp("pwd", args[0], 3) == 0)
		ft_pwd();
	else if (ft_strncmp("env", args[0], 3) == 0)
		print_env(shell->env);
	else if (ft_strncmp("cd", args[0], 2) == 0)
		ft_cd(args, shell->env);
	else if (ft_strncmp("echo", args[0], 4) == 0)
		ft_echo(args);
	else if (ft_strncmp("export", args[0], 6) == 0)
		ft_export(args, shell->env);
	else if (ft_strncmp("exit", args[0], 4) == 0)
		ft_exit(shell);
	else
		return (1);
	// ft_free_args(args);
	return (0);
}

void	exec_cmd(t_ms *shell)
{


	(void)shell;

}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 07:38:11 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/17 13:13:52 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_exec_builtin(t_shell *shell)
{
	char	**args;

	args = ft_split(shell->line, ' ');
	if (!args)
		return(1);
	if (strcmp(args[0], "pwd") == 0)
		ft_pwd();
	else if (strcmp(args[0], "env") == 0)
		print_env(shell->env);
	else if (strcmp(args[0], "cd") == 0)
		ft_cd(args);
	else if (strcmp(args[0], "echo") == 0)
		ft_echo(args);
	else if (strcmp(args[0], "export") == 0)
		ft_export(args, shell->env);
	else if (strcmp(args[0], "exit") == 0)
	{
		ft_exit(shell, args);
		exit(0);
	}
	else
	{
		ft_free_args(args);
		return (1);
	}
	ft_free_args(args);
	return (0);
}

char    *match_env_var(char *name, int len, t_list *env)
{
    while (env && ft_strncmp(((t_env *)env->content)->name, name, len))
        env = env->next;
    if (!env || ft_strlen(((t_env *)env->content)->name) != (size_t)len)
        return ("");
    else
        return (((t_env *)env->content)->value);
}

int try_command(char *cmd, char **path)
{
	int i;
	char *full_path;
	int status;

	i = 0;
	while (path[i])
	{
		full_path = ft_strjoin(path[i], cmd);
		if (!full_path)
			return (1);
		status = execve(full_path, NULL, NULL);
		free(full_path);
		if (status == 0)
			return (0);
		i++;
	}
	return (1);
}

void	exec_cmd(t_shell *shell)
{
	(void)shell;
	return ;
	char **path;

	path = ft_split(match_env_var("PATH", 4, shell->env), ':');
	if (!path)
		return ;
	try_command()
}

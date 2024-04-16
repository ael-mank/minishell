/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 07:38:11 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/05 07:38:16 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ADD = init env as linked lists

int	check_exec_builtin(t_shell *shell)
{
	char **args;

	args = ft_split(shell->line, ' ');
	if (strcmp(args[0], "pwd") == 0)
		ft_pwd();
	else if (strcmp(args[0], "env") == 0)
		print_env(shell->env);
	else if (strcmp(args[0], "cd") == 0)
		ft_cd(args);
	else if (strcmp(args[0], "echo") == 0)
		ft_echo(args);
	else if (strcmp(args[0], "exit") == 0)
	{
		free(shell->line);
		return (1);
	}
	else
		return (1);
	return (0);
}

//marche pas

void exec_cmd(t_shell *shell)
{
    pid_t   pid;
    int     status;
    char    *argv[] = {shell->line, NULL};  // Construct the argv array

    pid = fork();
    printf("im in exec_cmd\n");
    if (pid == 0)
    {
        execve(shell->line, argv, shell->env);
        perror("execve");
        exit(1);
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else
    {
        waitpid(pid, &status, 0);
    }
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/16 14:14:42 by ael-mank         ###   ########.fr       */
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
		free_env(shell->env);
		ft_free_args(args);
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

//marche pas

void exec_cmd(t_shell *shell)
{
	(void)shell;
}

void	mini_loop(t_shell *shell)
{
	while (1)
	{
		shell->line = readline("minishell> ");
		if (!shell->line)
			break ;
		if (check_syntax_errors(shell->line))
		{
			free(shell->line);
			continue;
		}
		add_history(shell->line);
		if (check_exec_builtin(shell))
			exec_cmd(shell);
		free(shell->line);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_shell *shell;

	(void)argc;
	(void)argv;
	shell = malloc(sizeof(t_shell));
	init_env(env, shell);
	setup_signals();
	mini_loop(shell);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/05 07:38:03 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	shell->env = env;
	setup_signals();
	mini_loop(shell);
	return (0);
}
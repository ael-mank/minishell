/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/18 11:27:13 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_loop(t_shell *shell)
{
	while (1)
	{
		shell->line = readline("minishell> ");
		if (!shell->line)
			return(0);
		if (ft_strlen(shell->line) == 0)
		{
			free(shell->line);
			continue;
		}
		if (check_syntax_errors(shell->line))
		{
			free(shell->line);
			continue ;
		}
		add_history(shell->line);
		if (check_exec_builtin(shell))
			exec_cmd(shell);
		free(shell->line);
	}
	return(1);
}

int	main(int argc, char **argv, char **env)
{
	t_shell *shell;

	(void)argc;
	(void)argv;
	shell = malloc(sizeof(t_shell));
	init_env(env, shell);
	setup_signals();
	if(!mini_loop(shell))
		ft_exit(shell, 0);
	return (0);
}

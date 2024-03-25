/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/25 18:01:21 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//ADD = init env as linked lists

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	setup_signals();
	while (1)
	{
		char *line = readline("minishell> ");
		if (!line)
			break ;
		add_history(line);
		check_syntax_errors(line);
		//Add check for builtin function
		// if (strcmp(line, "pwd") == 0)
		// 	ft_pwd();
		// else if (strcmp(line, "env") == 0)
		// 	print_env(env);
		// else if (strcmp(line, "cd") == 0)
		// 	ft_cd();
		// else if (strcmp(line, "echo") == 0)
		// 	ft_echo(NULL);
		// else if (strcmp(line, "exit") == 0)
		// {
		// 	free(line);
		// 	break ;
		// }
		// else
		// 	printf("Error: command not found: %s\n", line);
		free(line);
	}
	return (0);
}
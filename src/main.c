/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/13 09:49:00 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	while (1)
	{
		char *line = readline("minishell> ");
		if (!line)
			break ;
		add_history(line);
		if (strcmp(line, "pwd") == 0)
		{
			pid_t pid = fork();
			if (pid < 0)
			{
				perror("fork failed");
			}
			else if (pid == 0)
			{
				char *argv[] = {"/bin/pwd", NULL};
				execve(argv[0], argv, NULL);
				perror("execve failed");
				exit(1);
			}
			else
			{
				waitpid(pid, NULL, 0);
			}
		}
		else
		{
			printf("Error: command not found: %s\n", line);
		}
		free(line);
	}
}
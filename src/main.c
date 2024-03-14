/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/14 17:54:16 by ael-mank         ###   ########.fr       */
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
			ft_pwd();
		}
		else
		{
			printf("Error: command not found: %s\n", line);
		}
		free(line);
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 06:49:21 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/05 08:24:16 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int i;
	int end_line;

	i = 1;
	if (args[i][0] == '-' && args[i][1] == 'n')
	{
		end_line = 0;
		i++;
	}
	else
		end_line = 1;
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (end_line)
		ft_printf("\n");
	return (0);
}
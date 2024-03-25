/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 06:53:58 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/25 05:42:18 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(void)
{
	//AJOUTER UPDATE OLD_PWD
	if (args[1] == NULL)
		fprintf(stderr, "ft_cd: expected argument\n");
	else
	{
		if (chdir(args[1]) != 0)
			perror("ft_cd");
	}
	return (1);
}
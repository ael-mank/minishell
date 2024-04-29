/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 17:48:25 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/29 10:46:08 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	err(const char *msg, int ret)
{
	if (msg)
		ft_printf("%s\n", msg);
	return (ret);
}

int	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		return (err("Error getting current directory", -1));
	ft_printf("%s\n", path);
	free(path);
	return (0);
}

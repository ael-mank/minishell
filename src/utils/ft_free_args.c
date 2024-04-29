/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:33:42 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/29 10:55:18 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_free_args(char **str)
{
	int	i;

	i = 0;
	if (NULL == str || NULL == *str)
		return (NULL);
	while (str[i])
		free(str[i++]);
	free(str);
	return (NULL);
}

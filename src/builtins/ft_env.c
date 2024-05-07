/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 07:04:36 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/30 20:57:15 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_list *env)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		ft_printf("%s=%s\n", ((t_env *)tmp->content)->name,
			((t_env *)tmp->content)->value);
		tmp = tmp->next;
	}
	return (0);
}

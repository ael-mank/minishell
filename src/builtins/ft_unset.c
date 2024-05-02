/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:46:57 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/02 11:16:04 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_relink(t_list *prev, t_list *current)
{
	t_env	*env_var;

	prev->next = current->next;
	env_var = (t_env *)current->content;
	free(env_var->name);
	free(env_var->value);
	free(current->content);
	free(current);
}

int	ft_unset(char **args, t_list *env)
{
	t_list	*prev;
	t_list	*current;
	t_env	*env_var;

	prev = NULL;
	current = env;
	env_var = NULL;
	if (!args[1])
		return (0);
	while (current)
	{
		env_var = (t_env *)current->content;
		if (strcmp(env_var->name, args[1]) == 0)
		{
			if (prev)
				free_and_relink(prev, current);
			else
				env = current->next;
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

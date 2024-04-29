/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_env_val.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 10:32:25 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/29 10:55:04 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	edit_env_value(t_list *env, char *name, char *new_value)
{
	t_env	*env_var;
	
	while (env)
	{
		env_var = (t_env *)env->content;
		if (strcmp(env_var->name, name) == 0)
		{
			free(env_var->value);
			env_var->value = strdup(new_value);
			if (!env_var->value)
				return ;
			break ;
		}
		env = env->next;
	}
}

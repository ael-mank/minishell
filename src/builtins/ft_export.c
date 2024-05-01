/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:22:08 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/30 20:55:29 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_name_valid(const char *name)
{
	if (!name)
		return (0);
	int(i) = 0;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args, t_list *env)
{
	t_env	*env_var;
	t_list	*env_var_node;
	char	**name_value;

	name_value = ft_split(args[1], '=');
	if (!is_name_valid(name_value[0]))
	{
		ft_free_args(name_value);
		return (1);
	}
	env_var = malloc(sizeof(t_env));
	if (!env_var)
		return (1);
	env_var->name = ft_strdup(name_value[0]);
	if (!env_var->name)
		return (1);
	env_var->value = ft_strdup(name_value[1]);
	if (!env_var->value)
		return (1);
	ft_free_args(name_value);
	env_var_node = ft_lstnew(env_var);
	if (!env_var_node)
		return (1);
	ft_lstadd_back(&env, env_var_node);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:22:08 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/29 10:24:57 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalnum_name(int c)
{
	if (c == '_')
		return (1);
	if (((c >= 48) && (c <= 57))
		|| ((c >= 65) && (c <= 90))
		|| ((c >= 97) && (c <= 122)))
		return (1);
	else
		return (0);
}

int is_name_valid(const char *name)
{
	if (!name)
		return(0);
	int (i) = 0;
	while (name[i])
	{
		if (!ft_isalnum_name(name[i]))
			return(0);
		i++;
	}
	return(1);
}

void	ft_export(char **args, t_list *env)
{
	t_env	*env_var;
	t_list	*env_var_node;
	char **name_value;
	
	name_value = ft_split(args[1], '=');
	if (!is_name_valid(name_value[0]))
	{
		ft_free_args(name_value);
		return ;
	}
	env_var = malloc(sizeof(t_env));
	if (!env_var)
		return ;
	env_var->name = ft_strdup(name_value[0]);
	if (!env_var->name)
		return ;
	env_var->value = ft_strdup(name_value[1]);
	if (!env_var->value)
		return ;
	ft_free_args(name_value);
	env_var_node = ft_lstnew(env_var);
	if (!env_var_node)
		return ;
	ft_lstadd_back(&env, env_var_node);
}

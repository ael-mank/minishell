/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 07:54:47 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/16 13:55:52 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	init_env(char **envp, t_shell *shell)
{
	t_env	*env_var;
	t_list	*env_var_node;
    int     i;
	int		j;

    i = -1;
	shell->env = NULL;
	while (envp[++i])
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		env_var = malloc(sizeof(t_env));
		if (!env_var)
			return (0);
		env_var->name = ft_substr(envp[i], 0, j);
		if (!env_var->name)
			return (0);
		env_var->value = ft_strdup(&envp[i][j + 1]);
		if (!env_var->value)
			return (0);
		env_var_node = ft_lstnew(env_var);
		if (!env_var_node)
			return (0);
		ft_lstadd_back(&shell->env, env_var_node);
	}
	return (1);
}

//code free env

void	free_env(t_list *env)
{
	t_list	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(((t_env *)tmp->content)->name);
		free(((t_env *)tmp->content)->value);
		free(tmp->content);
		free(tmp);
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 07:54:47 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/17 09:43:55 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	init_env(char **envp, t_shell *shell)
{
	t_env	*env_var;
	t_list	*env_var_node;
	int		i;
	int		j;

	i = -1;
	shell->env = NULL;
	while (envp[++i])
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		if (!(env_var = malloc(sizeof(t_env))))
			return (0);
		if (!(env_var->name = ft_substr(envp[i], 0, j)))
			return (0);
		if (!(env_var->value = ft_strdup(&envp[i][j + 1])))
			return (0);
		if (!(env_var_node = ft_lstnew(env_var)))
			return (0);
		ft_lstadd_back(&shell->env, env_var_node);
	}
	return (1);
}

void	free_env(t_list *env)
{
	t_list *tmp;

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
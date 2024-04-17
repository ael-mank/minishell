/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 07:54:47 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/17 10:39:25 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_var(char *envp)
{
	int		j;
	t_env	*env_var;

	j = 0;
	while (envp[j] != '=')
		j++;
	env_var = malloc(sizeof(t_env));
	if (!env_var)
		return (NULL);
	env_var->name = ft_substr(envp, 0, j);
	if (!env_var->name)
		return (NULL);
	env_var->value = ft_strdup(&envp[j + 1]);
	if (!env_var->value)
		return (NULL);
	return (env_var);
}

bool	init_env(char **envp, t_shell *shell)
{
	int		i;
	t_env	*env_var;
	t_list	*env_var_node;

	i = -1;
	shell->env = NULL;
	while (envp[++i])
	{
		env_var = create_env_var(envp[i]);
		if (!env_var)
			return (0);
		env_var_node = ft_lstnew(env_var);
		if (!env_var_node)
			return (0);
		ft_lstadd_back(&shell->env, env_var_node);
	}
	return (1);
}

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

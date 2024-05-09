/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 07:04:36 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/09 16:58:39 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_list *env, char **args)
{
	t_list	*tmp;
	int		argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 1)
	{
		ft_printf("env: '%s': No such file or directory\n", args[1]);
		return (127);
	}
	tmp = env;
	while (tmp)
	{
		ft_printf("%s=%s\n", ((t_env *)tmp->content)->name,
			((t_env *)tmp->content)->value);
		tmp = tmp->next;
	}
	return (0);
}

void	gen_env_dict(char **env_array, t_list *env)
{
	t_env	*data;
	char	*tmp;
	int		i;

	i = 0;
	while (env)
	{
		data = env->content;
		tmp = ft_strjoin(data->name, "=");
		env_array[i] = ft_strjoin(tmp, data->value);
		if (tmp)
			free(tmp);
		env = env->next;
		i++;
	}
	env_array[i] = NULL;
}

char	**create_env_array(t_list *env)
{
	char	**env_array;
	int		len;

	len = 1;
	while (env)
	{
		len++;
		env = env->next;
	}
	env_array = malloc(sizeof(char *) * len);
	if (!env_array)
		return (NULL);
	return (env_array);
}

char	**env_to_array(void)
{
	t_ms	*ms;
	t_list	*env;
	char	**env_array;

	ms = get_ms();
	env = ms->env;
	env_array = create_env_array(env);
	if (env_array)
		gen_env_dict(env_array, env);
	return (env_array);
}

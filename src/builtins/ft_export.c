/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:22:08 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/09 16:59:12 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_env_var(char *str, t_list **env)
{
	int		i;
	t_env	*env_var;
	t_list	*env_node;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	env_var = malloc(sizeof(t_env));
	if (!env_var)
		return (1);
	env_var->name = ft_substr(str, 0, i);
	if (!env_var->name)
		return (1);
	env_var->value = ft_strdup(&str[i + 1]);
	if (!env_var->value)
		return (1);
	env_node = ft_lstnew(env_var);
	if (!env_var)
		return (1);
	ft_lstadd_back(env, env_node);
	return (0);
}

bool	has_valid_name(const char *str)
{
	int	i;

	if (!str || !(*str) || *str == '=' || ft_isdigit(*str))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	add_or_edit_env_var(char *str, t_list **env_list)
{
	int		i;
	t_list	*curr;
	t_env	*env_var;

	i = 0;
	curr = *env_list;
	while (str[i] && str[i] != '=')
		i++;
	while (curr)
	{
		env_var = (t_env *)curr->content;
		if (!ft_strncmp(env_var->name, str, i)
			&& ft_strlen(env_var->name) == (size_t)i)
		{
			free(env_var->value);
			env_var->value = ft_strdup(&str[i + 1]);
			return ;
		}
		curr = curr->next;
	}
	if (!curr)
		add_env_var(str, env_list);
}

void	export_invalid_msg(char *arg)
{
	char	*tmp;
	char	*final;

	tmp = ft_strjoin("minishell: export: `", arg);
	final = ft_strjoin(tmp, "': not a valid identifier\n");
	ft_putstr_fd(final, 2);
	free(tmp);
	free(final);
}

int	ft_export(char **args, t_list *env)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	if (!args[1])
		ft_env(env, args);
	i = 0;
	while (args[++i])
	{
		if (!ft_strchr(args[i], '=') && has_valid_name(args[i]))
			continue ;
		if (!has_valid_name(args[i]))
		{
			export_invalid_msg(args[i]);
			exit_code = 1;
			continue ;
		}
		add_or_edit_env_var(args[i], &env);
	}
	return (exit_code);
}

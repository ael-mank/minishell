/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 06:53:58 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/02 10:04:49 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	edit_env_value(t_list *env, char *name, char *new_value)
{
	t_env	*env_var;
	int		len;

	len = ft_strlen(name);
	while (env)
	{
		env_var = (t_env *)env->content;
		if (ft_strncmp(env_var->name, name, len + 1) == 0)
		{
			free(env_var->value);
			env_var->value = ft_strdup(new_value);
			if (!env_var->value)
				return ;
			break ;
		}
		env = env->next;
	}
}

int	cd_command(t_list *env, char *path)
{
	char	*old_pwd;
	char	*pwd;

	old_pwd = getcwd(NULL, 0);
	edit_env_value(env, "OLDPWD", old_pwd);
	free(old_pwd);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	pwd = getcwd(NULL, 0);
	edit_env_value(env, "PWD", pwd);
	free(pwd);
	return (0);
}

int	ft_cd(char **args, t_list *env)
{
	int	args_count;

	args_count = 0;
	while (args[args_count])
		args_count++;
	if (args_count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	else
	{
		if (args[1] == NULL)
			return (cd_command(env, "/home"));
		else
			return (cd_command(env, args[1]));
	}
	return (1);
}

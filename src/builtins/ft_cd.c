/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 06:53:58 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/30 00:40:24 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **args, t_list *env)
{
	char	*old_pwd;
	char	*pwd;

	old_pwd = getcwd(NULL, 0);
	if (args[1] == NULL)
	{
		edit_env_value(env, "OLDPWD", old_pwd);
		free(old_pwd);
		if (chdir("/home") != 0)
			perror("ft_cd");
		pwd = getcwd(NULL, 0);
		edit_env_value(env, "PWD", pwd);
		free(pwd);
	}
	else
	{
		edit_env_value(env, "OLDPWD", old_pwd);
		free(old_pwd);
		if (chdir(args[1]) != 0)
			perror("ft_cd");
		pwd = getcwd(NULL, 0);
		edit_env_value(env, "PWD", pwd);
		free(pwd);
	}
	return (1);
}

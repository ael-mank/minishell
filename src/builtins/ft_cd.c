/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 06:53:58 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/30 21:02:29 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_command(t_list *env, char *path)
{
	char	*old_pwd;
	char	*pwd;
	
	old_pwd = getcwd(NULL, 0);
	edit_env_value(env, "OLDPWD", old_pwd);
	free(old_pwd);
	if (chdir(path) != 0)
	{
		perror("ft_cd");
		return (1);
	}
	pwd = getcwd(NULL, 0);
	edit_env_value(env, "PWD", pwd);
	free(pwd);
	return (0);
}

int	ft_cd(char **args, t_list *env)
{
	if (args[1] == NULL)
		return(cd_command(env, "/home"));
	else
		return(cd_command(env, args[1]));
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:56:01 by yrigny            #+#    #+#             */
/*   Updated: 2024/04/12 16:56:03 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		while (!pwd)
		{
			chdir("..");
			pwd = getcwd(NULL, 0);
		}
	}
	return (pwd);
}

char	*get_prompt(void)
{
	char	*pwd;
	char	*user;
	char	*temp_1;
	char	*temp_2;
	char	*finalprompt;

	user = match_env_var("USER", 4);
	pwd = get_pwd();
	temp_1 = ft_strjoin(user, "@");
	temp_2 = ft_strjoin(temp_1, pwd);
	free(temp_1);
	free(pwd);
	finalprompt = ft_strjoin(temp_2, "$ ");
	free(temp_2);
	return (finalprompt);
}

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

int	empty_line(char *line)
{
	char	*temp;

	temp = line;
	while (*temp && ft_isspace(*temp))
		temp++;
	if (*temp == '\0')
	{
		free(line);
		return (1);
	}
	return (0);
}

void	child_free_exit(int exit_code)
{
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(exit_code);
}

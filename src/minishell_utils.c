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

void	free_cmd_list(void)
{
	t_ms	*ms;
	t_list	**cmd_list;
	t_list	*next_cmd_node;

	ms = get_ms();
	cmd_list = &ms->cmds;
	while (*cmd_list)
	{
		next_cmd_node = (*cmd_list)->next;
		free_str_arr(&((t_cmd *)(*cmd_list)->content)->cmd_arr);
		free(((t_cmd *)(*cmd_list)->content)->fullpath);
		free_tokens(&((t_cmd *)(*cmd_list)->content)->redir);
		free((*cmd_list)->content);
		free(*cmd_list);
		*cmd_list = next_cmd_node;
	}
}

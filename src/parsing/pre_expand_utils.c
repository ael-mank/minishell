/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:29:13 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/03 11:22:08 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_expandable_dollar_str(t_token *token, int *dollar_pos)
{
	char	*s;
	int		quote_open;

	s = token->value;
	quote_open = 0;
	while (*s)
	{
		if (*s == '\'' || *s == '\"')
		{
			if (quote_open == 0)
				quote_open = *s;
			else if (quote_open == *s)
				quote_open = 0;
		}
		else if (quote_open != '\'' && *s == '$' 
			&& ((ft_isalnum(*(s + 1)) || *(s + 1) == '_') || *(s + 1) == '?'))
		{
			*dollar_pos = s - token->value;
			return (1);
		}
		s++;
	}
	return (0);
}

char	*assemble_new_str(char *old_str, char *value, int head, int end)
{
	int		value_len;
	int		tail_len;
	int		total_len;
	char	*new_str;

	value_len = ft_strlen(value);
	tail_len = ft_strlen(old_str) - (end + 1);
	total_len = ft_strlen(old_str) - (end - head + 1) + value_len;
	new_str = malloc(total_len + 1);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, old_str, head + 1);
	ft_strlcpy(new_str + head, value, value_len + 1);
	ft_strlcpy(new_str + head + value_len, old_str + end + 1, tail_len + 1);
	return (new_str);
}

char	*assemble_new_str2(char *old_str, int pair_of_quotes)
{
	int		new_str_len;
	char	*new_str;
	char	quote;
	int		i;

	new_str_len = ft_strlen(old_str) - 2 * pair_of_quotes;
	new_str = malloc(new_str_len + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (*old_str)
	{
		if (*old_str == '\'' || *old_str == '\"')
		{
			quote = *old_str;
			while (*(++old_str) != quote)
				new_str[i++] = *old_str;
		}
		else
			new_str[i++] = *old_str;
		old_str++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	**get_paths_array(void)
{
	t_list	*env;
	char	**paths;

	env = get_ms()->env;
	while (env && ft_strncmp(((t_env *)env->content)->name, "PATH", 5))
		env = env->next;
	if (env && !ft_strncmp(((t_env *)env->content)->name, "PATH", 5))
		paths = ft_split(((t_env *)env->content)->value, ':');
	else
		paths = NULL;
	return (paths);
}

void	assemble_fullpath(t_cmd *cmd, char *cmd_name, char **paths)
{
	char	*tmp_cmd_path;
	int		i;

	i = -1;
	while (!cmd->fullpath && cmd_name && paths[++i])
	{
		tmp_cmd_path = ft_strjoin(paths[i], "/");
		cmd->fullpath = ft_strjoin(tmp_cmd_path, cmd_name);
		if (access(cmd->fullpath, F_OK) != 0)
		{
			free(cmd->fullpath);
			cmd->fullpath = NULL;
		}
		free(tmp_cmd_path);
	}
}

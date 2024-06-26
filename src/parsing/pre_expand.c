/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:28:46 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/09 21:05:38 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_dollar_str(char **p_str)
{
	int	dollar_pos;

	dollar_pos = 0;
	while (has_expandable_dollar_str(*p_str, &dollar_pos))
	{
		expand_env_var(p_str, dollar_pos);
	}
}

void	expand_env_var(char **p_old_str, int head)
{
	int		end;
	char	*old_str;
	char	*env_var_value;
	char	*new_str;

	old_str = *p_old_str;
	end = head + 1;
	if (old_str[end] != '?')
	{
		while (ft_isalnum(old_str[end + 1]) || old_str[end + 1] == '_')
			end++;
	}
	env_var_value = match_env_var(&old_str[head + 1], end - head);
	new_str = assemble_new_str(old_str, env_var_value, head, end);
	free(old_str);
	*p_old_str = new_str;
}

char	*match_env_var(char *name, int len)
{
	t_list	*env;
	int		last_exit;

	if (len == 1 && !ft_strncmp(name, "?", 1))
	{
		last_exit = get_ms()->last_exit;
		if (last_exit == 1)
			return ("1");
		if (last_exit == 2)
			return ("2");
		if (last_exit == 126)
			return ("126");
		if (last_exit == 127)
			return ("127");
		if (last_exit == 130)
			return ("130");
		return ("0");
	}
	env = get_ms()->env;
	while (env && ft_strncmp(((t_env *)env->content)->name, name, len))
		env = env->next;
	if (!env)
		return ("");
	else
		return (((t_env *)env->content)->value);
}

void	remove_quotes(t_token *token, char *old_str)
{
	int		i;
	int		pair_of_quotes;
	char	quote;
	char	*new_str;

	i = -1;
	pair_of_quotes = 0;
	while (old_str[++i])
	{
		if (old_str[i] == '\'' || old_str[i] == '\"')
		{
			quote = old_str[i];
			i += 1;
			while (old_str[i] != quote)
				i++;
			if (old_str[i] == quote)
				pair_of_quotes += 1;
		}
	}
	if (!pair_of_quotes && token->type == TOKEN_REDIR_HEREDOC)
		token->type = TOKEN_REDIR_HEREDOC_WEXP;
	new_str = assemble_new_str2(old_str, pair_of_quotes);
	free(old_str);
	token->value = new_str;
}

void	expand_fullpath(t_list *cmds)
{
	char	**paths;
	t_cmd	*cmd;

	paths = get_paths_array();
	if (!paths)
		return ;
	while (cmds)
	{
		cmd = (t_cmd *)cmds->content;
		if (cmd->cmd_arr && !is_builtin(cmd->cmd_arr[0])
			&& cmd->cmd_arr[0][0] != '.' && cmd->cmd_arr[0][0] != '/')
			assemble_fullpath(cmd, cmd->cmd_arr[0], paths);
		cmds = cmds->next;
	}
	free_str_arr(&paths);
}

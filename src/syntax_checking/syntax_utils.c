/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 05:53:13 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/02 10:07:53 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	increment_quote_count(char c, int *single_quote_count,
		int *double_quote_count)
{
	if (c == '\"')
		(*double_quote_count)++;
	else if (c == '\'')
		(*single_quote_count)++;
}

const char	*skip_whitespace(const char *input)
{
	while (*input && ft_isspace(*input))
		input++;
	return (input);
}

int	is_invalid_redirection(const char **input)
{
	const char	*operator_start = *input;

	(*input)++;
	if (*operator_start == **input)
		(*input)++;
	*input = skip_whitespace(*input);
	if (**input == '\0' || ft_strchr("><|", **input))
		return (1);
	return (0);
}

char	*trim_line(char *str)
{
	int		i_start;
	int		i_end;
	char	*trimmed;

	if (!str)
		return (NULL);
	if (ft_strlen(str) == 0)
		return (ft_strdup(str));
	i_start = 0;
	i_end = ft_strlen(str) - 1;
	while (str[i_start] && ft_isspace(str[i_start]))
		i_start++;
	while (i_end && ft_isspace(str[i_end]))
		i_end--;
	if (i_start > i_end)
		trimmed = malloc(1);
	else
		trimmed = malloc(i_end - i_start + 2);
	if (!trimmed)
		return (NULL);
	if (i_start > i_end)
		trimmed[0] = '\0';
	else
		ft_strlcpy(trimmed, &str[i_start], i_end - i_start + 2);
	return (trimmed);
}

int	has_unclosed_quotes(const char *input)
{
	char	open_quote;

	open_quote = 0;
	while (*input)
	{
		if (ft_strchr("\'\"", *input))
		{
			if (open_quote == *input)
				open_quote = 0;
			else if (open_quote == 0)
				open_quote = *input;
		}
		input++;
	}
	return (open_quote != 0);
}

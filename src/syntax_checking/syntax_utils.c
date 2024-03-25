/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 05:53:13 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/25 17:57:46 by ael-mank         ###   ########.fr       */
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

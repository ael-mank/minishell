/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 05:53:13 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/25 07:43:43 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_quote_counts(char c, int *s_q_count, int *d_q_count)
{
	if (c == '\"')
		(*d_q_count)++;
	else if (c == '\'')
		(*s_q_count)++;
}

const char	*skip_spaces(const char *input)
{
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	return (input);
}

// Function to check if the current character sequence is an invalid operator
int is_invalid_operator(const char **input)
{
	const char *operator_start;

	operator_start = *input;
	(*input)++;
	if (*operator_start == **input)
		(*input)++;
	*input = skip_spaces(*input); // Skip any spaces or tabs
	if (**input == '\0' || **input == '>' || **input == '<' || **input == '|') // If the next character is the end of the string or one of the operators: '>', '<', '|'
		return (1); // Return 1 to indicate that it is an invalid operator
	return (0); // Return 0 to indicate that it is a valid operator
}

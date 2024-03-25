/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 05:52:01 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/25 07:46:10 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error_checker(const char *input)
{
	if (has_unclosed_quotes(input))
	{
		ft_putstr_fd("Syntax error: unclosed quote\n", STDERR_FILENO);
		return (1);
	}
	if (has_invalid_redirections(input))
	{
		ft_putstr_fd("Syntax error: invalid redirection\n", STDERR_FILENO);
		return (1);
	}
	if (has_misplaced_operators(input))
	{
		ft_putstr_fd("Syntax error: misplaced operator\n", STDERR_FILENO);
		return (1);
	}
	if (has_logical_operators(input))
	{
		ft_putstr_fd("Error: Logical operators '&&' and '||' \
			are not supported.\n", STDERR_FILENO);
		return (1);
	}
	return (0); // Returns 0 if there are no syntax errors
}

int has_unclosed_quotes(const char *input)
{
	char open_quote;

	open_quote = 0;
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			if (open_quote == *input)
				open_quote = 0;
			else if (!open_quote)
				open_quote = *input;
		}
		input++;
	}
	// Returns 1 if there is an unclosed quote, 0 otherwise
	return (open_quote != 0);
}

//single quote count and double quote count

int	has_invalid_redirections(const char *input)
{
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if ((!(s_q_count % 2) && !(d_q_count % 2))
			&& (*input == '>' || *input == '<'))
		{
			if (is_invalid_operator(&input))
				return (1); // Returns 1 if there are invalid redirections
		}
		else
			input++;
	}
	// Returns 0 if there are no invalid redirections, 1 otherwise
	return (0);
}

int	has_misplaced_operators(const char *input)
{
	int	expect_command_next;
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	expect_command_next = 0;
	if (*input == '|' || *input == '&')
		return (1); // Returns 1 if there are misplaced operators
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (*input == '|' && !(s_q_count % 2) && !(d_q_count % 2))
		{
			if (expect_command_next)
				return (1); // Returns 1 if there are misplaced operators
			expect_command_next = 1;
		}
		else if (!ft_isspace(*input))
			expect_command_next = 0;
		input++;
	}
	if (expect_command_next)
		return (1); // Returns 1 if there are misplaced operators
	return (0); // Returns 0 if there are no misplaced operators
}

int	has_logical_operators(const char *input)
{
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (!(d_q_count % 2) && !(s_q_count % 2)
			&& ((*input == '&' && *(input + 1) == '&')
				|| (*input == '|' && *(input + 1) == '|')))
			return (1); // Returns 1 if there are logical operators
		input++;
	}
	return (0); // Returns 0 if there are no logical operators
}
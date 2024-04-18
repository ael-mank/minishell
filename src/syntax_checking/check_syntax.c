/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 05:52:01 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/18 11:26:25 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			else
				open_quote = *input;
		}
		input++;
	}
	return (open_quote != 0);
}

int	has_invalid_redirections(const char *input)
{
	int	single_quote_count;
	int	double_quote_count;

	single_quote_count = 0;
	double_quote_count = 0;
	while (*input)
	{
		increment_quote_count(*input, &single_quote_count, &double_quote_count);
		if (!(single_quote_count % 2) && !(double_quote_count % 2)
			&& ft_strchr("><", *input))
		{
			if (is_invalid_redirection(&input))
				return (1);
		}
		else
			input++;
	}
	return (0);
}

int	has_misplaced_operators(const char *input)
{
	int	expect_command_next;
	int	sq_count;
	int	dq_count;

	expect_command_next = 0;
	sq_count = 0;
	dq_count = 0;
	if (ft_strchr("|&", *input))
		return (1);
	while (*input)
	{
		increment_quote_count(*input, &sq_count, &dq_count);
		if (*input == '|' && !(sq_count % 2) && !(dq_count % 2))
		{
			if (expect_command_next)
				return (1);
			expect_command_next = 1;
		}
		else if (!ft_isspace(*input))
			expect_command_next = 0;
		input++;
	}
	if (expect_command_next)
		return (1);
	return (0);
}

int	has_logical_operators(const char *input)
{
	int	sq_count;
	int	dq_count;

	sq_count = 0;
	dq_count = 0;
	while (*input)
	{
		increment_quote_count(*input, &sq_count, &dq_count);
		if (!(dq_count % 2) && !(sq_count % 2) && ((*input == '&' && *(input
						+ 1) == '&') || (*input == '|' && *(input + 1) == '|')))
			return (1);
		input++;
	}
	return (0);
}

int	check_syntax_errors(const char *input)
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
		ft_putstr_fd("Error: Logical operators '&&' and '||' are not supported.\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

char	*trim_line(char *str)
{
    int     i_start;
    int     i_end;
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
    if (i_start > i_end) // str has only whitespaces
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

t_token	*check_syntax_and_tokenize(char *line)
{
	char	*trimmed_line;
	t_token	*tokens;

	trimmed_line = trim_line(line);
	tokens = NULL;
	free(line);
	if (!trimmed_line)
		return (NULL);
	if (check_syntax_errors(trimmed_line))
	{
		free(trimmed_line);
		return (NULL);
	}
	tokens = tokenize(trimmed_line);
	free(trimmed_line);
	return (tokens);
}
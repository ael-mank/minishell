/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:36:17 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/25 06:58:31 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>	
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "libft.h"

// Builtins
int		ft_pwd(void);
int		ft_cd(char **args);
int		ft_echo(char **args);
void	print_env(char **env);

// Signals
void	setup_signals(void);

//Syntax
int			syntax_error_checker(const char *input);
int			has_unclosed_quotes(const char *input);
int			has_invalid_redirections(const char *input);
int			has_misplaced_operators(const char *input);
int			has_logical_operators(const char *input);
void		update_quote_counts(char c, int *s_q_count, int *d_q_count);
const char	*skip_spaces(const char *input);
int			is_invalid_operator(const char **input);

#endif
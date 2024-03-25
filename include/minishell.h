/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:36:17 by ael-mank          #+#    #+#             */
/*   Updated: 2024/03/25 18:00:39 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// Builtins
int			ft_pwd(void);
int			ft_cd(char **args);
int			ft_echo(char **args);
void		print_env(char **env);

// Signals
void		setup_signals(void);

// Syntax
int			check_syntax_errors(const char *input);
void		increment_quote_count(char c, int *single_quote_count,
				int *double_quote_count);
const char	*skip_whitespace(const char *input);
int			is_invalid_redirection(const char **input);

#endif
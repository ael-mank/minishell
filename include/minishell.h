/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:36:17 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/18 11:13:26 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_env
{
	char	*name;
	char	*value;
}			t_env;

typedef struct s_shell
{
	char	*line;
	t_list	*env;

}			t_shell;

// INIT
bool		init_env(char **env, t_shell *shell);
void		free_env(t_list *env);

// EXEC

int		check_exec_builtin(t_shell *shell);
void	exec_cmd(t_shell *shell);

// Builtins
int			ft_pwd(void);
int			ft_cd(char **args, t_list *env);
int			ft_echo(char **args);
void		print_env(t_list *env);
void		ft_export(char **args, t_list *env);
void		ft_exit(t_shell *shell, char **args);

// Signals
void		setup_signals(void);

// Syntax
int			check_syntax_errors(const char *input);
void		increment_quote_count(char c, int *single_quote_count,
				int *double_quote_count);
const char	*skip_whitespace(const char *input);
int			is_invalid_redirection(const char **input);

// utils
void		*ft_free_args(char **str);
void edit_env_value(t_list *env, char *name, char *new_value);

#endif
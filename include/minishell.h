/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:36:17 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/30 21:02:53 by ael-mank         ###   ########.fr       */
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
# include <stdbool.h>

# ifndef MAX_PIPE
#  define MAX_PIPE 1024
# endif

typedef enum e_token_type
{
    TOKEN_WORD,         // cmd and args
    TOKEN_PIPE,         // '|'
    TOKEN_REDIR_IN,     // '<'
    TOKEN_REDIR_HEREDOC,// '<<'
    TOKEN_REDIR_OUT,    // '>'
    TOKEN_REDIR_APPEND, // '>>'
    TOKEN_ENV_VAR,      // '$'
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct s_cmd
{
    char    **cmd_arr;
    char    *fullpath;
    t_token *redir_in;
    t_token *redir_out;
    int     fd_in;
    int     fd_out;
}   t_cmd;

typedef struct s_env
{
    char    *name;
    char    *value;
}   t_env;

typedef struct s_pipe
{
    int     fd[2];
    pid_t   pid;
}   t_pipe;

typedef struct s_ms
{
    t_list  *env;
    t_list  *cmds;
    pid_t   pids[1024];
	t_pipe  pipe[MAX_PIPE];
    int     last_exit;
}   t_ms;

/* main process */
void	shell_routine(void);
t_ms	*get_ms(void);
int		empty_line(char *line);

/* syntax check */
t_token	*check_syntax_and_tokenize(char *line);
char	*trim_line(char *str);
bool	syntax_error(char *line);
bool	has_unclosed_quote(char *line);
bool	has_unclosed_parenthesis(char *line);
bool	has_invalid_redir(char *line);
bool	has_misplaced_oparator(char *line);
// bool	has_logical_oparator(char *line);
void	skip_space(char **line);
void	skip_quoted(char **line);
void	syntax_error_pos(char *pos);

/* execution */
void	exec_manager(void);
void    single_cmd_exec(t_cmd *cmd);
void    pipex(t_ms *ms, t_list *cmds);
void    fork_children(int nb_cmds, t_pipe pipe_arr[MAX_PIPE], t_list *cmds);
void	child_first(t_cmd *child, int pipe[2]);
void	child_middle(t_cmd *child, int pipe1[2], int pipe2[2]);
void	child_last(t_cmd *child, int pipe[2]);
bool    cmd_exists(t_cmd *child);
bool	cmd_is_executable(t_cmd *child);
void	execute_child(t_cmd *child);
int		exec_builtin(t_cmd *child);
void	child_free_exit(int exit_code);

/* pre-execution */
void    handle_redirections(t_list *cmds);
void    handle_redir_in(t_cmd *cmd);
int     receive_heredoc(char *delimiter, char *filename);
char    *gen_unique_filename(unsigned long p);
void    handle_redir_out(t_cmd *cmd);

/* tokenization */
t_token	*tokenize(char *line);
t_token	*new_word(char *start, char **end);
t_token	*new_metacharacter(char *start, char **end);
t_token	*new_token(t_token_type type, char *value);
t_token	*concatenate(t_token *tokens, t_token *new);
void	update_in_quote(char c, int *in_quote);
t_token_type    get_type(char c);
bool    type_diff(char c1, char c2);
void	free_tokens(t_token **tokens);
void    print_tokens(t_token *tokens);

/* parse tokens */
bool    parse_tokens(t_token *tokens);
t_list  *gen_cmd_list(t_token *tokens);
t_list  *parse_cmd(t_token **tokens);
bool    parse_redir(char *file, t_token **redir_list, t_token **tokens);
bool    parse_cmd_and_arg(t_list **cmd_arg, t_token **tokens);
bool    reform_as_cmd_arr(t_list *cmd_arg, t_cmd *cmd);
void    free_cmd_arg_list(t_list **cmd_arg);
void    print_cmd_list(t_list *exec_list);
void    free_str_arr(char ***p_str_arr);
void    free_cmd_list(void);

/* pre-expand */
void    pre_expand(t_list **cmd_arg, t_cmd *cmd);
void    expand_env_var(t_list **cmd_arg, int head);
char    *match_env_var(char *name, int len);
void    expand_cmd_path(t_cmd *cmd, char *executable);
void    remove_quotes(t_list **str_node, char *old_str);
bool    has_expandable_dollar_str(t_list *arg, int *dollar_pos);
char    **get_paths_array(void);
char    *assemble_new_str(char *old_str, char *value, int head, int end);
char    *assemble_new_str2(char *old_str, int pair_of_quotes);

typedef struct s_shell
{
	char	*line;
	t_list	*env;

}			t_shell;

// INIT
bool		init_env(char **env, t_ms *shell);
void	    free_env(void);

// Builtins
int			ft_pwd(void);
int			ft_cd(char **args, t_list *env);
int			ft_echo(char **args);
int			print_env(t_list *env);
int			ft_export(char **args, t_list *env);
void		ft_exit(void);
int 		ft_unset(char **args, t_list *env);

// Signals
void		setup_signals(void);

// Syntax
int			check_syntax_errors(const char *input);
void		increment_quote_count(char c, int *single_quote_count,
				int *double_quote_count);
const char	*skip_whitespace(const char *input);
int			is_invalid_redirection(const char **input);

// utils
bool    is_builtin(char *cmd_name);
void		*ft_free_args(char **str);
void edit_env_value(t_list *env, char *name, char *new_value);
char	*trim_line(char *str);
int	has_unclosed_quotes(const char *input);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:36:17 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/09 17:19:06 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef MAX_PIPE
#  define MAX_PIPE 1024
# endif

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_HEREDOC,
	TOKEN_REDIR_HEREDOC_WEXP,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_cmd
{
	char			**cmd_arr;
	char			*fullpath;
	char			*filename;
	t_token			*redir;
	int				fd_in;
	int				fd_out;
	int				*i;
}					t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
}					t_env;

typedef struct s_pipe
{
	int				fd[2];
	pid_t			pid;
}					t_pipe;

typedef struct s_ms
{
	t_list			*env;
	t_list			*cmds;
	pid_t			pids[1024];
	t_pipe			pipe[MAX_PIPE];
	int				last_exit;
}					t_ms;

/* structure */
t_ms				*get_ms(void);
bool				init_env(char **envp, t_ms *shell);
void				free_env(void);
void				shell_routine(void);

/* utils */
t_env				*create_env_var(char *envp);
char				*get_prompt(void);
char				*get_pwd(void);
int					empty_line(char *line);
void				child_free_exit(int exit_code);

/* signals */
void				setup_signals(void);
void				sigint_handler(int sig);
void				child_sigint_handler(int sig);
void				update_heredoc_signal(int sig);
void				handle_heredoc_signal(int sig);

/* syntax check */
t_token				*check_syntax_and_tokenize(char *line);
int					check_syntax_errors(const char *input);
char				*trim_line(char *str);
int					has_unclosed_quotes(const char *input);
int					has_invalid_redirections(const char *input);
int					has_misplaced_operators(const char *input);
int					has_logical_operators(const char *input);
void				increment_quote_count(char c, int *single_quote_count,
						int *double_quote_count);
int					is_invalid_redirection(const char **input);
const char			*skip_whitespace(const char *input);

/* tokenization */
t_token				*tokenize(char *line);
t_token				*concatenate(t_token *tokens, t_token *new);
t_token				*new_metacharacter(char *start, char **end);
t_token				*new_word(char *start, char **end);
t_token				*new_token(t_token_type type, char *value);
bool				type_diff(char c1, char c2);
void				update_in_quote(char c, int *in_quote);
void				free_tokens(t_token **tokens);

/* token parsing */
void				parse_token_into_cmds(t_token *tokens);
t_list				*gen_cmd_list(t_token *tokens);
t_list				*parse_cmd(t_token **tokens);
void				parse_redir(char *file, t_token **redir_list,
						t_token **tokens);
void				parse_cmd_and_arg(t_list **cmd_arg, t_token **tokens);
void				reform_as_cmd_arr(t_list *cmd_arg, t_cmd *cmd);
void				free_cmd_arg_list(t_list **cmd_arg);
void				free_str_arr(char ***p_str_arr);

/* expansion */
void				pre_expand(t_token *tokens);
bool				has_expandable_dollar_str(t_token *token, int *dollar_pos);
void				expand_env_var(t_token *token, int head);
char				*match_env_var(char *name, int len);
char				*assemble_new_str(char *old_str, char *value, int head,
						int end);
char				*assemble_new_str2(char *old_str, int pair_of_quotes);
void				remove_quotes(t_token *token, char *old_str);

/* redirection */
void				handle_redirections(t_list *cmds);
bool				handle_redir_in(t_cmd *cmd, t_token *src);
void				gen_unique_filename(unsigned long p, t_cmd *cmd);
int					get_heredoc(char *delimiter, char *filename);
void				receive_heredoc(char *delimiter, int fd);
bool				handle_redir_out(t_cmd *cmd, t_token *dst);

/* path check */
void				expand_fullpath(t_list *cmds);
char				**get_paths_array(void);
void				assemble_fullpath(t_cmd *cmd, char *cmd_name, char **paths);
bool				valid_path(t_cmd *child, int *exit_code);
int					is_directory(char *path);

/* execution */
void				exec_manager(void);
void				execute_child(t_cmd *child);
char				**env_to_array(void);
char				**create_env_array(t_list *env);
void				gen_env_dict(char **env_array, t_list *env);
void				single_cmd_exec(t_cmd *cmd);
bool				is_builtin(char *cmd_name);
bool				any_cmd_failed(t_list *cmds);
int					exec_builtin(t_cmd *child);
int					exec_single_builtin(t_cmd *cmd);
// void				pipex(t_ms *ms, t_list *cmds, int nb_cmds);
// void				fork_children(int nb_cmds, t_pipe pipe_arr[MAX_PIPE],
// 						// t_list *cmds);
// void				child_first(t_cmd *child, int pipe[2]);
// void				child_middle(t_cmd *child, int pipe1[2], int pipe2[2]);
// void				child_last(t_cmd *child, int pipe[2]);
void				update_last_exit_status(int status);
void				pipex(t_ms *ms, t_list *cmds, int nb_cmds);
void				child_process(int i, int nb_cmds, t_cmd *cmd,
						int *output_cache);
void				handle_pipe(int *fd, t_ms *ms, int i);
void				handle_parent_process(int *fd, int *output_fd);
void				handle_child_input(t_cmd *cmd, int *fd, int *output_fd);
void				handle_child_output(int nb_cmds, t_cmd *cmd, int *fd,
						int *output_fd);

/* builtins */
int					ft_echo(char **args);
int					ft_cd(char **args, t_list *env);
int					cd_command(t_list *env, char *path);
void				edit_env_value(t_list *env, char *name, char *new_value);
int					ft_pwd(void);
int					ft_export(char **args, t_list *env);
bool				has_valid_name(const char *str);
void				add_or_edit_env_var(char *str, t_list **env_list);
int					add_env_var(char *str, t_list **env);
void				export_invalid_msg(char *arg);
int					ft_unset(char **args, t_list *env);
void				unset_env_var(char *arg, t_list **env);
void				free_and_relink(t_list *prev, t_list *current);
int					ft_env(t_list *envm, char **args);
int					ft_exit(char **args);
void				free_cmd_list(void);
int					is_input_valid(char **args);
int					is_number(char *str);

#endif

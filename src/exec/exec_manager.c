/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:23:50 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/09 16:08:24 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_manager(void)
{
	t_ms	*ms;
	t_list	*cmds;
	int		nb_cmds;

	ms = get_ms();
	cmds = ms->cmds;
	nb_cmds = ft_lstsize(cmds);
	if (nb_cmds == 0)
		return ;
	handle_redirections(cmds);
	if (any_cmd_failed(cmds))
		ms->last_exit = 1;
	expand_fullpath(cmds);
	if (nb_cmds == 1)
		single_cmd_exec(cmds->content);
	else
		pipex(ms, cmds, nb_cmds);
}

void	handle_redirections(t_list *cmds)
{
	t_cmd	*curr_cmd;
	t_token	*token;
	int		valid_redir;

	while (cmds)
	{
		curr_cmd = (t_cmd *)cmds->content;
		token = curr_cmd->redir;
		valid_redir = 1;
		while (token && valid_redir)
		{
			if (token->type == TOKEN_REDIR_HEREDOC
				|| token->type == TOKEN_REDIR_IN)
				valid_redir = handle_redir_in(curr_cmd, token);
			else
				valid_redir = handle_redir_out(curr_cmd, token);
			token = token->next;
		}
		if (curr_cmd->fd_out == 0)
			curr_cmd->fd_out = STDOUT_FILENO;
		cmds = cmds->next;
	}
}

void	single_cmd_exec(t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		get_ms()->last_exit = 1;
	if (cmd->fd_in == -1 || cmd->fd_out == -1 || !cmd->cmd_arr)
		return ;
	if (is_builtin(cmd->cmd_arr[0]))
	{
		get_ms()->last_exit = exec_builtin(cmd);
		return ;
	}
	pid = fork();
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, child_sigint_handler);
	if (pid == 0)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		dup2(cmd->fd_out, STDOUT_FILENO);
		execute_child(cmd);
	}
	waitpid(pid, &status, 0);
	update_last_exit_status(status);
	setup_signals();
}

void	update_last_exit_status(int status)
{
	if (WIFEXITED(status) && get_ms()->last_exit == 0)
		get_ms()->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status) && get_ms()->last_exit == 0)
		get_ms()->last_exit = WTERMSIG(status);
}

bool	is_builtin(char *cmd_name)
{
	if (!ft_strncmp(cmd_name, "echo", 5))
		return (1);
	if (!ft_strncmp(cmd_name, "cd", 3))
		return (1);
	if (!ft_strncmp(cmd_name, "pwd", 4))
		return (1);
	if (!ft_strncmp(cmd_name, "export", 7))
		return (1);
	if (!ft_strncmp(cmd_name, "unset", 6))
		return (1);
	if (!ft_strncmp(cmd_name, "env", 4))
		return (1);
	if (!ft_strncmp(cmd_name, "exit", 5))
		return (1);
	return (0);
}

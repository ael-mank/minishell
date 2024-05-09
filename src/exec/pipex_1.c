/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:34:58 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/09 13:01:50 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipex(t_ms *ms, t_list *cmds, int nb_cmds)
{
	int		i;
	int		output_cache;
	t_cmd	*curr_cmd;
	int		status;

	(void)ms;
	i = 1;
	output_cache = STDIN_FILENO;
	while (cmds)
	{
		curr_cmd = (t_cmd *)cmds->content;
		child_process(i, nb_cmds, curr_cmd, &output_cache);
		cmds = cmds->next;
		i++;
	}
	i = -1;
	while (++i < nb_cmds)
		waitpid(ms->pipe[i].pid, &status, 0);
	if (WIFEXITED(status))
		get_ms()->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		get_ms()->last_exit = WTERMSIG(status);
}

void	handle_pipe(int *fd, t_ms *ms, int i)
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	ms->pids[i] = fork();
	if (ms->pids[i] == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	handle_last_command(t_cmd *cmd, int *fd)
{
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(cmd->fd_out);
	}
	close(fd[1]); // Close write end in the last command
}

void	handle_child_process(int i, int nb_cmds, t_cmd *cmd, int *fd,
        int *output_fd)
{
    close(fd[0]); // Close unused read end
    if (cmd->fd_in != STDIN_FILENO)
    {
        if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(cmd->fd_in);
    }
    else if (i != 1)
    {
        if (dup2(*output_fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(*output_fd);
    }
    if (i < nb_cmds)
    {
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd[1]); // Close unused write end
    }
    else
    {
        handle_last_command(cmd, fd);
    }
    execute_child(cmd);  // Execute the command
    perror("minishell"); // Report errors
    exit(EXIT_FAILURE);  // Exit child process
}

void	handle_parent_process(int *fd, int *output_fd)
{
	close(fd[1]); // Close unused write end
	if (*output_fd != STDIN_FILENO)
		close(*output_fd); // Close previous output file descriptor
	*output_fd = fd[0];    // Update output file descriptor for next command
}

void	child_process(int i, int nb_cmds, t_cmd *cmd, int *output_fd)
{
    int		fd[2];
    t_ms	*ms;

    ms = get_ms();
    handle_pipe(fd, ms, i);
    if (ms->pids[i] == 0)
    {
        handle_child_process(i, nb_cmds, cmd, fd, output_fd);
    }
    else
    {
        handle_parent_process(fd, output_fd);
    }
}

void	execute_last_cmd(t_cmd *cmd)
{
	char **env;

	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		get_ms()->last_exit = 1;
	if (!cmd->cmd_arr[0] || cmd->cmd_arr[0][0] == '\0')
		get_ms()->last_exit = 0;
	if (is_builtin(cmd->cmd_arr[0]))
		get_ms()->last_exit = exec_single_builtin(cmd);
	if (ft_strchr("./", cmd->cmd_arr[0][0]) && !valid_path(cmd,
			&get_ms()->last_exit))
		return ;
	if (!cmd->fullpath)
	{
		ft_putstr_fd(cmd->cmd_arr[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		get_ms()->last_exit = 127;
	}
	env = env_to_array();
	execve(cmd->fullpath, cmd->cmd_arr, env);
	perror("minishell");
	close(cmd->fd_in);
	close(cmd->fd_out);
}
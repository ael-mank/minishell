/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:34:58 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/08 22:35:01 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pipex(t_ms *ms, t_list *cmds, int nb_cmds)
{
    int i;
    int output_cache;
    t_cmd   *curr_cmd;

    (void)ms;
    i = 1;
    output_cache = 0;
    while (i < nb_cmds)
    {
        curr_cmd = (t_cmd *)cmds->content;
        child_process(i, nb_cmds, curr_cmd, &output_cache);
        printf("cache fd num = %d\n", output_cache);
        cmds = cmds->next;
        i++;
    }
    if (i == nb_cmds)
    {
        curr_cmd = (t_cmd *)cmds->content;
        if (curr_cmd->fd_in == STDIN_FILENO)
            dup2(output_cache, STDIN_FILENO);
        else if (curr_cmd->fd_in != STDIN_FILENO)
            dup2(curr_cmd->fd_in, STDIN_FILENO);
        if (curr_cmd->fd_out != STDOUT_FILENO)
            dup2(curr_cmd->fd_out, STDOUT_FILENO);
        execute_last_cmd(curr_cmd);
    }
}

void    child_process(int i, int nb_cmds, t_cmd *cmd, int *output_cache)
{
    pid_t   pid;
    int     fd[2];

    if (pipe(fd) == -1)
        ft_putstr_fd("unable to pipe\n", 2);
    pid = fork();
    if (pid == -1)
        ft_putstr_fd("unable to fork\n", 2);
    if (pid == 0)
    {
        if (i == 1)
        {
            printf("cmd1: pipe_in=%d pipe_out=%d\n", fd[1], fd[0]);
            close(fd[0]);
            if (cmd->fd_in != STDIN_FILENO)
                dup2(cmd->fd_in, STDIN_FILENO);
            if (cmd->fd_out == STDOUT_FILENO)
                dup2(fd[1], STDOUT_FILENO);
            else if (cmd->fd_out != STDOUT_FILENO)
            {
                dup2(cmd->fd_out, STDOUT_FILENO);
                close(fd[1]);
            }
        }
        if (i < nb_cmds)
        {
            printf("cmd2: pipe_in=%d pipe_out=%d\n", fd[1], fd[0]);
            printf("cache fd num = %d\n", *output_cache);
            if (cmd->fd_in == STDIN_FILENO)
                dup2(*output_cache, STDIN_FILENO);
            else if (cmd->fd_in != STDIN_FILENO)
            {
                close(*output_cache);
                dup2(cmd->fd_in, STDIN_FILENO);
            }
            if (cmd->fd_out == STDOUT_FILENO)
                dup2(fd[1], STDOUT_FILENO);
            else if (cmd->fd_out != STDOUT_FILENO)
            {
                close(fd[1]);
                dup2(cmd->fd_out, STDOUT_FILENO);
            }
        }
        execute_child(cmd);
        perror("minishell");
	    child_free_exit(1);
    }
    else
    {
        close(fd[1]);
        close(fd[0]);
        *output_cache = fd[0];
        waitpid(pid, NULL, 0);
    }
}

void    execute_last_cmd(t_cmd *cmd)
{
	char	**env;

	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		get_ms()->last_exit = 1;
	if (!cmd->cmd_arr[0] || cmd->cmd_arr[0][0] == '\0')
		get_ms()->last_exit = 0;
	if (is_builtin(cmd->cmd_arr[0]))
		get_ms()->last_exit = exec_single_builtin(cmd);
	if (ft_strchr("./", cmd->cmd_arr[0][0]) && !valid_path(cmd, &get_ms()->last_exit))
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
}
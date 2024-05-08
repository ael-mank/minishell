/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:24:00 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/08 10:46:15 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

bool	handle_redir_in(t_cmd *cmd, t_token *src)
{
	if (src->type == TOKEN_REDIR_HEREDOC)
	{
		gen_unique_filename((unsigned long)cmd, cmd);
		cmd->fd_in = get_heredoc(src->value, cmd->filename);
		unlink(cmd->filename);
	}
	else if (src->type == TOKEN_REDIR_IN)
		cmd->fd_in = open(src->value, O_RDONLY, 0644);
	if (cmd->fd_in == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(src->value);
		return (0);
	}
	return (1);
}

int	get_heredoc(char *delimiter, char *filename)
{
	int		fd_tmp;
	pid_t	pid;

	fd_tmp = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	signal(SIGINT, &update_heredoc_signal);
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("unable to fork\n", 2);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, &handle_heredoc_signal);
		signal(SIGQUIT, SIG_IGN);
		receive_heredoc(delimiter, fd_tmp);
		child_free_exit(0);
	}
	waitpid(pid, NULL, 0);
	signal(SIGINT, &sigint_handler);
	close(fd_tmp);
	return (open(filename, O_RDONLY, 0644));
}

void	receive_heredoc(char *delimiter, int fd)
{
	char	*line;

	line = readline("heredoc> ");
	if (!line)
		return ;
	while (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) != 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("heredoc> ");
		if (!line)
			return ;
	}
	if (line)
		free(line);
}

void	gen_unique_filename(unsigned long p, t_cmd *cmd)
{
	char	address[17];
	char	*hex_char;
	int		i;

	hex_char = "0123456789abcdef";
	i = -1;
	while (++i < 16)
		address[i] = hex_char[(p >> (4 * (15 - i))) & 0xf];
	address[i] = '\0';
	cmd->filename = ft_strjoin(address, ".heredoc.tmp");
}

bool	handle_redir_out(t_cmd *cmd, t_token *dst)
{
	if (dst->type == TOKEN_REDIR_APPEND)
		cmd->fd_out = open(dst->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (dst->type == TOKEN_REDIR_OUT)
		cmd->fd_out = open(dst->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd->fd_out == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(dst->value);
		return (0);
	}
	return (1);
}

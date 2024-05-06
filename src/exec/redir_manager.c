/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrigny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:24:00 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/02 16:24:01 by yrigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	handle_redir_in(t_cmd *cmd, t_token *src)
{
	char	*filename;

	if (src->type == TOKEN_REDIR_HEREDOC)
	{
		filename = gen_unique_filename((unsigned long)cmd);
		cmd->fd_in = receive_heredoc(src->value, filename);
		unlink(filename);
		free(filename);
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

int	receive_heredoc(char *delimiter, char *filename)
{
	int		fd_tmp;
	char	*line;

	fd_tmp = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter))
			&& ft_strlen(line) == ft_strlen(delimiter) + 1)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd_tmp);
		free(line);
	}
	close(fd_tmp);
	return (open(filename, O_RDONLY, 0644));
}

char	*gen_unique_filename(unsigned long p)
{
	char	address[17];
	char	*hex_char;
	char	*filename;
	int		i;

	hex_char = "0123456789abcdef";
	i = -1;
	while (++i < 16)
		address[i] = hex_char[(p >> (4 * (15 - i))) & 0xf];
	address[i] = '\0';
	filename = ft_strjoin(address, ".heredoc.tmp");
	return (filename);
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

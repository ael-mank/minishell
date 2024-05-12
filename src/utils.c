/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 21:03:57 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/09 21:04:08 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_free_exit(int exit_code)
{
	int	fd;

	fd = get_ms()->fd_heredoc;
	close(fd);
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(exit_code);
}

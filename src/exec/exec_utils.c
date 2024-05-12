/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:08:25 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/09 16:08:32 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	any_cmd_failed(t_list *cmds)
{
	t_cmd	*curr_cmd;

	while (cmds)
	{
		curr_cmd = (t_cmd *)cmds->content;
		if (curr_cmd->fd_in == -1 || curr_cmd->fd_out == -1)
			return (1);
		cmds = cmds->next;
	}
	return (0);
}

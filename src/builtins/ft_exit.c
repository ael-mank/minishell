/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 09:37:20 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/02 10:34:11 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_input_valid(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i == 2 && !is_number(args[1]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (0);
	}
	return (1);
}

void	free_cmd_list(void)
{
	t_ms	*ms;
	t_list	**cmd_list;
	t_list	*next_cmd_node;

	ms = get_ms();
	cmd_list = &ms->cmds;
	while (*cmd_list)
	{
		next_cmd_node = (*cmd_list)->next;
		free_str_arr(&((t_cmd *)(*cmd_list)->content)->cmd_arr);
		free(((t_cmd *)(*cmd_list)->content)->fullpath);
		free(((t_cmd *)(*cmd_list)->content)->filename);
		free_tokens(&((t_cmd *)(*cmd_list)->content)->redir);
		free((*cmd_list)->content);
		free(*cmd_list);
		*cmd_list = next_cmd_node;
	}
}

int	ft_exit(char **args)
{
	int	status;
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i > 2)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	ft_printf("exit\n");
	if (i == 2 && is_input_valid(args))
		status = ft_atoi(args[1]);
	else
		status = 2;
	free_cmd_list();
	free_env();
	rl_clear_history();
	exit(status);
}

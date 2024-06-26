/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parser_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 20:23:28 by yrigny            #+#    #+#             */
/*   Updated: 2024/05/03 11:35:05 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reform_as_cmd_arr(t_list *cmd_arg, t_cmd *cmd)
{
	char	**ptr;
	int		wordcount;

	wordcount = ft_lstsize(cmd_arg);
	if (!wordcount)
		return ;
	cmd->cmd_arr = malloc(sizeof(char *) * (wordcount + 1));
	if (!(cmd->cmd_arr))
		return ;
	ptr = cmd->cmd_arr;
	while (cmd_arg)
	{
		*ptr = ft_strdup((char *)(cmd_arg->content));
		if (!(*ptr))
			free_str_arr(&cmd->cmd_arr);
		cmd_arg = cmd_arg->next;
		ptr++;
	}
	*ptr = NULL;
}

void	free_str_arr(char ***p_str_arr)
{
	char	**head;

	head = *p_str_arr;
	if (!head)
		return ;
	while (*head)
	{
		free(*head);
		head++;
	}
	free(*p_str_arr);
}

void	free_cmd_arg_list(t_list **cmd_arg)
{
	t_list	*next;

	while (*cmd_arg)
	{
		next = (*cmd_arg)->next;
		free((*cmd_arg)->content);
		free(*cmd_arg);
		*cmd_arg = next;
	}
}
/*
void	print_cmd_list(t_list *cmd_list)
{
	t_cmd	*cmd_node;
	t_token	*redir_files;
	char	**str_arr;

	while (cmd_list)
	{
		printf("----------------------\n");
		cmd_node = (t_cmd *)(cmd_list->content);
		redir_files = cmd_node->redir;
		while (redir_files && redir_files->next)
		{
			printf("redir: %s\n", redir_files->value);
			redir_files = redir_files->next;
		}
		str_arr = cmd_node->cmd_arr;
		if (*str_arr)
		{
			printf("cmd: %s\n", *str_arr);
			str_arr++;
		}
		while (*str_arr)
		{
			printf("arg: %s\n", *str_arr);
			str_arr++;
		}
		cmd_list = cmd_list->next;
		printf("----------------------\n");
	}
}*/

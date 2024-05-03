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
	t_token	*infiles;
	t_token	*outfiles;
	char	**str_arr;
	t_ms	*ms;
	t_list	**cmd_list;
	t_list	*next_cmd_node;
	t_ms	*ms;
	t_list	**cmd_list;
	t_list	*next_cmd_node;
	t_ms	*ms;
	t_list	**cmd_list;
	t_list	*next_cmd_node;

	while (cmd_list)
	{
		printf("----------------------\n");
		cmd_node = (t_cmd *)(cmd_list->content);
		// print infile(s)
		infiles = cmd_node->redir_in;
		while (infiles && infiles->next)
		{
			printf("silent infile: %s\n", infiles->value);
			infiles = infiles->next;
		}
		if (infiles)
			printf("active infile: %s\n", infiles->value);
		// print outfile(s)
		outfiles = cmd_node->redir_out;
		while (outfiles && outfiles->next)
		{
			printf("silent outfile: %s\n", outfiles->value);
			outfiles = outfiles->next;
		}
		if (outfiles)
			printf("active outfile: %s\n", outfiles->value);
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
		free_tokens(&((t_cmd *)(*cmd_list)->content)->redir);
		free((*cmd_list)->content);
		free(*cmd_list);
		*cmd_list = next_cmd_node;
	}
}

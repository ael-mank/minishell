/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/18 12:55:14 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_routine(void)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		// if (GNL == 0)
		line = readline("minishell> ");
		// // else
		// 	line = get_next_line(0);
		if (!line)
			break ;
		if (empty_line(line))
			continue ;
		add_history(line);
		tokens = check_syntax_and_tokenize(line);
		// if (!tokens)
		// 	g_signal = 258;
		if (!tokens)
			continue ;
		parse_tokens(tokens);
		free_cmd_list();
		// update_env_status(env, status, "=?");
	}
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_ms *ms;

	(void)argc;
	(void)argv;
	ms = get_ms();
	// g_signal = 0;
	if (init_env(envp, ms) == FAILURE)
		return (FAILURE);
	// ms->curr_dir = get_curr_dir();
	shell_routine();
	free_env(ms->env);
	return (SUCCESS);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/04/18 15:58:01 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_routine(void)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		char *(user) = match_env_var("USER", 4);
		char *(pwd) = getcwd(NULL, 0);
		char *(prompt) = ft_strjoin(user, "@");
		prompt = ft_strjoin(prompt, pwd);
		prompt = ft_strjoin(prompt, " $ ");
		line = readline(prompt);
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
		// if (!g_signal)
		// {
		// 	exec_list = parse_tokens(&tokens);
		// 	execution_manager(exec_list, env, &status);
		// 	free_exec_list(exec_list);
		// }
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
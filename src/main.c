/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/02 09:42:26 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_routine(void)
{
	char	*line;
	t_token	*tokens;
	char	*user;
	char	*pwd;
	char	*prompt;
	char	*tempPrompt;
	char	*finalPrompt;

	while (1)
	{
		user = match_env_var("USER", 4);
		pwd = getcwd(NULL, 0);
		prompt = ft_strjoin(user, "@");
		tempPrompt = ft_strjoin(prompt, pwd);
		free(prompt);
		free(pwd);
		finalPrompt = ft_strjoin(tempPrompt, "$ ");
		free(tempPrompt);
		line = readline(finalPrompt);
		free(finalPrompt);
		if (!line)
			break ;
		if (empty_line(line))
			continue ;
		add_history(line);
		tokens = check_syntax_and_tokenize(line);
		if (!tokens)
			continue ;
		parse_tokens(tokens);
		free_cmd_list();
	}
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	t_ms	*ms;

	(void)argc;
	(void)argv;
	ms = get_ms();
	// g_signal = 0;
	if (init_env(envp, ms) == 0)
		return (EXIT_FAILURE);
	setup_signals();
	shell_routine();
	free_env();
	return (EXIT_SUCCESS);
}
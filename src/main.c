/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/02 21:21:30 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(void)
{
	char	*pwd;
	char	*finalprompt;

	char **(go_back) = malloc(sizeof(char *) * 3);
	char *(user) = match_env_var("USER", 4);
	if (!go_back)
		return (NULL);
	go_back[0] = "cd";
	go_back[1] = ".";
	go_back[2] = NULL;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		chdir("..");
		ft_cd(go_back, get_ms()->env);
		pwd = getcwd(NULL, 0);
	}
	char *(prompt) = ft_strjoin(user, "@");
	char *(tempprompt) = ft_strjoin(prompt, pwd);
	free(prompt);
	free(pwd);
	finalprompt = ft_strjoin(tempprompt, "$ ");
	free(tempprompt);
	free(go_back);
	return (finalprompt);
}

void	shell_routine(void)
{
	char	*line;
	t_token	*tokens;
	char	*finalprompt;

	while (1)
	{
		finalprompt = create_prompt();
		line = readline(finalprompt);
		free(finalprompt);
		if (!line)
			break ;
		if (empty_line(line))
			continue ;
		add_history(line);
		tokens = check_syntax_and_tokenize(line);
		if (!tokens)
			continue ;
		parse_token_into_cmds(tokens);
		exec_manager();
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
	if (init_env(envp, ms) == 0)
		return (EXIT_FAILURE);
	setup_signals();
	shell_routine();
	free_env();
	return (EXIT_SUCCESS);
}

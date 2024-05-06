/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/06 10:11:59 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		while (!pwd)
		{
			chdir("..");
			pwd = getcwd(NULL, 0);
		}
	}
	return (pwd);
}

char	*create_prompt(char *user, char *pwd)
{
	char	*prompt;
	char	*tempprompt;
	char	*finalprompt;

	prompt = ft_strjoin(user, "@");
	tempprompt = ft_strjoin(prompt, pwd);
	free(prompt);
	free(pwd);
	finalprompt = ft_strjoin(tempprompt, "$ ");
	free(tempprompt);
	return (finalprompt);
}

char	*get_prompt(void)
{
	char	*pwd;
	char	*user;
	char	*finalprompt;

	user = match_env_var("USER", 4);
	pwd = get_pwd();
	finalprompt = create_prompt(user, pwd);
	return (finalprompt);
}

void print_ms_cmds(t_list *ms_cmds)
{
    t_cmd *cmd;
    int i;

    while (ms_cmds)
    {
        cmd = (t_cmd *)ms_cmds->content;
        printf("Command: %s\n", cmd->cmd_arr[0]);
        printf("Arguments:\n");
        i = 1;
        while (cmd->cmd_arr[i])
        {
            printf("  %s\n", cmd->cmd_arr[i]);
            i++;
        }
        ms_cmds = ms_cmds->next;
    }
}

void	shell_routine(void)
{
	char	*line;
	t_token	*tokens;
	char	*finalprompt;

	while (1)
	{
		finalprompt = get_prompt();
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

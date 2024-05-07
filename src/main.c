/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mank <ael-mank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 09:16:09 by ael-mank          #+#    #+#             */
/*   Updated: 2024/05/06 12:10:39 by ael-mank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	init_env(char **envp, t_ms *shell)
{
	int		i;
	t_env	*env_var;
	t_list	*env_var_node;

	i = -1;
	shell->env = NULL;
	if (!envp)
		return (0);
	while (envp[++i])
	{
		env_var = create_env_var(envp[i]);
		if (!env_var)
			return (0);
		env_var_node = ft_lstnew(env_var);
		if (!env_var_node)
			return (0);
		ft_lstadd_back(&shell->env, env_var_node);
	}
	return (1);
}

void	free_env(void)
{
	t_list	*env;
	t_list	*tmp;

	env = get_ms()->env;
	while (env)
	{
		tmp = env;
		env = env->next;
		free(((t_env *)tmp->content)->name);
		free(((t_env *)tmp->content)->value);
		free(tmp->content);
		free(tmp);
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

t_ms	*get_ms(void)
{
	static t_ms	minishell;

	return (&minishell);
}

int	main(int argc, char **argv, char **envp)
{
	t_ms	*ms;

	(void)argc;
	(void)argv;
	ms = get_ms();
	if (!envp)
		return (EXIT_FAILURE);
	if (init_env(envp, ms) == 0)
		return (EXIT_FAILURE);
	setup_signals();
	shell_routine();
	if (isatty(STDIN_FILENO))
		write(2, "exit\n", 6);
	free_env();
	return (EXIT_SUCCESS);
}

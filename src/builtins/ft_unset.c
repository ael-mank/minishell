#include "minishell.h"

void free_and_relink(t_list *prev, t_list *current)
{
	prev->next = current->next;
	t_env *env_var = (t_env *)current->content;
	free(env_var->name);
	free(env_var->value);
	free(current->content);
	free(current);
}

void ft_unset(char **args, t_list *env)
{
	t_list *prev = NULL;
	t_list *current = env;

	while (current)
	{
		t_env *env_var = (t_env *)current->content;
		if (strcmp(env_var->name, args[1]) == 0)
		{
			if (prev)
				free_and_relink(prev, current);
			else
				env = current->next;
			break;
		}
		prev = current;
		current = current->next;
	}
}
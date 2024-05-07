#include "minishell.h"

int	is_directory(char *path)
{
    struct stat	file_stat;

	if (stat(path, &file_stat) == 0)
    {
        if (S_ISDIR(file_stat.st_mode))
            return (1);
        else
            return (0);
    }
    return (0);
}

void	execute_child(t_cmd *child)
{
    int	exit_code;

    if (child->fd_in == -1 || child->fd_out == -1)
        child_free_exit(1);
    if (is_builtin(child->cmd_arr[0]))
    {
        exit_code = exec_single_builtin(child);
        child_free_exit(exit_code);
    }
    if (!child->cmd_arr[0] || child->cmd_arr[0][0] == '\0')
        child_free_exit(0);
    if (ft_strchr("./", child->cmd_arr[0][0])
        && !valid_path(child, &exit_code))
        child_free_exit(exit_code);
    if (!child->fullpath)
    {
        ft_putstr_fd(child->cmd_arr[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        child_free_exit(127);
    }
    execve(child->fullpath, child->cmd_arr, NULL);
    perror("minishell");
    child_free_exit(1);
}

bool    valid_path(t_cmd *child, int *exit_code)
{
    if (access(child->cmd_arr[0], F_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        perror(child->cmd_arr[0]);
        *exit_code = 127;
        return (0);
    }
    if (access(child->cmd_arr[0], X_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        perror(child->cmd_arr[0]);
        *exit_code = 126;
        return (0);
    }
    if (is_directory(child->cmd_arr[0]))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(child->cmd_arr[0], 2);
        ft_putstr_fd(": Is a directory\n", 2);
        *exit_code = 126;
        return (0);
    }
    child->fullpath = child->cmd_arr[0];
    return (1);
}

void	child_free_exit(int exit_code)
{
    free_cmd_list();
    free_env();
    rl_clear_history();
    exit(exit_code);
}

int	exec_builtin(t_cmd *child)
{
    int	exit_code;
    int	original;

    exit_code = 0;
    original = 0;
    if (child->fd_out != STDOUT_FILENO)
    {
        original = dup(STDOUT_FILENO);
        dup2(child->fd_out, STDOUT_FILENO);
    }
    exit_code = exec_single_builtin(child);
    if (child->fd_out != STDOUT_FILENO)
        dup2(original, STDOUT_FILENO);
    return (0 || exit_code);
}
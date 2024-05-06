#include "minishell.h"

int	ft_isdirectory(char *path)
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

bool	cmd_exists(t_cmd *child)
{
    if (!child->fullpath || child->fullpath[0] == '\0')
    {
        if (child->cmd_arr[0] && child->cmd_arr[0][0] != '\0')
        {
            ft_putstr_fd(child->cmd_arr[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            return (0);
        }
    }
    return (1);
}

bool	cmd_is_executable(t_cmd *child)
{
    if (child->fullpath && access(child->fullpath, X_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        perror(child->cmd_arr[0]);
        return (0);
    }
    return (1);
}

void	execute_child(t_cmd *child)
{
    int	builtin_exit_code;

    if (child->fd_in == -1 || child->fd_out == -1)
        child_free_exit(1);
    if (is_builtin(child->cmd_arr[0]))
    {
        builtin_exit_code = exec_single_builtin(child);
        child_free_exit(builtin_exit_code);
    }
    if (!child->cmd_arr[0] || child->cmd_arr[0][0] == '\0')
        child_free_exit(0);
    if (!cmd_exists(child))
        child_free_exit(127);
    if (!cmd_is_executable(child))
        child_free_exit(126);
    if (child->fullpath && ft_isdirectory(child->fullpath) == 1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(child->cmd_arr[0], 2);
        ft_putstr_fd(": Is a directory\n", 2);
        child_free_exit(126);
    }
    if (child->fullpath)
        execve(child->fullpath, child->cmd_arr, NULL);
    perror("minishell");
    child_free_exit(1);
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
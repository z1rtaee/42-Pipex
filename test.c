#include "pipex.h"

void ft_child_process(t_pipex *pipex, char **argv, char **envp, int i)
{
    if (i == 0) {
        dup2(pipex->input_fd, STDIN_FILENO);
        dup2(pipex->pipes[i][1], STDOUT_FILENO);
    } else if (i == pipex->cmd_count - 1) {
        dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
        dup2(pipex->output_fd, STDOUT_FILENO);
    } else {
        dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
        dup2(pipex->pipes[i][1], STDOUT_FILENO);
    }
    // Close pipes in child
    int j = 0;
    while (j < pipex->cmd_count - 1) {
        close(pipex->pipes[j][0]);
        close(pipex->pipes[j][1]);
        j++;
    }
    ft_extract_cmd_args(pipex, argv[i + 2 + pipex->here_doc]);
    ft_exec_cmd(pipex, envp);
}

void ft_close_pipes(t_pipex *pipex)
{
    int i = 0;
    while (i < pipex->cmd_count - 1) {
        close(pipex->pipes[i][0]);
        close(pipex->pipes[i][1]);
        i++;
    }
}

void ft_wait_processes(t_pipex *pipex)
{
    int i = 0;
    while (i < pipex->cmd_count) {
        waitpid(pipex->pids[i], NULL, 0);
        i++;
    }
}

void free_resources(t_pipex *pipex)
{
    int i = 0;
    while (i < pipex->cmd_count - 1) {
        free(pipex->pipes[i]);
        i++;
    }
    free(pipex->pipes);
    free(pipex->pids);
}

void ft_exec_cmd(t_pipex *pipex, char **envp)
{
    char *tmp_path = ft_find_cmd_path(pipex);
    if (!tmp_path) {
        ft_putendl_fd("Command not found", 2);
        exit(127);
    }
    if (execve(tmp_path, pipex->cmd_args, envp) == -1) {
        ft_putendl_fd("Execution Failed", 2);
        free(tmp_path);
        exit(1);
    }
}

void ft_fork_processes(t_pipex *pipex, char **argv, char **envp)
{
    int i;

    i = 0;
    while (i < pipex->cmd_count)
    {
        pipex->pids[i] = fork();
        if (pipex->pids[i] == -1)
        {
            ft_putendl_fd("Fork failed", 2);
            exit(1);
        }
        if (pipex->pids[i] == 0)
        {
            ft_child_process(pipex, argv, envp, i);
            exit(0); 
        }
        i++;
    }
}

void ft_close_pipes(t_pipex *pipex)
{
    int i = 0;
    while (i < pipex->cmd_count - 1)
    {
        close(pipex->pipes[i][0]); 
        close(pipex->pipes[i][1]); 
        i++;
    }
}

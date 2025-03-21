/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_02.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:44:48 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/21 10:45:37 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	ft_extract_cmd_args(t_pipex *pipex, char *cmd)
{
	pipex->cmd_args = ft_split(cmd, ' ');
	if (!pipex->cmd_args || !pipex->cmd_args[0])
	{
		ft_putendl_fd("Error: Command parsing failed!", 2);
		exit(1);
	}
}

char	*ft_find_cmd_path(t_pipex *pipex)
{
	char	*tmp_path;
    char    *final_path;
	int		i;

	i = 0;
	while (pipex->cmd_paths[i])
	{
		tmp_path = ft_strjoin(pipex->cmd_paths[i], "/");
		final_path = ft_strjoin(tmp_path, pipex->cmd_args[0]);
		if (access(final_path, X_OK) == 0)
			return (free(tmp_path), final_path);
		free(tmp_path);
        free(final_path);
		i++;
	}
	return (NULL);
}

void	ft_exec_cmd(t_pipex *pipex, char **envp)
{
	char *tmp_path;

	tmp_path = ft_find_cmd_path(pipex);
	if (!tmp_path)
	{
        free_ar((void **)pipex->cmd_paths);
        free_ar((void **)pipex->cmd_args);
        free_ar((void **)pipex->pipes);
        free(pipex->pids);
		ft_putendl_fd("Command not found", 2);
		exit(127);
	}
	if (execve(tmp_path, pipex->cmd_args, envp) == -1)
	{
		ft_putendl_fd("Execution Failed", 2);
		free(tmp_path);
		exit(1);
	}
    free(tmp_path);
}

void    ft_child_process(t_pipex *pipex, char **argv, char **envp, int i)
{
    if (i == 0)
    {
        dup2(pipex->input_fd, STDIN_FILENO);
        dup2(pipex->pipes[i][1], STDOUT_FILENO);
    }
    else if (i == pipex->cmd_count - 1)
    {
        dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
        dup2(pipex->output_fd, STDOUT_FILENO);
    }
    else
    {
        dup2(pipex->pipes[i - 1][0], STDIN_FILENO);
		dup2(pipex->pipes[i][1], STDOUT_FILENO);
        ft_close_all(pipex);
    }
    ft_close_all(pipex);
	ft_extract_cmd_args(pipex, argv[i + 2 + pipex->here_doc]);
	ft_exec_cmd(pipex, envp);
}

void    ft_create_pipes(t_pipex *pipex)
{
    int i = 0;

    while (i < pipex->cmd_count - 1)
    {
        if (pipe(pipex->pipes[i]) == -1)
        {
            ft_putendl_fd("Error creating pipe", 2);
            exit(1);
        }
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


void ft_close_all(t_pipex *pipex)
{
    int i = 0;
    while (i < pipex->cmd_count - 1)
    {
        close(pipex->pipes[i][0]); 
        close(pipex->pipes[i][1]); 
        i++;
    }
    close(pipex->input_fd);
    close(pipex->output_fd);
}
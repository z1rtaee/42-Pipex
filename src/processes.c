/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:53:54 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/29 06:50:24 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	switch_in_out_pipe(t_pipex *pipex)
{
	if (pipex->in_pipe[0] > -1 && pipex->in_pipe[1] > -1)
	{
		close(pipex->in_pipe[0]);
		close(pipex->in_pipe[1]);
	}
	pipex->in_pipe[0] = pipex->out_pipe[0];
	pipex->out_pipe[0] = -1;
	pipex->in_pipe[1] = pipex->out_pipe[1];
	pipex->out_pipe[1] = -1;
}

void	ft_fork_processes(t_pipex *pipex, char **argv, char **envp)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
	{
		if (pipe(pipex->out_pipe) == -1)
			exit_error("Error: Pipe failed!");
		pipex->pids[i] = fork();
		if (pipex->pids[i] == -1)
			exit_error("Error: Fork failed!");
		if (pipex->pids[i] == 0)
		{
			ft_child_process(pipex, argv, envp, i);
			exit(1);
		}
		switch_in_out_pipe(pipex);
		i++;
	}
}

void	ft_child_process(t_pipex *pipex, char **argv, char **envp, int i)
{
	if (i == 0)
	{
		dup2(pipex->input_fd, STDIN_FILENO);
		dup2(pipex->out_pipe[1], STDOUT_FILENO);
	}
	else if (i == pipex->cmd_count - 1)
	{
		dup2(pipex->in_pipe[0], STDIN_FILENO);
		dup2(pipex->output_fd, STDOUT_FILENO);
	}
	else
	{
		dup2(pipex->in_pipe[0], STDIN_FILENO);
		dup2(pipex->out_pipe[1], STDOUT_FILENO);
		ft_close_all(pipex);
	}
	ft_close_all(pipex);
	ft_extract_cmd_args(pipex, argv[i + 2 + pipex->here_doc]);
	ft_exec_cmd(pipex, envp);
}

void	ft_exec_cmd(t_pipex *pipex, char **envp)
{
	char	*tmp_path;

	tmp_path = ft_find_cmd_path(pipex);
	if (!tmp_path)
	{
		free_resources(pipex);
		ft_putendl_fd("Error: Command not found", 2);
		exit(127);
	}
	if (execve(tmp_path, pipex->cmd_args, envp) == -1)
	{
		free(tmp_path);
		exit_error("Error: Execution failed");
	}
	free(tmp_path);
}

void	ft_wait_processes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->pids[i], NULL, 0);
		i++;
	}
}

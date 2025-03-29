/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:23:07 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/28 19:50:52 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	ft_init_pipex(t_pipex *pipex)
{
	pipex->input_fd = -1;
	pipex->output_fd = -1;
	pipex->here_doc = false;
	pipex->cmd_paths = NULL;
	pipex->cmd_args = NULL;
	pipex->cmd_count = 0;
	pipex->in_pipe[0] = -1;
	pipex->in_pipe[1] = -1;
	pipex->out_pipe[0] = -1;
	pipex->out_pipe[1] = -1;
	pipex->pids = NULL;
}

void	ft_processes(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->cmd_count = argc - 3 - pipex->here_doc;
	pipex->pids = malloc(sizeof(pid_t) * (pipex->cmd_count));
	if (!pipex->pids)
		exit_error("Error: Memory allocation failed");
	ft_fork_processes(pipex, argv, envp);
	ft_close_all(pipex);
	ft_wait_processes(pipex);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	ft_init_pipex(&pipex);
	ft_check_args(argc, argv, &pipex);
	ft_path_dir(&pipex, envp);
	ft_processes(&pipex, argc, argv, envp);
	free_resources(&pipex);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_close_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:40:19 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/28 19:44:51 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	free_resources(t_pipex *pipex)
{
	free_ar((void **)pipex->cmd_paths);
	free_ar((void **)pipex->cmd_args);
	free(pipex->pids);
}

void	ft_close_all(t_pipex *pipex)
{
	if (pipex->in_pipe[0] > -1 && pipex->in_pipe[1] > -1)
	{
		close(pipex->in_pipe[0]);
		close(pipex->in_pipe[1]);
	}
	if (pipex->out_pipe[0] > -1 && pipex->out_pipe[1] > -1)
	{
		close(pipex->out_pipe[0]);
		close(pipex->out_pipe[1]);
	}
	if (pipex->input_fd >= 0)
		close(pipex->input_fd);
	if (pipex->output_fd >= 0)
		close(pipex->output_fd);
}

void	exit_error(char *error_message)
{
	ft_putendl_fd(error_message, 2);
	exit(1);
}

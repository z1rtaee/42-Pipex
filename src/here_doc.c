/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:37:51 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/29 03:02:23 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	ft_handle_hd(char *delimiter, t_pipex *pipex)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		exit_error("Error: Error creating pipe.");
	while (1)
	{
		ft_putstr_fd("here_doc> ", 1);
		line = get_next_line(0);
		if (!line || (strncmp(line, delimiter, ft_strlen(line) - 1) == 0
				&& ft_strlen(line) - 1 == ft_strlen(delimiter)))
		{
			if (line)
				free(line);
			break ;
		}
		write (pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fd[1]);
	pipex->input_fd = pipe_fd[0];
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:59:31 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/20 09:01:37 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void    ft_init_pipex(t_pipex *pipex)
{
	pipex->input_fd = -1;
	pipex->output_fd = -1;
	pipex->here_doc = false;
	pipex->cmd_paths = NULL;
	pipex->cmd_args = NULL;
	pipex->cmd_count = 0;
	pipex->pids = NULL;
	pipex->pipes = NULL;
}

void    ft_check_args(int argc, char **argv, t_pipex *pipex)
{
	int	min_args;
	if (argc < 2)
	{
		ft_putendl_fd("Invalid number of arguments", 2);
		exit(1);
	}
		if (ft_strcmp(argv[1], "here_doc") == 0){
		min_args = 6;
		pipex->here_doc = true;
	}
	else
		min_args = 5;
	if (argc < min_args)
	{
		ft_putendl_fd("Invalid number of arguments", 2);
		exit(1);
	}
	if (pipex->here_doc)
		ft_handle_hd(argv[2], pipex);
	else
	{
		pipex->input_fd = open(argv[1], O_RDONLY);
		if (pipex->input_fd == -1)
		{
			ft_putendl_fd("Error opening input file", 2);
			exit(1);
		}
	}
	if (pipex->here_doc)
		pipex->output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		pipex->output_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->output_fd == -1)
	{
		ft_putendl_fd("Error opening output file", 2);
		exit(1);
	}
	//close(pipex->output_fd);
}

void	ft_handle_hd(char *delimiter, t_pipex *pipex)
{
	int		pipe_fd[2];
	char	*line;
	char	*aux;

	if (pipe(pipe_fd) == -1)
	{
		ft_putendl_fd("Error creating pipe", 2);
		exit(1);
	}
	while (1)
	{
		ft_putstr_fd("here_doc> ", 1);
		aux = get_next_line(0);
		line = ft_strtrim(aux, "\n");
		free(aux);
		if (!line || strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break;
		}
			write (pipe_fd[1], line, ft_strlen(line));
		free (line);
	}
	close(pipe_fd[1]);
	pipex->input_fd = pipe_fd[0];
}

char	*ft_find_path(char **envp)
{
	int 	i;
	char	*res;

	i = 0;
	res = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			res = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!res)
		return (ft_putendl_fd("Path not found", 2), NULL);
	return (res);
}

void	ft_path_dir(t_pipex *pipex, char **envp)
{
	char	*path;

	path = ft_find_path(envp);
	if (!path)
		exit(1);
	pipex->cmd_paths = ft_split(path, ':');
}


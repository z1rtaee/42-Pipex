/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:25:09 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/29 04:03:02 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	ft_check_args(int argc, char **argv, t_pipex *pipex)
{
	int	min_args;

	if (argc > 2 && ft_strcmp(argv[1], "here_doc") == 0)
	{
		min_args = 6;
		pipex->here_doc = true;
	}
	else
		min_args = 5;
	if (argc < min_args)
		exit_error("Invalid number of arguments");
	set_up_input(argc, argv, pipex);
	set_up_output(argc, argv, pipex);
}

//setsup the input file
void	set_up_input(int argc, char **argv, t_pipex *pipex)
{
	if (pipex->here_doc)
		ft_handle_hd(argv[2], pipex);
	else
	{
		pipex->input_fd = open(argv[1], O_RDONLY);
		if (pipex->input_fd == -1)
		{
			ft_putendl_fd("Error: Error opening input file.", 2);
			pipex->input_fd = open("/dev/null", O_RDONLY);
			if (pipex->input_fd == -1)
				exit_error("Critical error: Unable to open /dev/null.");
			set_up_output(argc, argv, pipex);
			close(pipex->output_fd);
		}
	}
}

//0644 ensures that the file is created so that 
//he owner can read and write, while everyone else can only read the file.
void	set_up_output(int argc, char **argv, t_pipex *pipex)
{
	if (pipex->here_doc)
		pipex->output_fd = open(argv[argc - 1], O_WRONLY
				| O_CREAT | O_APPEND, 0644);
	else
		pipex->output_fd = open(argv[argc -1], O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
	if (pipex->output_fd == -1)
	{
		close(pipex->input_fd);
		exit_error("Error: Error opening output file.");
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:13:28 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/28 23:32:42 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../complete_lib/42_Libft/libft.h"
# include "../complete_lib/42_GNL/get_next_line_bonus.h"
# include "../complete_lib/42_Printf/ft_printf.h"
# include <stdbool.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	int		input_fd;
	int		output_fd;
	bool	here_doc;
	char	**cmd_paths;
	char	**cmd_args;
	int		cmd_count;
	int		in_pipe[2];
	int		out_pipe[2];
	pid_t	*pids;
}				t_pipex;

//this function initializes all the data inside my 
//struct preventing any undefined behaviour
void	ft_init_pipex(t_pipex *pipex);

//Memory and Fd management + error handles
void	exit_error(char *error_message);
void	ft_close_all(t_pipex *pipex);
void	free_resources(t_pipex *pipex);

//this function checks for the minimun args and sets the
//input and output files ready to read or write/append or 
//trunc also creating them
void	ft_check_args(int argc, char **argv, t_pipex *pipex);
void	set_up_input(int argc, char **argv, t_pipex *pipex);
void	set_up_output(int argc, char **argv, t_pipex *pipex);

//here_doc handle
void	ft_handle_hd(char *delimiter, t_pipex *pipex);

				//Utils for the command making//
//Searchs on the envp array (enviroment variables) for the
//PATH variable and then returns it
char	*ft_find_path(char **envp);
//Splits the directories with the ':' seperator
//and then returns it to the struct joining it with the path
void	ft_path_dir(t_pipex *pipex, char **envp);
//Splits each string received to get only the commands using
//the sep ' ' and puts them into the char double pointer 
//(matrix) pipex->cmd_args
void	ft_extract_cmd_args(t_pipex *pipex, char *cmd);
//Joins the path with the cmds and a '/' in the middle
//getting the absolute path
char	*ft_find_cmd_path(t_pipex *pipex);

//Unix processes
void	switch_in_out_pipe(t_pipex *pipex);
void	ft_fork_processes(t_pipex *pipex, char **argv, char **envp);
void	ft_child_process(t_pipex *pipex, char **argv, char **envp, int i);
void	ft_exec_cmd(t_pipex *pipex, char **envp);
void	ft_wait_processes(t_pipex *pipex);

//main
void	ft_processes(t_pipex *pipex, int argc, char **argv, char **envp);
int		main(int argc, char **argv, char **envp);

#endif

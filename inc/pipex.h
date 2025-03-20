/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:42:31 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/20 08:54:11 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../complete_lib/42_Libft/libft.h"
# include "../complete_lib/42_GNL/get_next_line_bonus.h"
# include "../complete_lib/42_Printf/ft_printf.h"
# include <stdbool.h>
# include <sys/wait.h>

typedef struct  s_pipex
{
	int     input_fd;
	int     output_fd;
	bool    here_doc;
	char    **cmd_paths;
	char    **cmd_args;
	int			cmd_count;
	int     **pipes;
	pid_t		*pids;
}               t_pipex;

//This function is responsible for intializing/setting up the 
//t_pipex struct to prevent undefined behaviour and prepare for later steps.
void    ft_init_pipex(t_pipex *pipex);

void    ft_check_args(int argc, char **argv, t_pipex *pipex);

void	ft_handle_hd(char *delimiter, t_pipex *pipex);

char	*ft_find_path(char **envp);

void	ft_path_dir(t_pipex *pipex, char **envp);

void	ft_extract_cmd_args(t_pipex *pipex, char *cmd);

char	*ft_find_cmd_path(t_pipex *pipex);

void	ft_exec_cmd(t_pipex *pipex, char **envp);

void    ft_pipex(t_pipex *pipex, int argc, char **argv, char **envp);

void free_resources(t_pipex *pipex);

void ft_wait_processes(t_pipex *pipex);

void    ft_child_process(t_pipex *pipex, char **argv, char **envp, int i);

void    ft_create_pipes(t_pipex *pipex);

void ft_close_all(t_pipex *pipex);

void ft_fork_processes(t_pipex *pipex, char **argv, char **envp);

#endif
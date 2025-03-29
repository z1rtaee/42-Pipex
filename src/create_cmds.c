/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:41:13 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/29 05:31:57 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

char	*ft_find_path(char **envp)
{
	int		i;
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
		res = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
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

void	ft_extract_cmd_args(t_pipex *pipex, char *cmd)
{
	pipex->cmd_args = ft_split(cmd, ' ');
	if (!pipex->cmd_args || !pipex->cmd_args[0])
	{
		free_resources(pipex);
		exit_error("Error: Command parsing failed!");
	}
}

char	*ft_find_cmd_path(t_pipex *pipex)
{
	char	*tmp_path;
	char	*final_path;
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

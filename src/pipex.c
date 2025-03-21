/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpires-r <bpires-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 00:59:28 by bpires-r          #+#    #+#             */
/*   Updated: 2025/03/20 09:40:58 by bpires-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../inc/pipex.h"


void    ft_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
    int i;
    
    i = 0;
    pipex->cmd_count = argc - 3 - pipex->here_doc;
    pipex->pipes = malloc(sizeof(int *) * (pipex->cmd_count));
    if (!pipex->pipes)
    {
        ft_putendl_fd("Memory allocation failed", 2);
        exit(1);
    }
    while (i < pipex->cmd_count - 1)
    {
        pipex->pipes[i] = malloc(sizeof(int) * 2);
        if (!pipex->pipes[i])
        {
            ft_putendl_fd("Memory allocation failed", 2);
            exit(1);
        }
        i++;
    }
    pipex->pipes[i] = NULL;
    pipex->pids = malloc(sizeof(pid_t) * (pipex->cmd_count));
    if (!pipex->pids){
        ft_putendl_fd("Memory allocation failed", 2);
        exit(1);
    }
    ft_create_pipes(pipex);
    ft_fork_processes(pipex, argv, envp);
    ft_close_all(pipex);
    ft_wait_processes(pipex);
    i = -1;
    while (++i < pipex->cmd_count - 1)
        free(pipex->pipes[i]);
    free(pipex->pipes);
    free(pipex->pids);
    free_ar((void **)pipex->cmd_paths);
}

int main(int argc, char **argv, char **envp)
{
    t_pipex pipex;

    ft_init_pipex(&pipex);
    ft_check_args(argc, argv, &pipex);
    ft_path_dir(&pipex, envp);
    ft_pipex(&pipex, argc, argv, envp);
    return (0);
}

/* int main(int argc, char **argv, char **envp){
//    for (int i = 0; envp[i]; i++)
//      printf ("%s\n", envp[i]);
//printf ("odeio flags feias %s\n", argv[1]);
    t_pipex pipex;
    
    ft_init_pipex(&pipex);
    ft_check_args(argc, argv, &pipex);
    
    if (pipex.here_doc)
    {
        printf ("Here Doc Mode: Input from pipe instead of a file\n");
        printf ("pipex.input_fd = %d \n", pipex.input_fd);
    }
    else{
        printf("Regular Mode: Input from file: %s\n", argv[1]);
        printf("Regular pipex.input_fd = %d \n", pipex.input_fd);
    }
    printf ("\n");
    ft_path_dir(&pipex, envp);
    printf("\n");
    ft_extract_cmd_args(&pipex, argv[2]);
    printf("\n");
    ft_exec_cmd(&pipex, envp);
    return (0);
}
*/
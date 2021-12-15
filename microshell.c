/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbensarg <sbensarg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 16:42:36 by sbensarg          #+#    #+#             */
/*   Updated: 2021/12/15 16:20:20 by sbensarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"
#include <unistd.h>

int	fd_in;
int	p[2];
    
void ft_cd(char *cmd)
{
    int ret;

    ret = chdir(cmd);
    if (ret == -1)
    {
        write(2, "error: cd: cannot change directory to ", 39);
        write(2, cmd, strlen(cmd));
        write(2, "\n", 2);
    }
}
int if_b_in(char **cmd)
{
    int j;

    j = 0;
    if (!strcmp(cmd[j], "cd"))
    {
        if (cmd[j + 2] || cmd[j + 1] == NULL)
            write(2, "error: cd: bad arguments\n", 26);
        else if (cmd[j + 1])
            ft_cd(cmd[j + 1]);
        return (1);
    }
    return (0);
}
int main(int argc, char **argv, char **env)
{
    char *cmd[1024];
    int i;
    i = 1;
    int j = 0;
    int k = 0;
    pid_t pid;
    int flag;
    int ret;
	fd_in = 0;
    while (k < 1024)
    {
        cmd[k] = NULL;
        k++;
    }
    while (argv[i-1])
    {
        if ((argv[i] != NULL && !strcmp(argv[i], "|")) || argv[i] == NULL)
        {
			j = 0;
            if (!strcmp(cmd[j], "cd"))
            {
                if (cmd[j + 2] || cmd[j + 1] == NULL)
                    write(2, "error: cd: bad arguments\n", 26);
                else if (cmd[j + 1])
                    ft_cd(cmd[j + 1]);
            }
            else
            {
                pipe(p);
                pid = fork();
                if (pid == 0)
                {
                    dup2(fd_in, 0);
					if (argv[i] != NULL)
					{
                    	dup2(p[1], 1);
					}
                    close(p[0]);
                    execve(cmd[0], cmd, NULL);
                }
            }
            int m = 0;
            while (m < 1024)
            {
                cmd[m] = NULL;
                m++;
            }
        }
        else if (argv[i] == NULL || !strcmp(argv[i], ";"))
        {
            j = 0;
            if (!strcmp(cmd[j], "cd"))
            {
                if (cmd[j + 2] || cmd[j + 1] == NULL)
                    write(2, "error: cd: bad arguments\n", 26);
                else if (cmd[j + 1])
                    ft_cd(cmd[j + 1]);
            }
            else
            {
                pid = fork();
                if (pid == 0)
                {
                    ret = execve(cmd[j], cmd, env);
                     cmd[j] = NULL;
                    if (ret == -1)
                    {
                            write(2, "error: cannot execute ", 22);
                            write(2, cmd[j], strlen(cmd[j]));
                            write(2, "\n", 2);
                    }
                }
            }
            int m = 0;
            while (m < 1024)
            {
                cmd[m] = NULL;
                m++;
            }
        }
        else
        {
            cmd[j] = argv[i];
            close(p[1]);
            fd_in = p[0];
            j++;
        }
        i++;
    }
	int m = 0;
	while(m < j)
	{
		waitpid(pid, NULL, 0);
		m++;
	}
}

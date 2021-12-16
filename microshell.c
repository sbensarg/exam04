/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbensarg <sbensarg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 16:42:36 by sbensarg          #+#    #+#             */
/*   Updated: 2021/12/16 02:07:41 by sbensarg         ###   ########.fr       */
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
	flag = 0;
    while (k < 1024)
    {
        cmd[k] = NULL;
        k++;
    }
    while (argv[i-1])
    {
        if ((argv[i] != NULL && !strcmp(argv[i], "|")))
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
				flag = 1;
                if (pipe(p) == -1)
				{
					write(2, "error: fatal\n", 14);
					exit(EXIT_FAILURE);
				}
                pid = fork();
				if (pid == -1)
				{
					write(2, "error: fatal\n", 14);
					exit(EXIT_FAILURE);
				}
                if (pid == 0)
                {
                    dup2(fd_in, 0);
					if (argv[i] != NULL)
					{
                    	dup2(p[1], 1);
					}
                    close(p[0]);
                    ret = execve(cmd[0], cmd, NULL);
					if (ret == -1)
					{
						write(2, "error: cannot execute ", 22);
						write(2, cmd[j], strlen(cmd[j]));
						write(2, "\n", 2);
						exit(EXIT_FAILURE);
					}
                }
				else
					waitpid(pid, NULL, 0);
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
			if (cmd[0] != NULL)
			{
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
						if (flag == 1)
							dup2(fd_in, 0);
						ret = execve(cmd[j], cmd, env);
						if (ret == -1)
						{
							write(2, "error: cannot execute ", 22);
							write(2, cmd[j], strlen(cmd[j]));
							write(2, "\n", 2);
							exit(EXIT_FAILURE);
						}
					}
					else
					{
						waitpid(pid, NULL, 0);
						flag = 0;
					}
				}
				int m = 0;
				while (m < 1024)
				{
					cmd[m] = NULL;
					m++;
				}
			}
        }
        else
        {
            cmd[j] = argv[i];
			if (flag == 1)
			{
            	fd_in = p[0];
				close(p[1]);
			}
			j++;
        }
        i++;
    }
}

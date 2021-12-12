/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbensarg <sbensarg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 16:42:36 by sbensarg          #+#    #+#             */
/*   Updated: 2021/12/12 18:49:35 by sbensarg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

int main(int argc, char **argv, char **env)
{
    char *cmd[1024];
    int i;
    i = 1;
    int j = 0;

    while (argv[i])
    {
        if (!strncmp(argv[i], "|", 2))
        {
            cmd[j] = argv[i - 1];
            j++;
        }
        if (!strncmp(argv[i], ";", 2))
        {
            cmd[j] = argv[i - 1];
            j++;
        }
        if (argv[i + 1] == NULL && (strcmp(argv[i], "|") || strcmp(argv[i], ";")))
            cmd[j] = argv[i];
        i++;
    }
    i = 0;
    while (cmd[i])
    {
        printf("cmd %s \n", cmd[i]);
        i++;
    }
}

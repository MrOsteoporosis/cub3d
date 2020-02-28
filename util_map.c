/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averheij <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 13:23:26 by averheij          #+#    #+#             */
/*   Updated: 2020/02/28 14:33:27 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <libft.h>
#include <mlx.h>
#include "cub3d.h"

int     free_strings_and_close(t_vars *vars, int fd, char *line)
{
	if (fd != -1)
    	close(fd);
    if (line)
        free(line);
    if(vars->no.path)
        free(vars->no.path);
    if (vars->so.path)
        free(vars->so.path);
    if (vars->we.path)
        free(vars->we.path);
    if (vars->ea.path)
        free(vars->ea.path);
    return (1);
}

int     free_everything(t_vars *vars, int fd, char *line)
{
    int     i;

    if (vars->world.map)
    {
        i = 0;
        while (i < vars->world.map_height)
        {
            if (vars->world.map[i])
                free(vars->world.map[i]);
            i++;
        }
        free(vars->world.map);
    }
    free_strings_and_close(vars, fd, line);
    return (1);
}

int		ft_iswhitespace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int     ft_skip_comma(char **str)
{
    if (!(**str == ','))
        return (1);
    (*str)++;
    if (**str)
        return (0);
    return (1);
}

int     ft_skip_passed_func(char **str, t_isfunc is)
{
    while (**str && is(**str))
        (*str)++;
    if (**str)
        return (0);
    return (1);
}



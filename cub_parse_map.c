/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_map.c                                    :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 11:57:39 by averheij       #+#    #+#                */
/*   Updated: 2020/03/09 12:40:49 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <get_next_line.h>
#include <libft.h>
#include <mlx.h>
#include "cub3d.h"

int		map_line_sanitize(char **line)
{
	char	*res;
	int		i;

	i = 0;
	while ((*line)[i])
	{
		if (!is_valid_cub_char((*line)[i]))
			return (1);
		i++;
	}
    res = ft_strjoin(*line, " ");
    free(*line);
    *line = res;
	return (0);
}

int     validate_map_edges(int y, int x, char **map, t_vars *vars)
{
	int		invalid;

    if (!iscset(map[y][x], "02NSWE"))
        return (0);
	if (map[y][x] == '0')
		map[y][x] = 'O';
    else if (map[y][x] == '2')
        map[y][x] = 'I';
	invalid = 0;
    if (ismap(y - 1, x, &(vars->world)) && map[y - 1][x] != ' ')
        invalid += validate_map_edges(y - 1, x, map, vars);
    else
        invalid = 1;
    if (ismap(y + 1, x, &(vars->world)) && map[y + 1][x] != ' ')
        invalid += validate_map_edges(y + 1, x, map, vars);
	else
		invalid = 1;
    if (ismap(y, x - 1, &(vars->world)) && map[y][x - 1] != ' ')
        invalid += validate_map_edges(y, x - 1, map, vars);
	else
		invalid = 1;
    if (ismap(y, x + 1, &(vars->world)) && map[y][x + 1] != ' ')
        invalid += validate_map_edges(y, x + 1, map, vars);
	else
		invalid = 1;
	return (invalid);
}

int		validate_map(char **map, t_vars *vars)
{
	int		playerfound;
	int		i;
	int		i2;

    playerfound = 0;
	i = 0;
	while (i < vars->world.map_height)
	{
		i2 = 0;
		while (map[i][i2])
		{
			if (map[i][i2] == 'N')
				vars->world.lookdir = DEG90;
			else if (map[i][i2] == 'E')
				vars->world.lookdir = 0;
			else if (map[i][i2] == 'W')
				vars->world.lookdir = DEG180;
			else if (map[i][i2] == 'S')
				vars->world.lookdir = DEG270;
			if (map[i][i2] == 'N' || map[i][i2] == 'E' ||
					map[i][i2] == 'W' || map[i][i2] == 'S')
			{
				vars->world.playerx = GRID * i2 + (GRID / 2);
				vars->world.playery = GRID * i + (GRID / 2);
                if (playerfound || validate_map_edges(i, i2, map, vars))
                    return (1);
                playerfound = 1;
			}
			i2++;
		}
		i++;
	}
	return (0);
}

int		create_sprite_map(t_vars *vars)
{
	int		y;
	int		x;

	vars->world.spritemap = (t_sprite ***)ft_calloc(sizeof(t_sprite **), vars->world.map_height);
	if (!vars->world.spritemap)
		return (1);
	y = 0;
	while (y < vars->world.map_height)
	{
		vars->world.spritemap[y] = (t_sprite **)ft_calloc(sizeof(t_sprite *), ft_strlen(vars->world.map[y]));
		if (!vars->world.spritemap[y])
			return (1);//TODO function that frees everything upto there
		x = 0;
		while (vars->world.map[y][x])
		{
			if (vars->world.map[y][x] == 'I')
			{
				vars->world.spritemap[y][x] = (t_sprite *)ft_calloc(sizeof(t_sprite), 1);
				if (!vars->world.spritemap[y][x])
					return (1);//TODO function that frees everything malloc b4 this
				vars->world.spritemap[y][x]->x = x * GRID + (GRID >> 1);
				vars->world.spritemap[y][x]->y = y * GRID + (GRID >> 1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

int		parse_map(t_vars *vars, int fd)
{
	int		ret;
	char	*line;

	vars->world.map_height = 0;
	while (1)
	{
		line = NULL;
		ret = get_next_line(fd, &line);
		if (ret == 0)
			break ;
		if (ret == -1)
			free_everything(vars, fd, line);
		if (map_line_sanitize(&line))
			return (free_everything(vars, fd, line));
		if (array_append(&(vars->world.map), line, vars->world.map_height))
			return (free_everything(vars, fd, line));
		vars->world.map_height++;
	}
	free(line);
	vars->world.max_y = vars->world.map_height * GRID;
	if (validate_map(vars->world.map, vars))
		return (free_everything(vars, fd, NULL));
	if (create_sprite_map(vars))
		return (1);//TODO print_error MEM MANAGE
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_map.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/09/04 13:54:10 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <get_next_line.h>
#include <libft.h>
#include <mlx.h>
#include "cub3d.h"

int		validate_grid(int y, int x, char **map, t_vars *vars)
{
	if (ismap(y, x, &(vars->world)) && map[y][x] != ' ')
		return (validate_map_edges(y, x, map, vars));
	else
		return (1);
}

int		validate_map_edges(int y, int x, char **map, t_vars *vars)
{
	int		invalid;

	if (!iscset(map[y][x], "02"))
		return ((iscset(map[y][x], "NSWE")));
	else
		map[y][x] = (map[y][x] == '0') ? 'O' : 'I';
	invalid = 0;
	invalid += validate_grid(y - 1, x, map, vars);
	invalid += validate_grid(y + 1, x, map, vars);
	invalid += validate_grid(y, x - 1, map, vars);
	invalid += validate_grid(y, x + 1, map, vars);
	if (DIAGONAL)
	{
		invalid += validate_grid(y - 1, x - 1, map, vars);
		invalid += validate_grid(y + 1, x + 1, map, vars);
		invalid += validate_grid(y + 1, x - 1, map, vars);
		invalid += validate_grid(y - 1, x + 1, map, vars);
	}
	return (invalid);
}

int		validate_map(char **map, t_vars *vars)
{
	int		playerfound;
	int		y;
	int		x;

	playerfound = 0;
	y = 0;
	while (y < vars->world.map_height)
	{
		x = 0;
		while (map[y][x])
		{
			if (iscset(map[y][x], "NESW"))
			{
				vars->world.lookdir = get_lookdir(&(map[y][x]));
				if (playerfound || validate_map_edges(y, x, map, vars))
					return (1);
				playerfound = 1;
				vars->world.playerx = GRID * x + (GRID >> 1);
				vars->world.playery = GRID * y + (GRID >> 1);
			}
			x++;
		}
		y++;
	}
	return (!playerfound);
}

int		create_sprite_map(t_world *w)
{
	int		y;
	int		x;

	w->spritemap = (t_sprite ***)ft_calloc(sizeof(t_sprite **), w->map_height + 1);
	if (!w->spritemap)
		return (1);
	y = 0;
	while (y < w->map_height)
	{
		w->spritemap[y] = (t_sprite **)ft_calloc(sizeof(t_sprite *),
														ft_strlen(w->map[y]));
		if (!w->spritemap[y])
			return (1);
		x = 0;
		while (w->map[y][x])
		{
			if (w->map[y][x] == 'I')
				if (create_sprite(&w->spritemap[y][x], x, y))
					return (1);
			x++;
		}
		y++;
	}
	return (0);
}

void	parse_map(t_vars *vars, int fd)
{
	int		ret;
	char	*line;

	ret = 0;
	while (1)
	{
		line = NULL;
		ret = get_next_line(fd, &line);
		if (ret == 0 && line && !*line)
			break ;
		if (ret == -1)
			print_error("File read failed", vars, fd, line);
		if (map_line_sanitize(&line))
			print_error("Invalid map characters", vars, fd, line);
		if (array_append(&(vars->world.map), line, vars->world.map_height))
			print_error("Failed to append map line", vars, fd, line);
		vars->world.map_height++;
	}
	if (line && !*line)
		free(line);
	vars->world.max_y = vars->world.map_height * GRID;
	if (validate_map(vars->world.map, vars))
		print_error("Broken map edges/Invalid player pos", vars, fd, NULL);
	if (create_sprite_map(&vars->world))
		print_error("Failed to allocate sprites", vars, fd, NULL);
}

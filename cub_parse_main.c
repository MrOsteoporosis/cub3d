/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_main.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/09/03 13:58:48 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <get_next_line.h>
#include <libft.h>
#include "cub3d.h"

int		free_spritemap(t_world *world)
{
	int		y;
	size_t	x;

	y = 0;
	while (world->spritemap && world->spritemap[y] && y < world->map_height)
	{
		x = 0;
		while (x < ft_strlen(world->map[y]))
		{
			if (world->spritemap[y][x])
				free(world->spritemap[y][x]);
			x++;
		}
		free(world->spritemap[y]);
		y++;
	}
	free(world->spritemap);
	return (1);
}

int		free_map(t_world *world)
{
	int		y;

	y = 0;
	while (world->map && y < world->map_height)
	{
		free(world->map[y]);
		y++;
	}
	if (world->map)
		free(world->map);
	return (1);
}

int		call_element_parser(t_vars *vars, char *line, int *elecount)
{
	int				i;
	static char		*ele[8] = {"R", "NO", "SO", "WE", "EA", "S ", "F", "C"};
	static t_efunc	efuncs[8] = {&parse_res, &parse_tex, &parse_tex,
		&parse_tex, &parse_tex, &parse_tex, &parse_color, &parse_color};

	i = 0;
	if (*line == '\0')
		return (0);
	while (i < 9)
	{
		if (i == 8)
			return (1);
		if (!ft_strncmp(line, ele[i], ft_strlen(ele[i])))
		{
			if (efuncs[i](vars, line))
				return (1);
			(*elecount)++;
			return (0);
		}
		i++;
	}
	return (0);
}

void	parse_cub(t_vars *vars, char *map_path)
{
	char	*line;
	int		fd;
	int		ret;
	int		elecount;

	fd = open(map_path, O_RDONLY);
	if (!fd)
		print_error("Failed to open .cub/Invalid path", vars, 0, NULL);
	elecount = 0;
	ret = 1;
	vars->world.colorceiling = -1;
	vars->world.colorfloor = -1;
	while (elecount < 8)
	{
		line = NULL;
		ret = get_next_line(fd, &line);
		if (ret != 1)
			print_error("Failed to read .cub/Unexpected EOF", vars, fd, line);
		if (call_element_parser(vars, line, &elecount))
			print_error("Invalid .cub element", vars, fd, line);
		free(line);
	}
	parse_map(vars, fd);
	close(fd);
}

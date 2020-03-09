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

int		array_append(char ***map, char *line, int currentlength)
{
	char	**res;
	int		i;

	res = (char **)ft_calloc(currentlength + 1, sizeof(char *));
	if (!res)
		return (1);
	i = 0;
	while (i < currentlength)
	{
		res[i] = (*map)[i];
		i++;
	}
	res[currentlength] = line;
	free((*map));
	*map = res;
	return (0);
}

int		map_line_sanitize(char *line)
{
	char	*res;
	int		i;
	int		valid;

	i = 0;
	while (line[i])
	{
		if (!is_valid_cub_char(*line))
			return (1);
		i++;
	}
	return (0);
}

int		validate_map(char **map, t_vars *vars)
{
	int		spritecount;
	int		i;
	int		i2;

	//pathfind around the map to check for unbroken edge wall
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
			}
			i2++;
		}
		i++;
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
		if (map_line_sanitize(line))
			return (free_everything(vars, fd, line));
		if (array_append(&(vars->world.map), line, vars->world.map_height))
			return (free_everything(vars, fd, line));
		vars->world.map_height++;
	}
	free(line);
	if (validate_map(vars->world.map, vars))
		return (free_everything(vars, fd, NULL));
	//2d array of t_sprite pointers with maps dimensions
	//loop over map
	//malloc for a link at every '2' and assign pointer
	//lst
	//	int	gridx;
	//	int	gridy;
	//	int queued;
	//	t_sprite *next
	//	t_sprite *prev
	vars->world.max_y = vars->world.map_height * GRID;
	return (0);
}

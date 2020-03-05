/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_map.c                                    :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 11:57:39 by averheij       #+#    #+#                */
/*   Updated: 2020/03/04 13:02:24 by averheij         ###   ########.fr       */
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

int		map_line_sanitize(char **lineorigin, char *line, int *ft_width)
{
	char	*res;
	int		i;
	int		valid;

	*ft_width = count_non_whitespace_char(line);
	res = (char *)ft_calloc(*ft_width + 1, sizeof(char));
	if (!res)
		return (1);
	i = 0;
	while (i < *ft_width && *line)
	{
		valid = is_valid_cub_char(*line);
		if (valid && (*(line + 1) && *(line + 1) != ' '))
			return (1);
		if (valid)
		{
			res[i] = *line;
			i++;
		}
		line++;
	}
	free(*lineorigin);
	*lineorigin = res;
	return (0);
}

int		validate_map(char **map, t_vars *vars)
{
	//pathfind around the map to check for 1s?
	//set player pos
	vars->world.playerx = GRID * 1 + (GRID / 2);
	vars->world.playery = GRID * 1 + (GRID / 2);
	vars->world.lookdir = DEG90 * 0;
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
		if (map_line_sanitize(&line, line, &(vars->world.map_width)))
			return (free_everything(vars, fd, line));
		if (array_append(&(vars->world.map), line, vars->world.map_height))
			return (free_everything(vars, fd, line));
		vars->world.map_height++;
	}
	free(line);
	if (validate_map(vars->world.map, vars))
		return (free_everything(vars, fd, NULL));
	vars->world.max_x = vars->world.map_width * GRID;
	vars->world.max_y = vars->world.map_height * GRID;
	return (0);
}
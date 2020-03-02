/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_map.c                                    :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 11:57:39 by averheij       #+#    #+#                */
/*   Updated: 2020/03/02 12:19:27 by averheij         ###   ########.fr       */
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

int		is_valid_cub_char(char c)
{
	int			i;
	static char	set[7] = {'0', '1', '2', 'N', 'S', 'W', 'E'};

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int		count_non_whitespace_char(char *line)
{
	int		count;

	while (*line)
	{
		if (*line != ' ')
			count++;
		line++;
	}
	return (count);
}

int		map_line_sanitize(char **lineorigin, char *line, int *ft_width)
{
	char	*res;
	int		i;
	int		valid;

	*ft_width = count_non_whitespace_char(line);
	res = (char *)ft_calloc(*ft_width, sizeof(char));
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
	//Validate closed edjes
	//Validate map widths same
	//Set player pos
	vars->world.max_x = vars->world.map_width * GRID;
	vars->world.max_y = vars->world.map_height * GRID;
	return (0);
}


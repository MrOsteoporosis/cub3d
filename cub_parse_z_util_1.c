/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_z_util_1.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/11 14:33:47 by averheij      #+#   #+#                  */
/*   Updated: 2020/05/11 16:38:30 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <libft.h>
#include "cub3d.h"

int		is_valid_cub_char(char c)
{
	int			i;
	static char	set[8] = {' ', '0', '1', '2', 'N', 'S', 'W', 'E'};

	i = 0;
	while (i < 8)
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

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

double	get_lookdir(char c)
{
	if (c == 'N')
		return (DEG90);
	else if (c == 'E')
		return (0);
	else if (c == 'W')
		return (DEG180);
	else if (c == 'S')
		return (DEG270);
}

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

int		create_sprite(t_sprite **sprite, int x, int y)
{
	*sprite = (t_sprite *)ft_calloc(sizeof(t_sprite), 1);
	if (!sprite)
		return (1);
	(*sprite)->x = x * GRID + (GRID >> 1);
	(*sprite)->y = y * GRID + (GRID >> 1);
	return (0);
}

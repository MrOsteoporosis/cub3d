/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averheij <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 11:15:01 by averheij          #+#    #+#             */
/*   Updated: 2020/03/02 12:29:14 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <get_next_line.h>
#include <libft.h>
#include <mlx.h>
#include "cub3d.h"

int		call_element_parser(t_vars *vars, char *line, int *elecount)
{
	int				i;
	static char		*ele[8] = {"R", "NO", "SO", "WE", "EA", "S ", "F", "C"};
	static t_efunc	efuncs[8] = {&parse_res, &parse_tex, &parse_tex,
		&parse_tex, &parse_tex, &parse_tex, &parse_color, &parse_color};

	i = 0;
	if (!*line)
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

int		parse_cub(t_vars *vars, char *map_path)
{
	char	*line;
	int		fd;
	int		ret;
	int		elecount;

	fd = open(map_path, O_RDONLY);
	if (!fd)
		return (1);
	elecount = 0;
	ret = 1;
	while (elecount < 8)
	{
		line = NULL;
		ret = get_next_line(fd, &line);
		if (ret != 1)
			return (free_line_and_close(fd, line));
		if (call_element_parser(vars, line, &elecount))
			return (free_line_and_close(fd, line));
		free(line);
	}
	if (parse_map(vars, fd))
		return (1);
	close(fd);
	return (0);
}

int		free_line_and_close(int fd, char *line)
{
	if (fd != -1)
		close(fd);
	if (line)
		free(line);
	return (1);
}

int		free_everything(t_vars *vars, int fd, char *line)
{
	int	i;

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
	free_line_and_close(fd, line);
	return (1);
}

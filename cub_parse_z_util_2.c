/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_z_util_2.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/11 14:32:55 by averheij      #+#   #+#                  */
/*   Updated: 2020/05/11 14:32:58 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "cub3d.h"

int		ft_iswhitespace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int		skip_comma(char **str)
{
	if (!(**str == ','))
		return (1);
	(*str)++;
	if (**str)
		return (0);
	return (1);
}

int		skip_passed_func(char **str, t_isfunc is)
{
	while (**str && is(**str))
		(*str)++;
	if (**str)
		return (0);
	return (1);
}

int		ismap(int y, int x, t_world *world)
{
	if (y < 0 || y >= world->map_height)
		return (0);
	world->map_width = ft_strlen(world->map[y]);
	if (x < 0 || x >= world->map_width)
		return (0);
	return (1);
}

int		iscset(char c, char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

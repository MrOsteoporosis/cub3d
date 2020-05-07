/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util_cub.c                                         :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/28 13:23:26 by averheij       #+#    #+#                */
/*   Updated: 2020/03/05 13:19:47 by averheij         ###   ########.fr       */
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

int		ft_iswhitespace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int		ft_skip_comma(char **str)
{
	if (!(**str == ','))
		return (1);
	(*str)++;
	if (**str)
		return (0);
	return (1);
}

int		ft_skip_passed_func(char **str, t_isfunc is)
{
	while (**str && is(**str))
		(*str)++;
	if (**str)
		return (0);
	return (1);
}

int     ismap(int y, int x, t_world *world)
{
	if (y < 0 || y >= world->map_height)
		return (0);
	world->map_width = ft_strlen(world->map[y]);
	if (x < 0 || x >= world->map_width)
		return (0);
	return (1);
}

int     iscset(char c, char *set)
{
    while (*set)
    {
        if (c == *set)
            return (1);
        *set++;
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

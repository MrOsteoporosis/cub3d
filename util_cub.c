/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util_cub.c                                         :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/28 13:23:26 by averheij       #+#    #+#                */
/*   Updated: 2020/03/02 12:31:29 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "cub3d.h"

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

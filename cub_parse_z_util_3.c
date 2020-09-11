/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_z_util_3.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/09/11 12:43:09 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "cub3d.h"

t_data	*get_tex_pointer(t_vars *vars, char *line)
{
	if (!ft_strncmp(line, "NO", 2))
		return (&(vars->no));
	else if (!ft_strncmp(line, "SO", 2))
		return (&(vars->so));
	else if (!ft_strncmp(line, "WE", 2))
		return (&(vars->we));
	else if (!ft_strncmp(line, "EA", 2))
		return (&(vars->ea));
	else if (!ft_strncmp(line, "S", 1))
		return (&(vars->s));
	return (NULL);
}

int		detect_png_xpm(char *line)
{
	int	len;

	len = ft_strlen(line) - 1;
	while (line[len] == 32)
		len--;
	line[len + 1] = '\0';
	if (len > 3 && !ft_strncmp((line + (len - 3)), ".png", 4))
		return (1);
	else
		return (0);
}

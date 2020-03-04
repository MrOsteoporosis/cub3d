/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_meta.c                                   :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/28 13:27:44 by averheij       #+#    #+#                */
/*   Updated: 2020/03/04 14:21:32 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <mlx.h>
#include "cub3d.h"

int		parse_rgb_partial(int *color, char **str, int notlast)
{
	if (ft_skip_passed_func(str, &ft_iswhitespace))
		return (1);
	if (!ft_isdigit(**str))
		return (1);
	*color = ft_atoi(*str);
	if (notlast && ft_skip_passed_func(str, &ft_isdigit))
		return (1);
	if (notlast && ft_skip_comma(str))
		return (1);
	return (0);
}

int		parse_color(t_vars *vars, char *line)
{
	int	red;
	int	green;
	int	blue;
	int	*colorpointer;

	if (!ft_strncmp(line, "C", 1))
		colorpointer = &(vars->world.colorceiling);
	else if (!ft_strncmp(line, "F", 1))
		colorpointer = &(vars->world.colorfloor);
	line++;
	if (parse_rgb_partial(&red, &line, 1))
		return (1);
	if (parse_rgb_partial(&green, &line, 1))
		return (1);
	if (parse_rgb_partial(&blue, &line, 0))
		return (1);
	if (red > 255 || blue > 255 || green > 255)
		return (1);
	*colorpointer = create_trgb(0, red, green, blue);
	return (0);
}

void	validate_res(t_vars *vars)
{
	int		max_resx;
	int		max_resy;

	mlx_get_screen_size(vars->mlx, &max_resx, &max_resy);
	max_resy -= 44;
	if (vars->img.resx > max_resx)
		vars->img.resx = max_resx;
	if (vars->img.resy > max_resy)
		vars->img.resy = max_resy;
}

int		parse_res(t_vars *vars, char *line)
{
	line++;
	if (ft_skip_passed_func(&line, &ft_iswhitespace))
		return (1);
	if (!ft_isdigit(*line))
		return (1);
	vars->img.resx = ft_atoi(line);
	if (ft_skip_passed_func(&line, &ft_isdigit))
		return (1);
	if (ft_skip_passed_func(&line, &ft_iswhitespace))
		return (1);
	if (!ft_isdigit(*line))
		return (1);
	vars->img.resy = ft_atoi(line);
	if (!(vars->img.resx) || !(vars->img.resy))
		return (1);
	validate_res(vars);
	vars->img.halfresy = vars->img.resy / 2;
	return (0);
}

int		parse_tex(t_vars *vars, char *line)
{
	t_data	*tex;

	if (!ft_strncmp(line, "NO", 2))
		tex = &(vars->no);
	else if (!ft_strncmp(line, "SO", 2))
		tex = &(vars->so);
	else if (!ft_strncmp(line, "WE", 2))
		tex = &(vars->we);
	else if (!ft_strncmp(line, "EA", 2))
		tex = &(vars->ea);
	else if (!ft_strncmp(line, "S ", 2))
		tex = &(vars->s);
	if (ft_skip_passed_func(&line, &ft_isalpha))
		return (1);
	if (ft_skip_passed_func(&line, &ft_iswhitespace))
		return (1);
	tex->img = mlx_png_file_to_image(vars->mlx, line, &tex->resx, &tex->resy);
	if (!tex->img)
		return (1);
	tex->addr = mlx_get_data_addr(tex->img,
			&tex->bits_per_pixel, &tex->line_length, &tex->endian);
	if (!tex->addr)
		return (1);
	return (0);
}

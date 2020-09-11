/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub_parse_meta.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/09/11 10:52:44 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <mlx.h>
#include "cub3d.h"

int		parse_rgb_partial(int *color, char **str, int notlast)
{
	if (!ft_isdigit(**str))
		return (1);
	*color = ft_atoi(*str);
	if (notlast && skip_passed_func(str, &ft_isdigit))
		return (1);
	if (RGBSPACE && notlast && skip_passed_func(str, &ft_iswhitespace))
		return (1);
	if (notlast && skip_comma(str))
		return (1);
	if (RGBSPACE && notlast && skip_passed_func(str, &ft_iswhitespace))
		return (1);
	if (!notlast)
	{
		skip_passed_func(str, &ft_isdigit);
		return (!skip_passed_func(str, &ft_iswhitespace));
	}
	return (0);
}

int		parse_color(t_vars *vars, char *line)
{
	int	red;
	int	green;
	int	blue;
	int	*colorpointer;

	colorpointer = NULL;
	if (!ft_strncmp(line, "C", 1))
		colorpointer = &(vars->world.colorceiling);
	else if (!ft_strncmp(line, "F", 1))
		colorpointer = &(vars->world.colorfloor);
	if (!colorpointer || *colorpointer != -1)
		return (1);
	line++;
	if (skip_passed_func(&line, &ft_iswhitespace))
		return (1);
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
	if (vars->img.resx == -1 || vars->img.resx > max_resx)
		vars->img.resx = max_resx;
	if (vars->img.resy == -1 || vars->img.resy > max_resy)
		vars->img.resy = max_resy;
}

int		parse_res(t_vars *vars, char *line)
{
	line++;
	if (skip_passed_func(&line, &ft_iswhitespace))
		return (1);
	if (!ft_isdigit(*line))
		return (1);
	vars->img.resx = ft_atoi(line);
	if (skip_passed_func(&line, &ft_isdigit))
		return (1);
	if (skip_passed_func(&line, &ft_iswhitespace))
		return (1);
	if (!ft_isdigit(*line))
		return (1);
	vars->img.resy = ft_atoi(line);
	skip_passed_func(&line, &ft_isdigit);
	if (!skip_passed_func(&line, &ft_iswhitespace))
		return (1);
	if (!(vars->img.resx) || !(vars->img.resy))
		return (1);
	if (!vars->save)
		validate_res(vars);
	vars->img.halfresy = vars->img.resy / 2;
	return (0);
}

int		parse_tex(t_vars *vars, char *line)
{
	t_data	*t;

	t = NULL;
	if (!ft_strncmp(line, "NO", 2))
		t = &(vars->no);
	else if (!ft_strncmp(line, "SO", 2) || !ft_strncmp(line, "WE", 2))
		t = !ft_strncmp(line, "SO", 2) ? &(vars->so) : &(vars->we);
	else if (!ft_strncmp(line, "EA", 2) || !ft_strncmp(line, "S", 1))
		t = !ft_strncmp(line, "EA", 2) ? &(vars->ea) : &(vars->s);
	line = line + ((t != &(vars->s)) ? 2 : 1);
	if (t->img || skip_passed_func(&line, &ft_iswhitespace))
		return (1);
	printf("%s\n", line);
	if (ft_strlen(line) > 4
			&& !ft_strncmp((line + (ft_strlen(line) - 4)), ".png", 4))//somehow trim whitespace off the end?
		t->img = mlx_png_file_to_image(vars->mlx, line, &t->resx, &t->resy);
	else
	{
		t->img = mlx_xpm_file_to_image(vars->mlx, line, &t->resx, &t->resy);
		printf("found xpm\n");
	}
	printf("creating texture\n");
	if (!t->img)
		return (1);
	t->addr = mlx_get_data_addr(t->img,
			&t->bits_per_pixel, &t->line_length, &t->endian);
	if (!t->addr)
		return (1);
	return (0);
}

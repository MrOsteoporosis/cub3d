/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 11:57:39 by averheij       #+#    #+#                */
/*   Updated: 2020/02/24 11:07:05 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <get_next_line.h>
#include <libft.h>
#include <mlx.h>
#include "cub3d.h"

int		ft_iswhitespace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

int     ft_skip_comma(char **str)
{
    if (!(**str == ','))
        return (1);
    (*str)++;
    if (**str)
        return (0);
    return (1);
}

int     ft_skip_passed_func(char **str, t_isfunc is)
{
    while (**str && is(**str))
        (*str)++;
    if (**str)
        return (0);
    return (1);
}

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

int     parse_color(t_vars *vars, char *line)
{
    int     red;
    int     green;
    int     blue;
    int     *colorpointer;

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
	printf("color: %X\n", *colorpointer);
    return (0);
}

int     parse_res(t_vars *vars, char *line)
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
    if (vars->img.resx > MAX_RESX)
        vars->img.resx = MAX_RESX;
    if (vars->img.resy > MAX_RESY)
        vars->img.resy = MAX_RESY;
	vars->img.halfresy = vars->img.resy / 2;
    printf("resx: %d resy: %d\n", vars->img.resx, vars->img.resy);
    return (0);
}

int     parse_tex(t_vars *vars, char *line)
{
    t_tex   *tex;

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
    tex->path = ft_strdup(line);
	tex->img.img = mlx_png_file_to_image(vars->mlx, tex->path, &tex->width, &tex->height);
	if (!tex->img.img)
		return (1);
	tex->img.addr = mlx_get_data_addr(tex->img.img, &tex->img.bits_per_pixel, &tex->img.line_length, &tex->img.endian);
	if (!tex->img.addr)
		return (1);
    printf("tex path: %s width%d height%d\n", tex->path, tex->width, tex->height);
    return (0);
}

void    parse_cub(t_vars *vars, char *map_path)
{
    char    *line;
    int     fd;
    int     ret;
    int     i;
    int     elecount;
    char    *ele[8] = {"R", "NO", "SO", "WE", "EA", "S ", "F", "C"};
    t_efunc efuncs[8] = {&parse_res, &parse_tex, &parse_tex, &parse_tex, &parse_tex, &parse_tex, &parse_color, &parse_color};

    fd = open(map_path, O_RDONLY);
    elecount = 0;
    ret = 1;
    while (ret && elecount < 8)
    {
        ret = get_next_line(fd, &line);
        printf("OUT _%d_%d_%s_\n", fd, ret, line);
        i = 0;
        while (i < 9)
        {
			if (i == 8)
				printf("INVALID ELEMENT FAM");
            if(!ft_strncmp(line, ele[i], ft_strlen(ele[i])))
            {
                if (efuncs[i](vars, line))
                    printf("ERROR FAM");
                elecount++;
				break;
            }
            i++;
        }
        free(line);
        if (ret != 1)
            close(fd);
    }
    printf("ELES DONE %d\n", elecount);
    //Write an array append to parse map
    //Remember to validate closed edges, consistent lengths?
    //Assign struct vars: playerx etc.
}
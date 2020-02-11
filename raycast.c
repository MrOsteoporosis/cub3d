/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averheij <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:51:20 by averheij          #+#    #+#             */
/*   Updated: 2020/02/11 10:42:24 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

int     draw_texture_column(t_vars *vars, t_caster *caster, t_tex *tex)
{
    char	        *dst;
    char            *color;
    int		        i;
    int             y;
    int             endy;
    int             tex_column;

    tex_column = (tex->width * caster->near->tex_offset) >> GRIDPOW;
    y = HALF_FRAME_HEIGHT - (caster->near->height >> 1);
    endy = HALF_FRAME_HEIGHT + (caster->near->height >> 1);
    i = -1 * (HALF_FRAME_HEIGHT - (caster->near->real_height >> 1)) + y;
    while (y < endy)
    {
        dst = vars->img.addr + (y * vars->img.line_length + caster->column * (vars->img.bits_per_pixel >> 3));
        color = tex->img.addr + (((tex->height * i) / caster->near->real_height) * tex->img.line_length + tex_column * (tex->img.bits_per_pixel >> 3));
        *(unsigned int*)dst = *(unsigned int*)color;
        i++;
        y++;
    }
    return (0);
}

int		calc_distance(t_vars *vars, t_caster *caster)
{
	float	temp;
	float	dist;
	int		height;
	float	sin_a;

	sin_a = sin(caster->a);
    caster->h.dist = ft_abs((vars->world.playery - caster->h.y) / sin_a);
    caster->v.dist = ft_abs((vars->world.playery - caster->v.y) / sin_a);
    caster->near = &(caster->v);
	if (caster->h.dist < caster->v.dist)
		caster->near = &(caster->h);
	caster->near->dist = caster->near->dist * cos(caster->raydir);
	caster->near->height = (GRID / caster->near->dist) * vars->world.proj_plane_dist;
    caster->near->real_height = caster->near->height;
	if (caster->near->height > FRAME_HEIGHT)
		caster->near->height = FRAME_HEIGHT;
	return (1);
}

int     cast_vertical(t_vars *vars, t_caster *caster, float tan_a)
{
    caster->v.xincr = GRID;
    caster->v.yincr = GRID * tan_a;
    if (caster->a < DEG270 && caster->a > DEG90)
    {
        caster->v.x = (((int)vars->world.playerx >> GRIDPOW) << GRIDPOW) - 1;
        caster->v.xincr *= -1;
    }
    else
    {
        caster->v.x = (((int)vars->world.playerx >> GRIDPOW) << GRIDPOW) + GRID;
        caster->v.yincr *= -1;
    }
    caster->v.y = vars->world.playery + ((vars->world.playerx - caster->v.x) * tan_a);
    caster->v.gridx = caster->v.x / GRID;
    caster->v.gridy = caster->v.y / GRID;
    check_bounds(&(vars->world), &(caster->v));
    if(!caster->v.foundwall)
        extendray(vars, &(caster->v));
    return (0);
}

int     cast_horizontal(t_vars *vars, t_caster *caster, float tan_a)
{
    caster->h.yincr = GRID;
    caster->h.xincr = GRID / tan_a;
    if (caster->a < DEG180 && caster->a > 0)
    {
        caster->h.y = (((int)vars->world.playery >> GRIDPOW) << GRIDPOW) - 1;
        caster->h.yincr *= -1;
    }
    else
    {
        caster->h.y = (((int)vars->world.playery >> GRIDPOW) << GRIDPOW) + GRID;
        caster->h.xincr *= -1;
    }
    caster->h.x = vars->world.playerx + ((vars->world.playery - caster->h.y) / tan_a);
    caster->h.gridx = caster->h.x / GRID;
    caster->h.gridy = caster->h.y / GRID;
    check_bounds(&(vars->world), &(caster->h));
    if(!caster->h.foundwall)
        extendray(vars, &(caster->h));
    return (0);
}

int     cast_ray(t_vars *vars)
{
    t_caster	caster;
	float		tan_a;

    caster.raydir = HALF_FOV;
	caster.column = 1;
	while (caster.column <= FRAME_WIDTH)
	{
		caster.a = ray_angle(vars->world.lookdir, caster.raydir);
		tan_a = tan(caster.a);
        cast_horizontal(vars, &caster, tan_a);
        cast_vertical(vars, &caster, tan_a);
        calc_offsets(vars, &caster);
        calc_distance(vars, &caster);
        draw_texture_column(vars, &caster, caster.near->tex);
		caster.raydir -= vars->world.radians_per_pixel;
		caster.column++;
	}
    return (0);
}

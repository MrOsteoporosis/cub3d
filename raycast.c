/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averheij <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:51:20 by averheij          #+#    #+#             */
/*   Updated: 2020/02/10 13:48:38 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

int     draw_texture_column(t_vars *vars, t_caster *caster)
{
    char	        *dst;
    char            *color;
    int		        i;
    int             y;
    int             tex_height;
    int             tex_column;
    int             tex_h_offset;

    tex_column = (vars->no.width * caster->near->tex_offset) >> GRIDPOW;
    tex_height = ((vars->no.height * caster->near->height) / caster->near->real_height);
    tex_h_offset = (((caster->near->real_height - caster->near->height) >> 1) * vars->no.height) / caster->near->real_height;
    y = HALF_FRAME_HEIGHT - (caster->near->height >> 1);
    i = 0;
    while (i < caster->near->height)
    {
        dst = vars->img.addr + ((y + i) * vars->img.line_length + caster->column * (vars->img.bits_per_pixel / 8));
        color = vars->no.img.addr + ((((tex_height * i) / caster->near->height) + tex_h_offset) * vars->no.img.line_length + tex_column * (vars->no.img.bits_per_pixel / 8));
        *(unsigned int*)dst = *(unsigned int*)color;
        i++;
    }
    return (0);
}

int		distanceanddraw(t_vars *vars, t_caster *caster)
{
	float	temp;
	float	dist;
	int		height;
	float	sin_a;

	sin_a = sin(caster->a);
	caster->h.dist = (vars->world.playery - caster->h.y) / sin_a;
	if (caster->h.dist < 0)
		caster->h.dist *= -1;
	caster->v.dist = (vars->world.playery - caster->v.y) / sin_a;
	if (caster->v.dist < 0)
		caster->v.dist *= -1;
    caster->near = &(caster->v);
	if (caster->h.dist < caster->v.dist)
		caster->near = &(caster->h);
	caster->near->dist = caster->near->dist * cos(caster->raydir);
	caster->near->height = (GRID / caster->near->dist) * vars->world.proj_plane_dist;
    caster->near->real_height = caster->near->height;
	if (caster->near->height > FRAME_HEIGHT)
		caster->near->height = FRAME_HEIGHT;
    draw_texture_column(vars, caster);
	return (1);
}

int     calc_offsets(t_caster *caster)
{
    if (caster->a < DEG270 && caster->a > DEG90)
        caster->v.tex_offset = (int)caster->v.y % GRID;
    else
        caster->v.tex_offset = GRID - ((int)caster->v.y % GRID);
    if (caster->a < DEG180 && caster->a > 0)
        caster->h.tex_offset = (int)caster->h.x % GRID;
    else
        caster->h.tex_offset = GRID - ((int)caster->h.x % GRID);
    return (0);
}

int		extendray(t_vars *vars, t_ray *ray)
{
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->gridx = ray->x / GRID;
		ray->gridy = ray->y / GRID;
		check_bounds(&(vars->world), ray);
	}
	return (0);
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
	{//Use shift ops, right shift by 8 to divide by GRID
		caster.a = ray_angle(vars->world.lookdir, caster.raydir);
		tan_a = tan(caster.a);
        cast_horizontal(vars, &caster, tan_a);
        cast_vertical(vars, &caster, tan_a);
        calc_offsets(&caster);
        distanceanddraw(vars, &caster);
		caster.raydir -= vars->world.radians_per_pixel;
		caster.column++;
	}
    return (0);
}

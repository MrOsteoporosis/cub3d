/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averheij <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:51:20 by averheij          #+#    #+#             */
/*   Updated: 2020/02/06 11:24:02 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

int		distanceanddraw(t_vars *vars, t_caster *caster)
{
	float	temp;
	float	dist;
	int		height;
	float	sin_a;

	sin_a = sin(caster->a);
	dist = (vars->world.playery - caster->h.y) / sin_a;
	if (dist < 0)
		dist *= -1;
	temp = (vars->world.playery - caster->v.y) / sin_a;
	if (temp < 0)
		temp *= -1;
	if (dist > temp)
		dist = temp;
	dist = dist * cos(caster->raydir);
	height = (64 / dist) * vars->world.proj_plane_dist;
	if  (height > FRAME_HEIGHT)
		height = FRAME_HEIGHT;
	my_mlx_sliver_put(&(vars->img), caster->column,
            HALF_FRAME_HEIGHT - (height >> 1), height,
            create_trgb(0, 100, 100, 100));
	return (1);
}

int		extendray(t_vars *vars, t_ray *ray)
{
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->gridx = ray->x / 64;
		ray->gridy = ray->y / 64;
		check_bounds(&(vars->world), ray);
	}
	return (0);
}

int     cast_vertical(t_vars *vars, t_caster *caster, float tan_a)
{
    caster->v.xincr = 64;
    caster->v.yincr = 64 * tan_a;
    if (caster->a < 270DEG && caster->a > 90DEG)
    {
        caster->v.x = (((int)vars->world.playerx / 64) * 64) - 1;
        caster->v.xincr *= -1;
    }
    else
    {
        caster->v.x = (((int)vars->world.playerx / 64) * 64) + 64;
        caster->v.yincr *= -1;
    }
    caster->v.y = vars->world.playery + ((vars->world.playerx - caster->v.x) * tan_a);
    caster->v.gridx = caster->v.x / 64;
    caster->v.gridy = caster->v.y / 64;
    check_bounds(&(vars->world), &(caster->v));
    if(!caster->v.foundwall)
        extendray(vars, &(caster->v));
    return (0);
}

int     cast_horizontal(t_vars *vars, t_caster *caster, float tan_a)
{
    caster->h.yincr = 64;
    caster->h.xincr = 64 / tan_a;
    if (caster->a < 180DEG && caster->a > 0)
    {
        caster->h.y = (((int)vars->world.playery / 64) * 64) - 1;
        caster->h.yincr *= -1;
    }
    else
    {
        caster->h.y = (((int)vars->world.playery / 64) * 64) + 64;
        caster->h.xincr *= -1;
    }
    caster->h.x = vars->world.playerx + ((vars->world.playery - caster->h.y) / tan_a);
    caster->h.gridx = caster->h.x / 64;
    caster->h.gridy = caster->h.y / 64;
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
	{//Use shift ops, right shift by 8 to divide by 64
		caster.a = ray_angle(vars->world.lookdir, caster.raydir);
		tan_a = tan(caster.a);
        cast_horizontal(vars, &caster, tan_a);
        cast_vertical(vars, &caster, tan_a);
        distanceanddraw(vars, &caster);
		caster.raydir -= vars->world.radians_per_pixel;
		caster.column++;
	}
    return (0);
}

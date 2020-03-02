/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util_ray.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/06 10:57:33 by averheij       #+#    #+#                */
/*   Updated: 2020/03/02 10:57:43 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <libft.h>
#include "cub3d.h"

float	ray_angle(float lookdir, float raydir)
{
	float a;

	a = lookdir + raydir;
	if (a > M_PI2)
		a -= M_PI2;
	return (a);
}

int		check_bounds(t_world *world, t_ray *ray)
{
	ray->foundwall = 0;
	ray->safe = 0;
	if (ray->x < 0 || ray->x >= world->max_x)
		return (0);
	if (ray->y < 0 || ray->y >= world->max_y)
		return (0);
	if (world->map[ray->gridy][ray->gridx] == '1')
		ray->foundwall = 1;
	ray->safe = 1;
	return (1);
}

int		ft_abs(int x)
{
	int y;

	y = (x >> 31);
	return ((x ^ y) - y);
}

void	calc_offsets(t_vars *vars, t_caster *caster)
{
	if (caster->a < DEG270 && caster->a > DEG90)
	{
		caster->v.tex_offset = (int)caster->v.y % GRID;
		caster->v.tex = &(vars->we);
	}
	else
	{
		caster->v.tex_offset = GRID - ((int)caster->v.y % GRID);
		caster->v.tex = &(vars->ea);
	}
	if (caster->a < DEG180 && caster->a > 0)
	{
		caster->h.tex_offset = (int)caster->h.x % GRID;
		caster->h.tex = &(vars->so);
	}
	else
	{
		caster->h.tex_offset = GRID - ((int)caster->h.x % GRID);
		caster->h.tex = &(vars->no);
	}
}

void	extendray(t_world *world, t_ray *ray)
{
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->gridx = ray->x / GRID;
		ray->gridy = ray->y / GRID;
		check_bounds(world, ray);
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_z_util.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/11 20:59:38 by averheij      #+#   #+#                  */
/*   Updated: 2020/05/11 20:59:39 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <libft.h>
#include "cub3d.h"

double	ray_angle(double lookdir, double raydir)
{
	double a;

	a = lookdir + raydir;
	if (a > M_PI2)
		a -= M_PI2;
	return (a);
}

int		check_ray_bounds(t_world *world, t_ray *ray)
{
	ray->gridx = ray->x / GRID;
	ray->gridy = ray->y / GRID;
	ray->foundwall = 0;
	ray->safe = 0;
	if (ray->y < 0 || ray->y >= world->max_y)
		return (0);
	world->map_width = ft_strlen(world->map[ray->gridy]);
	world->max_x = world->map_width << GRIDPOW;
	if (ray->x < 0 || ray->x >= world->max_x)
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

void	set_tex(t_vars *vars, t_caster *caster)
{
	if (caster->a < DEG270 && caster->a > DEG90)
		caster->v.tex = &(vars->we);
	else
		caster->v.tex = &(vars->ea);
	if (caster->a < DEG180 && caster->a > 0)
		caster->h.tex = &(vars->so);
	else
		caster->h.tex = &(vars->no);
}

void	get_tan_a(double a, double *tan_a, int *taniszero)
{
	*tan_a = tan(a);
	*taniszero = 0;
	if ((int)(*tan_a * 1000000) == 0)
	{
		*tan_a = 0.000001;
		*taniszero = 1;
	}
}

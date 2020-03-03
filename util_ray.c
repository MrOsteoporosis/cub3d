/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util_ray.c                                         :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/06 10:57:33 by averheij       #+#    #+#                */
/*   Updated: 2020/03/03 14:38:45 by averheij         ###   ########.fr       */
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
	if (ray->y < 0 || ray->y >= world->max_y)
		return (0);
	world->map_width = ft_strlen(world->map[ray->gridy]);
	world->max_x = world->map_width << GRIDPOW;
	if (ray->x < 0 || ray->x >= world->max_x)
		return (0);
	if (world->map[ray->gridy][ray->gridx] == '1' )
		ray->foundwall = 1;
	//check for sprite
		//save x y in linked lst
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

void	extendray(t_world *world, t_ray *ray)
{
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->gridx = ray->x / GRID;
		ray->gridy = ray->y / GRID;
		check_bounds(world, ray);
		ray->tex_offset = (int)ray->y % GRID;
		if (ray->foundwall && ray->tex_offset == GRID - 1 &&
				ray->gridy + 1 < world->map_height &&
				world->map[ray->gridy + 1][ray->gridx] != '1')
			ray->foundwall = 0;
	}
}

void	extendray_b(t_world *world, t_ray *ray)
{
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->gridx = ray->x / GRID;
		ray->gridy = ray->y / GRID;
		check_bounds(world, ray);
		ray->tex_offset = (int)ray->x % GRID;
		if (ray->foundwall && ray->tex_offset == GRID - 1 &&
				ray->gridx + 1 < world->map_height &&
				world->map[ray->gridy][ray->gridx + 1] != '1')
			ray->foundwall = 0;
	}
}

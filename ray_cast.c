/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_cast.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/11 20:56:51 by averheij      #+#   #+#                  */
/*   Updated: 2020/05/11 20:56:52 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

void	cast_vertical(t_world *world, t_ray *ray, double a, double tan_a)
{
	ray->xincr = GRID;
	ray->yincr = GRID * tan_a;
	if (a < DEG270 && a > DEG90)
	{
		ray->x = (((int)world->playerx >> GRIDPOW) << GRIDPOW);
		ray->off_mod = -1;
		ray->xincr *= -1;
	}
	else
	{
		ray->x = (((int)world->playerx >> GRIDPOW) << GRIDPOW) + GRID;
		ray->off_mod = 0;
		ray->yincr *= -1;
	}
	ray->y = world->playery + ((world->playerx - ray->x) * tan_a);
	ray->yorigin = ray->y;
	ray->xorigin = ray->x;
	extend_vertical(world, ray);
}

void	extend_vertical(t_world *world, t_ray *ray)
{
	ray->x += ray->off_mod;
	check_ray_bounds(world, ray);
	ray->x -= ray->off_mod;
	ray->tex_offset = (int)ray->y % GRID;
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->x += ray->off_mod;
		check_ray_bounds(world, ray);
		ray->x -= ray->off_mod;
		ray->tex_offset = (int)ray->y % GRID;
	}
}

void	cast_horizontal(t_world *world, t_ray *ray, double a, double tan_a)
{
	ray->yincr = GRID;
	ray->xincr = GRID / tan_a;
	if (a < DEG180 && a > 0)
	{
		ray->y = (((int)world->playery >> GRIDPOW) << GRIDPOW);
		ray->off_mod = -1;
		ray->yincr *= -1;
	}
	else
	{
		ray->y = (((int)world->playery >> GRIDPOW) << GRIDPOW) + GRID;
		ray->off_mod = 0;
		ray->xincr *= -1;
	}
	ray->x = world->playerx + ((world->playery - ray->y) / tan_a);
	ray->yorigin = ray->y;
	ray->xorigin = ray->x;
	extend_horizontal(world, ray);
}

void	extend_horizontal(t_world *world, t_ray *ray)
{
	ray->y += ray->off_mod;
	check_ray_bounds(world, ray);
	ray->y -= ray->off_mod;
	ray->tex_offset = (int)ray->x % GRID;
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->y += ray->off_mod;
		check_ray_bounds(world, ray);
		ray->y -= ray->off_mod;
		ray->tex_offset = (int)ray->x % GRID;
	}
}

void	cast_ray(t_vars *vars)
{
	static t_caster	caster;
	double			tan_a;

	caster.raydir = HALF_FOV;
	caster.column = 0;
	vars->world.spritelst = (void *)0;
	vars->world.spritelstlast = (void *)0;
	while (caster.column < vars->img.resx)
	{
		caster.a = ray_angle(vars->world.lookdir, caster.raydir);
		get_tan_a(caster.a, &tan_a, &caster.taniszero);
		set_tex(vars, &caster);
		cast_horizontal(&(vars->world), &(caster.h), caster.a, tan_a);
		cast_vertical(&(vars->world), &(caster.v), caster.a, tan_a);
		calc_distance(&(vars->world), &caster, vars->distarr);
		draw_texture_column(&(vars->img), caster.near, caster.column,
				caster.near->tex);
		detect_sprites(&(caster.v), caster.near, &(vars->world), &(caster));
		detect_sprites(&(caster.h), caster.near, &(vars->world), &(caster));
		caster.raydir -= vars->world.radians_per_pixel;
		caster.column++;
	}
	calculate_sprites(&caster, vars);
	draw_sprites(&caster, vars);
}

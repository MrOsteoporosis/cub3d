/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/06 10:51:20 by averheij       #+#    #+#                */
/*   Updated: 2020/03/02 12:43:15 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

void	draw_texture_column(t_data *frame, t_ray *ray, int frame_column,
		t_data *tex)
{
	char	*dst;
	int		i;
	int		y;
	int		endy;
	int		tex_column;

	if (ray->height > frame->resy)
		ray->height = frame->resy;
	tex_column = (tex->resx * ray->tex_offset) >> GRIDPOW;
	y = frame->halfresy - (ray->height >> 1);
	endy = frame->halfresy + (ray->height >> 1);
	i = -1 * (frame->halfresy - (ray->real_height >> 1)) + y;
	while (y < endy)
	{
		dst = frame->addr + (y * frame->line_length +
				frame_column * (frame->bits_per_pixel >> 3));
		*(unsigned int*)dst = *(unsigned int*)(tex->addr +
				(((tex->resy * i) / ray->real_height) * tex->line_length
				+ tex_column * (tex->bits_per_pixel >> 3)));
		i++;
		y++;
	}
}

void	calc_distance(t_world *world, t_caster *caster)
{
	float	temp;
	float	dist;
	int		height;
	float	sin_a;

	sin_a = sin(caster->a);
	caster->h.dist = ft_abs((world->playery - caster->h.y) / sin_a);
	caster->v.dist = ft_abs((world->playery - caster->v.y) / sin_a);
	caster->near = &(caster->v);
	if (caster->h.dist < caster->v.dist)
		caster->near = &(caster->h);
	caster->near->dist = caster->near->dist * cos(caster->raydir);
	caster->near->height = (GRID / caster->near->dist) * world->proj_plane_dist;
	caster->near->real_height = caster->near->height;
}

void	cast_vertical(t_world *world, t_ray *ray, float a, float tan_a)
{
	ray->xincr = GRID;
	ray->yincr = GRID * tan_a;
	if (a < DEG270 && a > DEG90)
	{
		ray->x = (((int)world->playerx >> GRIDPOW) << GRIDPOW) - 1;
		ray->xincr *= -1;
	}
	else
	{
		ray->x = (((int)world->playerx >> GRIDPOW) << GRIDPOW) + GRID;
		ray->yincr *= -1;
	}
	ray->y = world->playery + ((world->playerx - ray->x) * tan_a);
	ray->gridx = ray->x / GRID;
	ray->gridy = ray->y / GRID;
	check_bounds(world, ray);
	if (!ray->foundwall)
		extendray(world, ray);
}

void	cast_horizontal(t_world *world, t_ray *ray, float a, float tan_a)
{
	ray->yincr = GRID;
	ray->xincr = GRID / tan_a;
	if (a < DEG180 && a > 0)
	{
		ray->y = (((int)world->playery >> GRIDPOW) << GRIDPOW) - 1;
		ray->yincr *= -1;
	}
	else
	{
		ray->y = (((int)world->playery >> GRIDPOW) << GRIDPOW) + GRID;
		ray->xincr *= -1;
	}
	ray->x = world->playerx + ((world->playery - ray->y) / tan_a);
	ray->gridx = ray->x / GRID;
	ray->gridy = ray->y / GRID;
	check_bounds(world, ray);
	if (!ray->foundwall)
		extendray(world, ray);
}

void	cast_ray(t_vars *vars)
{
	t_caster	caster;
	float		tan_a;

	caster.raydir = HALF_FOV;
	caster.column = 1;
	while (caster.column <= vars->img.resx)
	{
		caster.a = ray_angle(vars->world.lookdir, caster.raydir);
		tan_a = tan(caster.a);
		cast_horizontal(&(vars->world), &(caster.h), caster.a, tan_a);
		cast_vertical(&(vars->world), &(caster.v), caster.a, tan_a);
		calc_offsets(vars, &caster);
		calc_distance(&(vars->world), &caster);
		draw_texture_column(&(vars->img), caster.near, caster.column,
				caster.near->tex);
		caster.raydir -= vars->world.radians_per_pixel;
		caster.column++;
	}
}

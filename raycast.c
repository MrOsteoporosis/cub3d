/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/06 10:51:20 by averheij       #+#    #+#                */
/*   Updated: 2020/03/05 13:30:40 by averheij         ###   ########.fr       */
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
	if (frame_column == frame->resx || frame_column == 0)
		endy--;
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
	double	temp;
	double	dist;
	int		height;
	double	trig_a;

	if (!caster->taniszero)
	{
		trig_a = sin(caster->a);
		caster->h.dist = ft_abs((world->playery - caster->h.y) / trig_a);
		caster->v.dist = ft_abs((world->playery - caster->v.y) / trig_a);
	}
	else
	{
		trig_a = cos(caster->a);
		caster->h.dist = ft_abs((world->playerx - caster->h.x) / trig_a);
		caster->v.dist = ft_abs((world->playerx - caster->v.x) / trig_a);
	}
	caster->near = &(caster->h);
	if (caster->v.dist < caster->h.dist)
		caster->near = &(caster->v);
	else if (caster->v.dist == caster->h.dist)
		caster->near = caster->ftprev;
	caster->ftprev = caster->near;
	caster->near->dist = caster->near->dist * cos(caster->raydir);
	caster->near->height = (GRID / caster->near->dist) * world->proj_plane_dist;
	caster->near->real_height = caster->near->height;
}

void	cast_vertical(t_world *world, t_ray *ray, double a, double tan_a)
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
	extend_vertical(world, ray);
}

void	extend_vertical(t_world *world, t_ray *ray)
{
	ray->gridx = ray->x / GRID;
	ray->gridy = ray->y / GRID;
	check_bounds(world, ray);
	ray->tex_offset = (int)ray->y % GRID;
	if (ray->foundwall && ray->tex_offset == GRID - 1 &&
			ray->gridy + 1 < world->map_height &&
			world->map[ray->gridy + 1][ray->gridx] != '1')
		ray->foundwall = 0;
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

void	cast_horizontal(t_world *world, t_ray *ray, double a, double tan_a)
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
	extend_horizontal(world, ray);
}

void	extend_horizontal(t_world *world, t_ray *ray)
{
	ray->gridx = ray->x / GRID;
	ray->gridy = ray->y / GRID;
	check_bounds(world, ray);
	ray->tex_offset = (int)ray->x % GRID;
	if (ray->foundwall && ray->tex_offset == GRID - 1 &&
			ray->gridx + 1 < world->map_width &&
			world->map[ray->gridy][ray->gridx + 1] != '1')
		ray->foundwall = 0;
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->gridx = ray->x / GRID;
		ray->gridy = ray->y / GRID;
		check_bounds(world, ray);
		ray->tex_offset = (int)ray->x % GRID;
		if (ray->foundwall && ray->tex_offset == GRID - 1 &&
				ray->gridx + 1 < world->map_width &&
				world->map[ray->gridy][ray->gridx + 1] != '1')
			ray->foundwall = 0;
	}
}



void	cast_ray(t_vars *vars)
{
	static t_caster	caster;
	double			tan_a;

	caster.raydir = HALF_FOV;
	caster.column = 1;
	//make a sprite linked list, adding new link in check bounds, should be sorted in reverse render order automatially
	//yaayy memory management
	while (caster.column <= vars->img.resx)
	{
		caster.a = ray_angle(vars->world.lookdir, caster.raydir);
		tan_a = tan(caster.a);
		caster.taniszero = 0;
		if ((int)(tan_a * 1000000) == 0)
		{
			tan_a = 0.000001;
			caster.taniszero = 1;
		}
		set_tex(vars, &caster);
		cast_horizontal(&(vars->world), &(caster.h), caster.a, tan_a);
		cast_vertical(&(vars->world), &(caster.v), caster.a, tan_a);
		calc_distance(&(vars->world), &caster);
		draw_texture_column(&(vars->img), caster.near, caster.column,
				caster.near->tex);
		//loop over list
			//calc sprte distance
			//calc sprite offset
			//draw sprite column
			//free
		caster.raydir -= vars->world.radians_per_pixel;
		caster.column++;
	}
}

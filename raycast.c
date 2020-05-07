/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/06 10:51:20 by averheij       #+#    #+#                */
/*   Updated: 2020/03/09 12:48:48 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

void	draw_texture_column(t_data *frame, t_ray *ray, int frame_column,
		t_data *tex)
{
	char	*dst;
	unsigned int	color;
	int		i[2];
	int		end;

	if (ray->height > frame->resy)
		ray->height = frame->resy;
	i[1] = frame->halfresy - (ray->height >> 1);
	end = frame->halfresy + (ray->height >> 1);
	i[0] = -1 * (frame->halfresy - (ray->real_height >> 1)) + i[1];
	while (i[1] < end)
	{
		dst = frame->addr + (i[1] * frame->line_length +
				frame_column * (frame->bits_per_pixel >> 3));
		color = *(unsigned int*)(tex->addr +
				(((tex->resy * i[0]) / ray->real_height) * tex->line_length
				+ ray->tex_column * (tex->bits_per_pixel >> 3)));
		if ((ray->invisible_on && color != INVISIBLE) || !ray->invisible_on)
			*(unsigned int*)dst = color;
		i[0]++;
		i[1]++;
	}
}

void	calc_distance(t_world *world, t_caster *caster, double *distarr)
{
	double	temp;
	double	dist;
	int		height;

    if (!caster->taniszero)
    {
		caster->trig_a = sin(caster->a);
		caster->h.dist = ft_abs((world->playery - caster->h.y) / caster ->trig_a);
        caster->v.dist = ft_abs((world->playery - caster->v.y) / caster->trig_a);
    }
    else
    {
        caster->trig_a = cos(caster->a);
        caster->h.dist = ft_abs((world->playerx - caster->h.x) / caster->trig_a);
        caster->v.dist = ft_abs((world->playerx - caster->v.x) / caster->trig_a);
    }
    caster->near = &(caster->h);
	if (caster->v.dist < caster->h.dist)
		caster->near = &(caster->v);
	else if (caster->v.dist == caster->h.dist)
		caster->near = caster->ftprev;
	distarr[caster->column] = caster->near->dist;
	caster->ftprev = caster->near;
	calc_distance_norm(world, caster, distarr);
}

void	calc_distance_norm(t_world *world, t_caster *caster, double *distarr)
{
	caster->near->dist = caster->near->dist * cos(caster->raydir);
	caster->near->tex_column = (caster->near->tex->resx * caster->near->tex_offset) >> GRIDPOW;
	caster->near->height = (GRID / caster->near->dist) * world->proj_plane_dist;
	caster->near->real_height = caster->near->height;
	caster->near->invisible_on = 0;
}

void	cast_vertical(t_world *world, t_ray *ray, double a, double tan_a)
{
	ray->xincr = GRID;
	ray->yincr = GRID * tan_a;
	if (a < DEG270 && a > DEG90)
	{
		ray->x = (((int)world->playerx >> GRIDPOW) << GRIDPOW);;
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
	check_bounds(world, ray);
    ray->x -= ray->off_mod;
	ray->tex_offset = (int)ray->y % GRID;
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->x += ray->off_mod;
		check_bounds(world, ray);
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
		ray->y = (((int)world->playery >> GRIDPOW) << GRIDPOW);;
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
	check_bounds(world, ray);
    ray->y -= ray->off_mod;
	ray->tex_offset = (int)ray->x % GRID;
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
        ray->y += ray->off_mod;
		check_bounds(world, ray);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sprite.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/07/01 14:52:41 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <libft.h>
#include <math.h>

void	draw_sprites(t_caster *cast, t_vars *vars)
{
	t_sprite	*link;
	int			spritecol;
	int			col;
	int			c;

	while (vars->world.spritelst)
	{
		link = select_furthest(&vars->world.spritelst);
		c = (link->height - link->half_height);
		spritecol = -1 * c;
		while (spritecol < link->half_height)
		{
			col = link->center_column + spritecol;
			cast->v.height = link->height;
			cast->v.real_height = link->height;
			if ((col > 0 && col < vars->img.resx)
											&& link->dist < vars->distarr[col])
			{
				cast->v.tex_column = (vars->s.resx * (spritecol + c))
																/ link->height;
				draw_texture_column(&(vars->img), &(cast->v), col, &(vars->s));
			}
			spritecol++;
		}
	}
}

void	calculate_sprites(t_caster *caster, t_vars *vars)
{
	t_sprite	*link;

	link = vars->world.spritelst;
	caster->v.invisible_on = 1;
	while (link)
	{
		link->dx = vars->world.playerx - link->x;
		link->dy = vars->world.playery - link->y;
		link->dist = sqrt(link->dy * link->dy + link->dx * link->dx);
		link->a = asin(link->dy / link->dist);
		if (link->dx < 0 && link->dy < 0)
			link->a = M_PI2 + link->a;
		else if (link->dx > 0)
			link->a = DEG180 - link->a;
		link->plyrsprt_a = (vars->world.lookdir - link->a);
		if (link->plyrsprt_a < -1 * FOV)
			link->plyrsprt_a = M_PI2 + link->plyrsprt_a;
		else if (link->plyrsprt_a > 1 * FOV)
			link->plyrsprt_a = link->plyrsprt_a - M_PI2;
		link->center_column = (link->plyrsprt_a / vars->world.radians_per_pixel)
														+ (vars->img.resx >> 1);
		link->height = (GRID / link->dist) * vars->world.proj_plane_dist;
		link->half_height = link->height >> 1;
		link = link->lstnext;
	}
}

void	detect_sprites(t_ray *ray, t_ray *near, t_world *world)
{
	double	x;
	double	y;
	int		gridx;
	int		gridy;

	x = ray->xorigin;
	y = ray->yorigin;
	gridx = x / GRID;
	gridy = y / GRID;
	check_sprite_map(world, gridy, gridx);
	while (ft_abs(world->playerx - x) < ft_abs(world->playerx - near->x)
			&& ft_abs(world->playery - y) < ft_abs(world->playery - near->y))
	{
		x = x + ray->xincr;
		y = y + ray->yincr;
		gridx = x / GRID;
		gridy = y / GRID;
		if (y < 0 || y >= world->max_y)
			break ;
		world->map_width = ft_strlen(world->map[gridy]);
		world->max_x = world->map_width << GRIDPOW;
		if (x < 0 || x >= world->max_x)
			break ;
		check_sprite_map(world, gridy, gridx);
	}
}

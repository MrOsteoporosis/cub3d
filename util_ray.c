/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util_ray.c                                         :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/06 10:57:33 by averheij       #+#    #+#                */
/*   Updated: 2020/03/09 12:41:26 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <libft.h>
#include "cub3d.h"

#include <stdio.h>

double	ray_angle(double lookdir, double raydir)
{
	double a;

	a = lookdir + raydir;
	if (a > M_PI2)
		a -= M_PI2;
	return (a);
}

int		check_bounds(t_world *world, t_ray *ray)
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
	if (world->map[ray->gridy][ray->gridx] == '1' )
		ray->foundwall = 1;
	ray->safe = 1;
	return (1);
}

void    detect_sprites(t_ray *ray, t_ray *near, t_world *world, int col)
{
    double  x;
    double  y;
    int     gridx;
    int     gridy;

    x = ray->xorigin;
    y = ray->yorigin;
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
        if (world->map[gridy][gridx] == '2' &&
            !(world->spritemap[gridy][gridx]->queued))
        {
            if (!world->spritelst)
            {
                printf("1st %d %d ", gridy, gridx);
                world->spritelst = world->spritemap[gridy][gridx];
                world->spritelstlast = world->spritelst;
                world->spritelst->lstprev = (void *)0;
                world->spritelst->lstnext = (void *)0;
                world->spritelst->queued = 1;
            }
            else
            {
                printf("anotha %d %d ", gridy, gridx);
                world->spritemap[gridy][gridx]->lstprev = world->spritelstlast;
                world->spritemap[gridy][gridx]->lstnext = (void *)0;
                world->spritelstlast->lstnext = world->spritemap[gridy][gridx];
                world->spritelstlast = world->spritemap[gridy][gridx];
                world->spritelstlast->queued = 1;
            }
        }
    }
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

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
		t_data *tex)//TODO Rewrite this to not take ray but how else? a unique struct
{
	char	*dst;
	int		i;
	int		y;
	int		endy;

	if (ray->height > frame->resy)
		ray->height = frame->resy;
	y = frame->halfresy - (ray->height >> 1);
	endy = frame->halfresy + (ray->height >> 1);
	i = -1 * (frame->halfresy - (ray->real_height >> 1)) + y;
	while (y < endy)
	{
		dst = frame->addr + (y * frame->line_length +
				frame_column * (frame->bits_per_pixel >> 3));
		*(unsigned int*)dst = *(unsigned int*)(tex->addr +
				(((tex->resy * i) / ray->real_height) * tex->line_length
				+ ray->tex_column * (tex->bits_per_pixel >> 3)));
		i++;
		y++;
	}
}

void	calc_distance(t_world *world, t_caster *caster, double *distarr)
{
	double	temp;
	double	dist;
	int		height;

    if (!caster->taniszero)
    {
		caster->trig_a = sin(caster->a);//TODO This can be a local variable
		caster->h.dist = ft_abs((world->playery - caster->h.y) / caster->trig_a);
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
	caster->near->dist = caster->near->dist * cos(caster->raydir);
	caster->near->tex_column = (caster->near->tex->resx * caster->near->tex_offset) >> GRIDPOW;
	caster->near->height = (GRID / caster->near->dist) * world->proj_plane_dist;
	caster->near->real_height = caster->near->height;
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
    ray->x += ray->off_mod;//TODO move into while
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
    ray->y += ray->off_mod;//TODO move into while
	check_bounds(world, ray);
    ray->y -= ray->off_mod;
	ray->tex_offset = (int)ray->x % GRID;
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
        ray->y += ray->off_mod;//TODO HACKY MAKE THIS NICE
		check_bounds(world, ray);
        ray->y -= ray->off_mod;
		ray->tex_offset = (int)ray->x % GRID;
	}
}

void    clean_list(t_caster *caster, t_vars *vars)
{
    t_sprite    *link;
    int         spritecol;
    int         col;
    int         c;
	int			dx;
	int			dy;
	double		player_sprite_a;

    link = vars->world.spritelst;
	/*if (link)*/
		printf(" yeet\n");
    while (link)
    {
		dx = vars->world.playerx - link->x;
		dy = vars->world.playery - link->y;
		link->dist = sqrt(dy * dy + dx * dx);
		link->a = asin(dy / link->dist);
		/*link->a = acos((ft_abs(vars->world.playerx - link->x)) / link->dist);*/
		if (dx < 0 && dy < 0)
			link->a = M_PI2 + link->a;
		else if (dx > 0)
			link->a = DEG180 - link->a;
		player_sprite_a = (vars->world.lookdir - link->a);
		if (player_sprite_a < -1)
			player_sprite_a = M_PI2 + player_sprite_a;
		else if (player_sprite_a > 1)
			player_sprite_a = player_sprite_a - M_PI2;
		link->center_column = (player_sprite_a / vars->world.radians_per_pixel) + (vars->img.resx >> 1);
        link->height = (GRID / link->dist) * vars->world.proj_plane_dist;
        link->half_height = link->height >> 1;
        //Now sort??
        c = (link->height - link->half_height);
        spritecol = -1 * c;
        link->queued = 0;
		/*printf("d%f a%7.4f tb%7.4f, ta%7.4f cc%d\n",link->dist, link->a,(vars->world.lookdir - link->a), player_sprite_a, link->center_column);*/
        /*printf("x%d y%d sc%d hh%d h%d d%f z%f tr%7.4f ta%7.4f x%d", link->x, link->y, spritecol, link->half_height, link->height, link->dist, vars->distarr[link->center_column], link->trig_r, link->trig_a, link->x);*/
		/*if (link->center_column > vars->img.resx || link->center_column < 0)*/
			/*break ;*/
        while (spritecol < link->half_height)
        {
            col = link->center_column + spritecol;
			caster->v.height = link->height;
			caster->v.real_height = link->height;
            if ((col > 0 && col < vars->img.resx) && link->dist < vars->distarr[col])
            {
                caster->v.tex_column = (vars->s.resx * (spritecol + c)) / link->height;
                draw_texture_column(&(vars->img), &(caster->v), col, &(vars->s));
            }
            spritecol++;
        }
        link = link->lstnext;
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
		calc_distance(&(vars->world), &caster, vars->distarr);
		draw_texture_column(&(vars->img), caster.near, caster.column,
				caster.near->tex);
        detect_sprites(&(caster.v), caster.near, &(vars->world), &(caster));
        detect_sprites(&(caster.h), caster.near, &(vars->world), &(caster));
		//TODO Anotehr function to detect sprites on the edge of the screen, and in the current occupied grid
		caster.raydir -= vars->world.radians_per_pixel;
		caster.column++;
	}
    clean_list(&caster, vars);
}

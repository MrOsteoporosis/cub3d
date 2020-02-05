/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/02/03 13:34:55 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"

#include <stdio.h>

int		distanceanddraw(t_vars *vars, t_caster *caster)
{
	float	temp;
	float	dist;
	int		height;
	float	sin_a;

	sin_a = sin(caster->a);
	dist = (vars->world.playery - caster->h.y) / sin_a;
	if (dist < 0)
		dist *= -1;
	temp = (vars->world.playery - caster->v.y) / sin_a;
	if (temp < 0)
		temp *= -1;
	if (dist > temp)
		dist = temp;
	dist = dist * cos(caster->raydir);
	height = (64 / dist) * vars->world.proj_plane_dist;
	if  (height > FRAME_HEIGHT)
		height = FRAME_HEIGHT;
	my_mlx_sliver_put(&(vars->img), caster->column,
            HALF_FRAME_HEIGHT - (height >> 1), height,
            create_trgb(0, 100, 100, 100));
	return (1);
}

int		extendray(t_vars *vars, t_ray *ray)
{
	while (!ray->foundwall && ray->safe)
	{
		ray->x = ray->x + ray->xincr;
		ray->y = ray->y + ray->yincr;
		ray->gridx = ray->x / 64;
		ray->gridy = ray->y / 64;
		check_bounds(&(vars->world), ray);
	}
	return (0);
}

int     cast_vertical(t_vars *vars, t_caster *caster, float tan_a)
{
    caster->v.xincr = 64;
    caster->v.yincr = 64 * tan_a;
    if (caster->a < 4.71239 && caster->a > 1.5708)
    {
        caster->v.x = (((int)vars->world.playerx / 64) * 64) - 1;
        caster->v.xincr *= -1;
    }
    else
    {
        caster->v.x = (((int)vars->world.playerx / 64) * 64) + 64;
        caster->v.yincr *= -1;
    }
    caster->v.y = vars->world.playery + ((vars->world.playerx - caster->v.x) * tan_a);
    caster->v.gridx = caster->v.x / 64;
    caster->v.gridy = caster->v.y / 64;
    check_bounds(&(vars->world), &(caster->v));
    if(!caster->v.foundwall)
        extendray(vars, &(caster->v));
    return (0);
}

int     cast_horizontal(t_vars *vars, t_caster *caster, float tan_a)
{
    caster->h.yincr = 64;
    caster->h.xincr = 64 / tan_a;
    if (caster->a < 3.14159 && caster->a > 0)
    {
        caster->h.y = (((int)vars->world.playery / 64) * 64) - 1;
        caster->h.yincr *= -1;
    }
    else
    {
        caster->h.y = (((int)vars->world.playery / 64) * 64) + 64;
        caster->h.xincr *= -1;
    }
    caster->h.x = vars->world.playerx + ((vars->world.playery - caster->h.y) / tan_a);
    caster->h.gridx = caster->h.x / 64;
    caster->h.gridy = caster->h.y / 64;
    check_bounds(&(vars->world), &(caster->h));
    if(!caster->h.foundwall)
        extendray(vars, &(caster->h));
    return (0);
}

int     cast_ray(t_vars *vars)
{
    t_caster	caster;
	float		tan_a;

    caster.raydir = 0.5236;
	caster.column = 1;
	while (caster.column <= FRAME_WIDTH)
	{//Use shift ops, right shift by 8 to divide by 64
		caster.a = ray_angle(vars->world.lookdir, caster.raydir);
		tan_a = tan(caster.a);
        cast_horizontal(vars, &caster, tan_a);
        cast_vertical(vars, &caster, tan_a);
        distanceanddraw(vars, &caster);
		caster.raydir -= vars->world.radians_per_pixel;
		caster.column++;
	}
    return (0);
}

int     do_movement(t_vars *vars)
{
    if (vars->move.forward && vars->move.speedy > -0.5)
        vars->move.speedy -= 0.02;
    else if (vars->move.backward && vars->move.speedy < 0.5)
        vars->move.speedy += 0.02;
    if (vars->move.strafeleft && vars->move.speedx > -0.5)
        vars->move.speedx -= 0.02;
    else if (vars->move.straferight && vars->move.speedx < 0.5)
        vars->move.speedx += 0.02;
    if (!vars->move.forward && vars->move.speedy < 0)
        vars->move.speedy += 0.004;
    else if (!vars->move.backward && vars->move.speedy > 0)
        vars->move.speedy -= 0.004;
    if (!vars->move.strafeleft && vars->move.speedx < 0)
        vars->move.speedx += 0.004;
    else if (!vars->move.straferight && vars->move.speedx > 0)
        vars->move.speedx -= 0.004;
    vars->world.playerx += vars->move.speedx;
    vars->world.playery += vars->move.speedy;
    if (vars->move.lookleft)
    {
        vars->world.lookdir += 0.005;
        if (vars->world.lookdir > (M_PI * 2))
            vars->world.lookdir = 0;
    }
    else if (vars->move.lookright)
    {
        vars->world.lookdir -= 0.005;
        if (vars->world.lookdir < 0)
            vars->world.lookdir = (M_PI * 2);
    }
    return (0);
}

int		render(t_vars *vars)
{
	// if (vars->stop == 1)//DEBUGL
		// while(1);//DEBUGL
    my_mlx_clear_frame(&(vars->img), FRAME_WIDTH, FRAME_HEIGHT);
    do_movement(vars);
	cast_ray(vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	// vars->stop = 1;//DEBUGL
	return (0);
}

float	ray_angle(float lookdir, float raydir)
{
	float a;

	a = lookdir + raydir;
	if (a > M_PI * 2)
		a-= M_PI * 2;
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
	ray->foundwall = world->map[ray->gridy][ray->gridx];
	ray->safe = 1;
	return (1);
}

char	**statictodynamic(void)
{
	char	tempmap[7][7] =
		{{1,1,1,1,1,1,1},
		{1,0,1,0,0,1,1},
		{1,0,0,0,0,0,1},
		{1,0,0,'N',0,0,1},
		{1,0,0,0,0,0,1},
		{1,0,0,0,0,0,1},
		{1,1,1,1,1,1,1}};
	int		ia = 0;
	int		ib = 0;
	char	**res;


	res = ft_calloc(sizeof(tempmap[0]), 7);
	while (ia < 7)
	{
		ib = 0;
		res[ia] = ft_calloc(sizeof(char), 7);
		while (ib < 7)
		{
			res[ia][ib] = tempmap[ia][ib];
			ib++;
		}
		ia++;
	}
	return (res);
}


int		main(void)
{
	t_vars	vars;
	t_data	img;

	//Window setup
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, FRAME_WIDTH, FRAME_HEIGHT, "cub3d");
	//Basic event hooks
    mlx_hook(vars.win, 2, (1L << 0), key_press, &vars);//KeyPress
    mlx_hook(vars.win, 3, (1L << 1), key_release, &vars);//KeyRelease
    mlx_hook(vars.win, 6, (1L << 6), mouse_move, NULL);//MotionNotify
	mlx_hook(vars.win, 17, 0L, close, &vars);//DestroyNotify
	//Finish vars
	vars.img.img = mlx_new_image(vars.mlx, FRAME_WIDTH, FRAME_HEIGHT);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	vars.world.map = statictodynamic();
	vars.world.map_height = 7;
	vars.world.map_width = 7;
	vars.world.max_x = vars.world.map_width * 64;
	vars.world.max_y = vars.world.map_height * 64;
	vars.world.radians_per_pixel = (float)(FOV) / (float)(FRAME_WIDTH);
	vars.world.proj_plane_dist = (FRAME_WIDTH / 2) / tan(HALF_FOV);
	vars.world.playerx = 64 * 3 + 32;
	vars.world.playery = 64 * 3 + 32;
	vars.world.lookdir = 1.5708 * 1;
    vars.move.backward = 0;
    vars.move.forward = 0;
    vars.move.lookleft = 0;
    vars.move.lookright = 0;
    vars.move.strafeleft = 0;
    vars.move.straferight = 0;
    vars.move.speedx = 0;
    vars.move.speedy = 0;
	vars.stop = 0;
	mlx_loop_hook(vars.mlx, render, &vars);
	mlx_loop(vars.mlx);
	return (0);
}

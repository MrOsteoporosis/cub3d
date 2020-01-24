/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/01/24 14:50:04 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"

#include <stdio.h>

//Cant just alternate horizontal and vertical and assume itll result in the shortest? Maybe you can depending on the order/angle

int		distanceanddraw(t_vars *vars, t_caster *caster, t_ray *ray)
{//Calculate both ray distances and choose shorter of the two rays
	float	temp;
	float	dist;
	int		height;
	int		i;
	
	printf("\nWALL! y%d x%d ", ray->gridy, ray->gridx);//DEBUG
	printf("column %d ", caster->column);//DEBUG
	dist = (vars->world.playery - ray->y) / sin(vars->world.lookdir + caster->raydir);
	if (dist < 0)
		dist *= -1;
	dist = dist * cos(caster->raydir);
	printf("dist %f ", dist);//DEBUG
	height = (64 / dist) * vars->world.proj_plane_dist;
	printf("height %d ", height);//DEBUG
	my_mlx_sliver_put(&(vars->img), caster->column, HALF_FRAME_HEIGHT - (height / 2), height, create_trgb(0, 255, 0, 0));
	return (1);
}

int		extendray(t_vars *vars, t_caster *caster)
{//Call distance check after both rays resolved
	int i;

	i = 0;
	while (i < 2)
	{
		caster->a.x = caster->a.x + caster->a.xincr;
		caster->a.y = caster->a.y + caster->a.yincr;
		caster->a.gridx = caster->a.x / 64;
		caster->a.gridy = caster->a.y / 64;
		printf("h |%.0f,%.0f|", caster->a.x, caster->a.y);//DEBUG
		printf("%d,%d| ", caster->a.gridx, caster->a.gridy);//DEBUG
		if (check_map(&(vars->world), &(caster->a)))
			my_mlx_pixel_put(&(vars->img), caster->a.x, caster->a.y, create_trgb(0, 255, 0, 255));
			// distanceanddraw(vars, caster, &(caster->a));
		caster->b.x = caster->b.x + caster->b.xincr;
		caster->b.y = caster->b.y + caster->b.yincr;
		caster->b.gridx = caster->b.x / 64;
		caster->b.gridy = caster->b.y / 64;
		printf("v |%.0f,%.0f|", caster->b.x, caster->b.y);//DEBUG
		printf("%d,%d| ", caster->b.gridx, caster->b.gridy);//DEBUG
		if (check_map(&(vars->world), &(caster->b)))
			my_mlx_pixel_put(&(vars->img), caster->b.x, caster->b.y, create_trgb(0, 0, 255, 255));
			// distanceanddraw(vars, caster, &(caster->b));
		i++;
	}
	return (0);
}

int		render(t_vars *vars)
{//Rewrite into a single raycast function that checks horizontal or vetical depending on parameters
	t_caster	caster;
	float		temp;

	if (vars->stop == 1)//DEBUG
		while(1);//DEBUG
	caster.raydir = -0.5236;
	caster.column = 1;
	while (caster.column <= FRAME_WIDTH)
	{//Use shift ops, right shift by 8 to divide by 64
		temp = tan(vars->world.lookdir + caster.raydir);
		caster.a.yincr = 64;
		caster.a.xincr = 64 / temp;
		if (vars->world.lookdir + caster.raydir < 3.14159 && vars->world.lookdir + caster.raydir > 0)
		{
			caster.a.y = ((vars->world.playery / 64) * 64) - 1;
			caster.a.yincr *= -1;
		}
		else
		{
			caster.a.y = ((vars->world.playery / 64) * 64) + 64;
			caster.a.xincr *= -1;
		}
		caster.a.x = vars->world.playerx + ((vars->world.playery - caster.a.y) / temp);
		caster.a.gridx = caster.a.x / 64;
		caster.a.gridy = caster.a.y / 64;
		printf("\nray%7.4f| ", caster.raydir);//DEBUG
		printf("tan%8.4f| ", temp);//DEBUG
		printf("h |%.0f,%.0f|", caster.a.x, caster.a.y);//DEBUG
		printf("%d,%d| ", caster.a.gridx, caster.a.gridy);//DEBUG
		printf("y|%.0f,%.0f| ", caster.a.xincr, caster.a.yincr);//DEBUG
		if (check_map(&(vars->world), &(caster.a)))
		{
			my_mlx_pixel_put(&(vars->img), caster.a.x, caster.a.y, create_trgb(0, 255, 0, 0));//DEBUG

		}
		caster.b.xincr = 64;
		caster.b.yincr = 64 * temp; 
		if (vars->world.lookdir + caster.raydir < 4.71239 && vars->world.lookdir + caster.raydir > 1.5708)
		{
			caster.b.x = ((vars->world.playerx / 64) * 64) - 1;
			caster.b.xincr *= -1;
		}
		else
		{
			caster.b.x = ((vars->world.playerx / 64) * 64) + 64;
			caster.b.yincr *= -1;
		}
		caster.b.y = vars->world.playery + ((vars->world.playerx - caster.b.x) * temp);
		caster.b.gridx = caster.b.x / 64;
		caster.b.gridy = caster.b.y / 64;
		printf("v |%.0f,%.0f|", caster.b.x, caster.b.y);//DEBUG
		printf("%d,%d| ", caster.b.gridx, caster.b.gridy);//DEBUG
		printf("y|%.0f,%.0f| ", caster.b.xincr, caster.b.yincr);//DEBUG
		if (check_map(&(vars->world), &(caster.b)))
			my_mlx_pixel_put(&(vars->img), caster.b.x, caster.b.y, create_trgb(0, 0, 255, 0));//DEBUG
			// distanceanddraw(vars, &caster, &(caster.b));
		extendray(vars, &caster);
		caster.raydir += vars->world.radians_per_pixel;
		caster.column++;
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	vars->stop = 1;
	return (0);
}


int		check_map(t_world *world, t_ray *ray)
{
	if (ray->gridx < 0 || ray->gridx >= world->map_width)
		return (0);
	else if (ray->gridy < 0 || ray->gridy >= world->map_height)
		return (0);
	return (world->map[ray->gridy][ray->gridx]);
	// return (1);
}

/*
** because
** warning: incompatible pointer types assigning to 'char **' from 'char [5][5]'
**      vars.world.map = tempmap;
*/

char	**statictodynamic(void)
{
	char	tempmap[7][7] = 
		{{1,1,1,1,1,1,1},
		{1,1,1,1,0,0,1},
		{1,1,1,1,0,0,1},
		{1,1,1,'N',0,0,1},
		{1,1,1,1,0,0,1},
		{1,1,1,1,0,0,1},
		{1,1,1,1,1,1,1}};
	int		ia = 0;
	int		ib = 0;
	char	**res;


	res = ft_calloc(sizeof(tempmap[0]), 5);
	while (ia < 5)
	{
		ib = 0;
		res[ia] = ft_calloc(sizeof(tempmap[0][0]), 5);
		while (ib < 5)
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
	// mlx_hook(vars.win, 3, 0L, key_release, &vars);//KeyRelease
	// mlx_hook(vars.win, 6, (1L << 6), mouse_move, NULL);//MotionNotify
	mlx_hook(vars.win, 17, 0L, close, &vars);//DestroyNotify
	//Finish vars
	vars.img.img = mlx_new_image(vars.mlx, FRAME_WIDTH, FRAME_HEIGHT);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	vars.world.map = statictodynamic();
	vars.world.map_height = 7;
	vars.world.map_width = 7;
	vars.world.radians_per_pixel = (float)(FOV) / (float)(FRAME_WIDTH);
	vars.world.proj_plane_dist = (FRAME_WIDTH / 2) / tan(HALF_FOV);
	vars.world.playerx = 64 * 3 + 32;
	vars.world.playery = 64 * 3 + 32;
	vars.world.lookdir = 1.5708 * 2;
	vars.stop = 0;
	mlx_loop_hook(vars.mlx, render, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
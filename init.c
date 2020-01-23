/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/01/23 14:29:47 by averheij      ########   odam.nl         */
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
	
	printf("\nWALL! y%d x%d ", caster->gridy, caster->gridx);
	printf("column %d ", caster->column);
	dist = (vars->world.playery - ray->y) / sin(((vars->world.lookdir + caster->raydir) * M_PI) / 180.0);//change for native radians
	if (dist < 0)
		dist *= -1;
	dist = dist * cos((caster->raydir * M_PI) / 180.0);//change for native radians
	printf("dist %f ", dist);
	height = (64 / dist) * vars->world.proj_plane_dist;
	printf("height %d ", height);
	my_mlx_sliver_put(&(vars->img), caster->column, HALF_HEIGHT - (height / 2), height, create_trgb(0, 255, 0, 0));
	return (1);
}

int		extendray(t_vars *vars, t_caster *caster)
{//Call distance check after both rays resolved
	while (1)
	{
		caster->a.y = caster->a.y + caster->a.yincr;
		caster->a.x = caster->a.x + caster->a.xincr;
		caster->gridx = caster->a.y / 64;
		caster->gridy = caster->a.x / 64;
		printf("	ext |%.1f| ", caster->raydir);
		printf("ay |%.0f| ax |%.0f| ", caster->a.y, caster->a.x);
		printf("|%d||%d| ", caster->gridy, caster->gridx);
		if (vars->world.map[caster->gridy][caster->gridx] == 1)
		// {
		// 	my_mlx_pixel_put(&(vars->img), caster->a.x, caster->a.y, create_trgb(0, 255, 0, 0));
		// 	return (0);
		// }
			return(distanceanddraw(vars, caster, &(caster->a)));
		caster->b.y = caster->b.y + caster->b.yincr;
		caster->b.x = caster->b.x + caster->b.xincr;
		caster->gridx = caster->b.y / 64;
		caster->gridy = caster->b.x / 64;
		printf("by |%.0f| bx |%.0f| ", caster->b.y, caster->b.x);
		printf("|%d||%d| ", caster->gridy, caster->gridx);
		if (vars->world.map[caster->gridy][caster->gridx] == 1)
		// {
		// 	my_mlx_pixel_put(&(vars->img), caster->b.x, caster->b.y, create_trgb(0, 0, 255, 0));
		// 	return (0);
		// }
			return(distanceanddraw(vars, caster, &(caster->b)));
	}
}

int		render(t_vars *vars)
{//Rewrite into a single raycast function that checks horizontal or vetical depending on parameters
	t_caster	caster;
	float		temp;


	caster.raydir = -30;
	caster.column = 1;
	while (caster.column <= FRAME_WIDTH)
	{//Use shift ops, right shift by 8 to divide by 64
		caster.foundwall = 0;
		caster.a.yincr = 64;//SUBFUNCTION
		if (vars->world.lookdir + caster.raydir < 180 && vars->world.lookdir + caster.raydir > 0)
		{
			caster.a.y = ((vars->world.playery / 64) * 64) - 1;
			caster.a.yincr = -64;
		}
		else
			caster.a.y = ((vars->world.playery / 64) * 64) + 64;
		temp = tan(((vars->world.lookdir + caster.raydir) * M_PI) / 180.0);//change for native radians
		caster.a.x = vars->world.playerx + ((vars->world.playery - caster.a.y) / temp);
		caster.a.xincr = 64 / temp;
		caster.gridx = caster.a.y / 64;
		caster.gridy = caster.a.x / 64;
		printf("\nray |%.1f| ", caster.raydir);
		printf("ay |%.0f| ax |%.0f| ", caster.a.y, caster.a.x);
		printf("|%d||%d| ", caster.gridy, caster.gridx);
		if (vars->world.map[caster.gridy][caster.gridx] == 1)
			// my_mlx_pixel_put(&(vars->img), caster.a.x, caster.a.y, create_trgb(0, 255, 0, 0));
			caster.foundwall = distanceanddraw(vars, &caster, &(caster.a));//SUBFUNCTION END
		else 
		{
			caster.b.xincr = 64;//SUBFUNCTION
			if (vars->world.lookdir + caster.raydir < 270 && vars->world.lookdir + caster.raydir > 90)
			{
				caster.b.x = ((vars->world.playerx / 64) * 64) + 64;
				caster.b.xincr = -64;
			}
			else
				caster.b.x = ((vars->world.playerx / 64) * 64) - 1;
			caster.b.y = vars->world.playery + ((vars->world.playerx - caster.b.x) / temp);
			caster.b.yincr = caster.a.xincr; 
			caster.gridy = caster.b.y / 64;
			caster.gridx = caster.b.x / 64;
			printf("by |%.0f| bx |%.0f| ", caster.b.y, caster.b.x);
			printf("|%d||%d| ", caster.gridy, caster.gridx);
			if (vars->world.map[caster.gridy][caster.gridx] == 1)
				// my_mlx_pixel_put(&(vars->img), caster.b.x, caster.b.y, create_trgb(0, 0, 255, 0));
				caster.foundwall = distanceanddraw(vars, &caster, &(caster.b));//SUBFUNCTION END
		}
		if (!caster.foundwall)
			extendray(vars, &caster);
		caster.raydir += vars->world.pxdeg;
		caster.column++;
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

/*
** because
** warning: incompatible pointer types assigning to 'char **' from 'char [5][5]'
**      vars.world.map = tempmap;
*/

char	**statictodynamic(void)
{
	char	tempmap[5][5] = 
		{{1,1,1,1,1},
		{1,1,1,1,1},
		{1,1,0,1,1},
		{1,1,'N',1,1},
		{1,1,1,1,1}};
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
	vars.world.pxdeg = (float)(FOV) / (float)(FRAME_WIDTH);
	vars.world.proj_plane_dist = (FRAME_WIDTH / 2) / tan(((FOV / 2) * M_PI) / 180.0);
	vars.world.playerx = 128 + 32;
	vars.world.playery = 192 + 32;
	vars.world.lookdir = 90;
	mlx_loop_hook(vars.mlx, render, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
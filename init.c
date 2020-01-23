/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/01/23 12:17:06 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"

#include <stdio.h>

int		distanceanddraw(t_vars *vars)
{
	printf("WALL!\n");
	return (1);
}

int		extendray(t_vars *vars)
{
	printf("EXTEND\n");
	return (0);
}

int		render(t_vars *vars)
{//Compile vars into struct,  pass to distance and draw, calc distance and draw pixels based on height
	float		raydir;
	float		ay;
	float		ax;
	float		ayincr; // 64 down -64 up
	float		axincr; // 64 / tan(angle)
	float		by;
	float		bx;
	float		byincr; // 64 / tan(angle)
	float		bxincr; // 64 right -64 left
	int			gridy;
	int			gridx;
	float		temp;
	int			foundwall;


	raydir = -30;
	while (raydir < HALF_FOV)
	{//Use shift ops, right shift by 8 to divide by 64
		foundwall = 0;
		ayincr = 64;
		if (vars->world.lookdir + raydir < 180 && vars->world.lookdir + raydir > 0)
		{
			ay = ((vars->world.playery / 64) * 64) - 1;
			ayincr = -64;
		}
		else
			ay = ((vars->world.playery / 64) * 64) + 64;
		temp = tan(((vars->world.lookdir + raydir) * M_PI) / 180.0);
		ax = vars->world.playerx + ((vars->world.playery - ay) / temp);
		axincr = 64 / temp;
		gridy = ay / 64;
		gridx = ax / 64;
		my_mlx_pixel_put(&(vars->img), ax, ay, create_trgb(0, 255, 0, raydir + 30));
		// printf("ray |%f| ", raydir);
		// printf("ay |%f| ax |%f| ", ay, ax);
		// printf("gy |%d| gx |%d| ", gridy, gridx);
		// printf("grid |%d| ", vars->world.map[gridy][gridx]);
		if (vars->world.map[gridy][gridx] == 1)
			foundwall = distanceanddraw(vars/*, ray */);
		else 
		{
			bxincr = 64;
			if (vars->world.lookdir + raydir < 270 && vars->world.lookdir + raydir > 90)
			{
				bx = ((vars->world.playerx / 64) * 64) + 64;
				bxincr = -64;
			}
			else
				bx = ((vars->world.playerx / 64) * 64) - 1;
			by = vars->world.playery + ((vars->world.playerx - bx) / temp);
			byincr = axincr; 
			gridy = by / 64;
			gridx = bx / 64;
			my_mlx_pixel_put(&(vars->img), bx, by, create_trgb(0, 0, 255, raydir + 30));
			// printf("by |%f| bx |%f| ", by, bx);
			// printf("gy |%d| gx |%d| ", gridy, gridx);
			// printf("grid |%d| ", vars->world.map[gridy][gridx]);
			if (vars->world.map[gridy][gridx] == 1)
				foundwall = distanceanddraw(vars/*, ray */);
		}
		if (!foundwall)
			extendray(vars/*, ray */);
		raydir = raydir + vars->world.pxdeg;
	}
	//Get distance
	//Draw sliver
	//Loop
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
	vars.win = mlx_new_window(vars.mlx, 1920, 1080, "cub3d");
	//Basic event hooks
	// mlx_hook(vars.win, 3, 0L, key_release, &vars);//KeyRelease
	// mlx_hook(vars.win, 6, (1L << 6), mouse_move, NULL);//MotionNotify
	mlx_hook(vars.win, 17, 0L, close, &vars);//DestroyNotify
	//Finish vars
	vars.img.img = mlx_new_image(vars.mlx, 1920, 1080);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	vars.world.map = statictodynamic();
	vars.world.pxdeg = (float)(FOV) / (float)(FRAME_HEIGHT);
	vars.world.playerx = 128 + 32;
	vars.world.playery = 192 + 32;
	vars.world.lookdir = 90;
	mlx_loop_hook(vars.mlx, render, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
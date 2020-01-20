/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/01/20 14:52:12 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include "cub3d.h"

int		test(int a, int b, int c, int d)
{
	printf("a: %i", a);
	printf("b: %i", b);
	printf("c: %i", c);
	printf("d: %i", d);
	return (0);
}

int		render(t_vars *vars)
{
	int	x;
	int y;
	int	green;
	int	blue;
	int red;
	int random;

	y = 0;
	while (y < 1080)
	{
		x = 0;
		while (x < 1920)
		{
			random = (rand() % (20 - 0 + 1)) + 0; 
			red = ((vars->active / 255) % 2) ? ((vars->active + random) % 255) : (254 + (vars->active + random) % 255);
			green = ((x / 255) % 2) ? ((x + vars->active + random) % 255) : (254 + (x + vars->active + random) % 255);
			blue = ((y / 255) % 2) ? ((y + vars->active + random) % 255) : (254 + (y + vars->active + random) % 255);
			my_mlx_pixel_put(&vars->img, x, y, create_trgb(200, red, green, blue));
			x++;
		}
		y++;
	}
	vars->active+=2;
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

int		main(void)
{
	t_vars	vars;
	t_data	img;

	//Window setup
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 1920, 1080, "cub3d");
	//Basic event hooks
	mlx_hook(vars.win, 3, 0L, key_release, &vars);//KeyRelease
	mlx_hook(vars.win, 6, (1L << 6), mouse_move, NULL);//MotionNotify
	mlx_hook(vars.win, 17, 0L, close, &vars);//DestroyNotify
	//Create and push img
	vars.active = 0;
	vars.img.img = mlx_new_image(vars.mlx, 1920, 1080);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	mlx_loop_hook(vars.mlx, render, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/02/10 14:41:12 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"

#include <stdio.h>

int		render(t_vars *vars)
{
    my_mlx_clear_frame(&(vars->img), FRAME_WIDTH, FRAME_HEIGHT);
    do_movement(vars);
	cast_ray(vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

char	**statictodynamic(void)
{
	char	tempmap[7][7] =
		{{1,1,1,1,1,1,1},
		{1,0,0,0,1,0,1},
		{1,1,1,0,1,0,1},
		{1,0,0,0,0,0,1},
		{1,0,1,0,1,0,1},
		{1,0,0,0,0,'N',1},
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

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, FRAME_WIDTH, FRAME_HEIGHT, "cub3d");
    mlx_hook(vars.win, 2, (1L << 0), key_press, &vars);//KeyPress
    mlx_hook(vars.win, 3, (1L << 1), key_release, &vars);//KeyRelease
	mlx_hook(vars.win, 17, 0L, close, &vars);//DestroyNotifg
    /*mlx_hook(vars.win, 6, (1L << 6), mouse_move, NULL);//MotionNotify*/
	vars.img.img = mlx_new_image(vars.mlx, FRAME_WIDTH, FRAME_HEIGHT);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	vars.world.map = statictodynamic();//Add map parsing here
    vars.world.playerx = GRID * 5 + (GRID / 2);
	vars.world.playery = GRID * 5 + (GRID / 2);
	vars.world.lookdir = DEG90 * 1;
	vars.world.map_height = 7;
	vars.world.map_width = 7;
	vars.world.max_x = vars.world.map_width * GRID;
	vars.world.max_y = vars.world.map_height * GRID;
    vars.no.path = "./textures/grid32.png";
    vars.no.img.img = mlx_png_file_to_image(vars.mlx, vars.no.path, &vars.no.width, &vars.no.height);
    vars.no.img.addr = mlx_get_data_addr(vars.no.img.img, &vars.no.img.bits_per_pixel, &vars.no.img.line_length, &vars.no.img.endian);
	vars.world.radians_per_pixel = (float)(FOV) / (float)(FRAME_WIDTH);
	vars.world.proj_plane_dist = (FRAME_WIDTH / 2) / tan(HALF_FOV);
    vars.move.backward = 0;
    vars.move.forward = 0;
    vars.move.lookleft = 0;
    vars.move.lookright = 0;
    vars.move.strafeleft = 0;
    vars.move.straferight = 0;
    vars.move.speedx = 0;
    vars.move.speedy = 0;
	mlx_loop_hook(vars.mlx, render, &vars);
	mlx_loop(vars.mlx);
	return (0);
}

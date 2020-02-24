/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/02/24 11:03:06 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"

int		render(t_vars *vars)
{
	/*vars->waitframe = 1;*/
	clear_frame_color_sky_floor(&(vars->img), vars->world.colorceiling, vars->world.colorfloor);
	do_movement(&(vars->world), &(vars->move));
	cast_ray(vars);
	/*while (vars->waitframe);*/
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
	parse_cub(&vars, "map_basic.cub");
	vars.win = mlx_new_window(vars.mlx, vars.img.resx, vars.img.resy, "cub3d");
	mlx_hook(vars.win, 2, (1L << 0), key_press, &vars);//KeyPress
	mlx_hook(vars.win, 3, (1L << 1), key_release, &vars);//KeyRelease
	mlx_hook(vars.win, 17, 0L, close_window, &vars);//DestroyNotifg
	/*mlx_hook(vars.win, 6, (1L << 6), mouse_move, NULL);//MotionNotify*/
	vars.world.map = statictodynamic();//Add map parsing here
	vars.world.playerx = GRID * 5 + (GRID / 2);
	vars.world.playery = GRID * 5 + (GRID / 2);
	vars.world.lookdir = DEG90 * 1;
	vars.world.map_height = 7;
	vars.world.map_width = 7;
	vars.world.max_x = vars.world.map_width * GRID;
	vars.world.max_y = vars.world.map_height * GRID;
	vars.img.img = mlx_new_image(vars.mlx, vars.img.resx, vars.img.resy);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	vars.world.radians_per_pixel = (float)(FOV) / (float)(vars.img.resx);
	vars.world.proj_plane_dist = (vars.img.resx / 2) / tan(HALF_FOV);
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


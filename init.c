/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/03/02 12:10:40 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include <math.h>
#include <stdio.h>
#include "cub3d.h"

int		render(t_vars *vars)
{
	mlx_sync(1, vars->img.img);
	clear_frame_color_sky_floor(&(vars->img), vars->world.colorceiling, vars->world.colorfloor);
	do_movement(&(vars->world), &(vars->move));
	cast_ray(vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	mlx_sync(2, vars->win);
	return (0);
}

int		main(int argc, char **argv)
{
	t_vars	vars;

	ft_bzero((void *)(&vars), sizeof(vars));
	vars.mlx = mlx_init();
	if (argc == 1 || (argc == 2 &&
				ft_strncmp(argv[1] + (ft_strlen(argv[1]) - 4), ".cub", 4)))
	{
		perror("No map in arg");
		return (1);
	}
	if (parse_cub(&vars, argv[1]))
	{
		perror("Error");
		return (1);
	}
	vars.win = mlx_new_window(vars.mlx, vars.img.resx, vars.img.resy, "cub3d");
	mlx_hook(vars.win, 2, (1L << 0), key_press, &vars);//KeyPress
	mlx_hook(vars.win, 3, (1L << 1), key_release, &vars);//KeyRelease
	mlx_hook(vars.win, 17, 0L, close_window, &vars);//DestroyNotifg
	vars.world.playerx = GRID * 5 + (GRID / 2);//REMOVE and move to map parse
	vars.world.playery = GRID * 5 + (GRID / 2);//REMOVE and move to map parse
	vars.world.lookdir = DEG90 * 1;//REMOVE and move to map parse
	vars.img.img = mlx_new_image(vars.mlx, vars.img.resx, vars.img.resy);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	vars.world.radians_per_pixel = (float)(FOV) / (float)(vars.img.resx);
	vars.world.proj_plane_dist = (vars.img.resx / 2) / tan(HALF_FOV);
	mlx_loop_hook(vars.mlx, render, &vars);
	mlx_loop(vars.mlx);
	return (0);
}


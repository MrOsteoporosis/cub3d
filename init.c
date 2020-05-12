/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/15 15:30:43 by averheij       #+#    #+#                */
/*   Updated: 2020/05/12 11:45:21 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"

int		render(t_vars *vars)
{
	/*mlx_sync(1, vars->img.img);*/
	clear_frame_color_sky_floor(&(vars->img),
			vars->world.colorceiling, vars->world.colorfloor);
	do_movement(&(vars->world), &(vars->move));
	cast_ray(vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	/*mlx_sync(2, vars->win);*/
	return (0);
}

int		check_map_arg(int argc, char **argv, t_vars *vars)
{
	if (argc == 1 || (argc == 2 &&
			ft_strncmp(argv[1] + (ft_strlen(argv[1]) - 4), ".cub", 4)))
		print_error("No valid map path given", vars, 0, NULL);
}

void	check_save_arg(int argc, char **argv, t_vars *vars)
{
	if (argc == 3 && !ft_strncmp(argv[2], "--save", 6))
	{
		clear_frame_color_sky_floor(&vars->img,
				vars->world.colorceiling, vars->world.colorfloor);
		cast_ray(vars);
		if (write_bmp(&vars->img))
			print_error("Failed to write .bmp", vars, 0, NULL);
		close_window(vars);
	}
}

int		main(int argc, char **argv)
{
	t_vars	vars;

	ft_bzero((void *)(&vars), sizeof(vars));
	vars.mlx = mlx_init();
	check_map_arg(argc, argv, &vars);
	parse_cub(&vars, argv[1]);
	vars.img.img = mlx_new_image(vars.mlx, vars.img.resx, vars.img.resy);
	if (!vars.img.img)
		print_error("Failed to allocate image", &vars, 0, NULL);
	vars.img.addr = mlx_get_data_addr(vars.img.img,
			&vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	if (!vars.img.addr)
		print_error("Failed to get image address", &vars, 0, NULL);
	vars.world.radians_per_pixel = (float)(FOV) / (float)(vars.img.resx);
	vars.world.proj_plane_dist = (vars.img.resx / 2) / tan(HALF_FOV);
	vars.distarr = (double *)ft_calloc(sizeof(double), vars.img.resx);
	check_save_arg(argc, argv, &vars);
	vars.win = mlx_new_window(vars.mlx, vars.img.resx, vars.img.resy, "cub3d");
	mlx_hook(vars.win, 2, (1L << 0), key_press, &vars);
	mlx_hook(vars.win, 3, (1L << 1), key_release, &vars);
	mlx_hook(vars.win, 17, 0L, close_window, &vars);
	mlx_loop_hook(vars.mlx, render, &vars);
	mlx_loop(vars.mlx);
	return (0);
}

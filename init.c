/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/09/02 14:23:12 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include <math.h>
#include "cub3d.h"

int		render(t_vars *vars)
{
	/*mlx_sync(1, vars->img.img);*///UNCOMMENT
	clear_frame_color_sky_floor(&(vars->img),
			vars->world.colorceiling, vars->world.colorfloor);
	do_movement(&(vars->world), &(vars->move));
	cast_ray(vars);
	if (mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0))
		print_error("Failed to put image to window", vars, 0, (void *)0);
	/*mlx_sync(2, vars->win);*///UNCOMMENT
	return (0);
}

void	check_map_arg(int argc, char **argv, t_vars *vars)
{
	if (argc == 1 || (argc == 2 &&
			ft_strncmp(argv[1] + (ft_strlen(argv[1]) - 4), ".cub", 4)))
		print_error("No valid map path given", vars, 0, (void *)0);
	if (argc == 3 && !ft_strncmp(argv[2], "--save", 6))
		vars->save = 1;
}

void	check_save_arg(t_vars *vars)
{
	if (vars->save)
	{
		clear_frame_color_sky_floor(&vars->img,
				vars->world.colorceiling, vars->world.colorfloor);
		cast_ray(vars);
		if (write_bmp(&vars->img))
			print_error("Failed to write .bmp", vars, 0, (void *)0);
		close_window(vars);
	}
}

void	filthy_hooker(t_vars *vars)
{
	mlx_hook(vars->win, 2, (1L << 0), key_press, vars);
	mlx_hook(vars->win, 3, (1L << 1), key_release, vars);
	mlx_hook(vars->win, 17, 0L, close_window, vars);
	mlx_loop_hook(vars->mlx, render, vars);
}

int		main(int argc, char **argv)
{
	t_vars	vars;

	ft_bzero((void *)(&vars), sizeof(vars));
	vars.mlx = mlx_init();
	if (!vars.mlx)
		print_error("MLX failed to initialize", (void *)0, 0, (void *)0);
	check_map_arg(argc, argv, &vars);
	parse_cub(&vars, argv[1]);
	vars.img.img = mlx_new_image(vars.mlx, vars.img.resx, vars.img.resy);
	if (!vars.img.img)
		print_error("Failed to allocate image", &vars, 0, (void *)0);
	vars.img.addr = mlx_get_data_addr(vars.img.img,
			&vars.img.bits_per_pixel, &vars.img.line_length, &vars.img.endian);
	if (!vars.img.addr)
		print_error("Failed to get image address", &vars, 0, (void *)0);
	vars.world.radians_per_pixel = (float)(FOV) / (float)(vars.img.resx);
	vars.world.proj_plane_dist = (vars.img.resx / 2) / tan(HALF_FOV);
	vars.distarr = (double *)ft_calloc(sizeof(double), vars.img.resx);
	check_save_arg(&vars);
	vars.win = mlx_new_window(vars.mlx, vars.img.resx, vars.img.resy, "cub3d");
	if (!vars.win)
		print_error("Failed to create window", &vars, 0, (void *)0);
	filthy_hooker(&vars);
	mlx_loop(vars.mlx);
	return (0);
}

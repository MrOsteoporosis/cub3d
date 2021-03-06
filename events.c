/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   events.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/09/04 13:50:14 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <libft.h>
#include "cub3d.h"

int		close_window(t_vars *vars)
{
	if (vars)
		free_spritemap(&(vars->world));
	if (vars)
		free_map(&(vars->world));
	if (vars->win)
		mlx_destroy_window(vars->mlx, vars->win);
	if (vars->distarr)
		free(vars->distarr);
	if (vars->no.img)
		mlx_destroy_image(vars->mlx, vars->no.img);
	if (vars->so.img)
		mlx_destroy_image(vars->mlx, vars->so.img);
	if (vars->ea.img)
		mlx_destroy_image(vars->mlx, vars->ea.img);
	if (vars->we.img)
		mlx_destroy_image(vars->mlx, vars->we.img);
	if (vars->s.img)
		mlx_destroy_image(vars->mlx, vars->s.img);
	if (vars->img.img)
		mlx_destroy_image(vars->mlx, vars->img.img);
	exit(0);
}

int		key_press(int keycode, t_vars *vars)
{
	if (keycode == WKEY)
		vars->move.forward = 1;
	else if (keycode == AKEY)
		vars->move.strafeleft = 1;
	else if (keycode == SKEY)
		vars->move.backward = 1;
	else if (keycode == DKEY)
		vars->move.straferight = 1;
	else if (keycode == LAKEY)
		vars->move.lookleft = 1;
	else if (keycode == RAKEY)
		vars->move.lookright = 1;
	return (0);
}

int		key_release(int keycode, t_vars *vars)
{
	if (keycode == WKEY)
		vars->move.forward = 0;
	else if (keycode == AKEY)
		vars->move.strafeleft = 0;
	else if (keycode == SKEY)
		vars->move.backward = 0;
	else if (keycode == DKEY)
		vars->move.straferight = 0;
	else if (keycode == LAKEY)
		vars->move.lookleft = 0;
	else if (keycode == RAKEY)
		vars->move.lookright = 0;
	else if (keycode == ESC)
		close_window(vars);
	return (0);
}

void	print_error(char *err, t_vars *vars, int fd, char *line)
{
	perror(err);
	if (fd)
		close(fd);
	if (line)
		free(line);
	close_window(vars);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_basic.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 12:58:52 by averheij       #+#    #+#                */
/*   Updated: 2020/01/28 12:36:10 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include "cub3d.h"

int		close(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
}

int		key_release(int keycode, t_vars *vars)
{
	printf("keycode: %d\n", keycode);
	if (keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	else if (keycode == 13)
	{
		vars->world.playery++;
	}
	else if (keycode == 1)
	{
		vars->world.playery--;
	}
	else if (keycode == 0)
	{
		vars->world.playerx--;
	}
	else if (keycode == 2)
	{
		vars->world.playerx++;
	}
	return (0);
}

int		mouse_move(int var, int var2, int var3)
{
	printf("var: %d\n", var);
	printf("var2: %d\n", var2);
	printf("var3: %d\n", var3);
	return (0);
}

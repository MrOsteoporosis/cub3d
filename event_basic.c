/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_basic.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 12:58:52 by averheij       #+#    #+#                */
/*   Updated: 2020/02/03 12:51:44 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"

#include <stdio.h>

int		close(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
}

int		key_release(int keycode, t_vars *vars)
{
	printf("keycode: %d\n", keycode);
    if (keycode == 53)
	/*if (keycode == 65307)*/
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
    else if (keycode == 13)
    /*else if (keycode == 119)*/
    {
        vars->world.playery-= 10;
        if (vars->world.playery < 0)
            vars->world.playery = 0;
    }
    else if (keycode == 0)
	/*else if (keycode == 97)*/
	{
		vars->world.playerx-= 10;
		if (vars->world.playerx < 0)
			vars->world.playerx = 0;
	}
    else if (keycode == 1)
    /*else if (keycode == 115)*/
    {
        vars->world.playery+= 10;
        if (vars->world.playery > vars->world.max_y)
            vars->world.playery = vars->world.max_y;
    }
    else if (keycode == 2)
	/*else if (keycode == 100)*/
	{
		vars->world.playerx+= 10;
        if (vars->world.playerx > vars->world.max_x)
			vars->world.playerx = vars->world.max_x;
	}
    else if (keycode == 123)
    /*else if (keycode == 65361)*/
    {
        vars->world.lookdir+= 0.05;
        if (vars->world.lookdir > M_PI * 2)
            vars->world.lookdir-= M_PI * 2;
    }
    else if (keycode == 124)
    /*else if (keycode == 65363)*/
    {
        vars->world.lookdir-= 0.05;
        if (vars->world.lookdir < 0)
            vars->world.lookdir+= M_PI * 2;
    }
	return (0);
}

int		mouse_move(int x, int y)
{
	printf("x: %d y: %d\n", x, y);
	return (0);
}

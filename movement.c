/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averheij <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:55:59 by averheij          #+#    #+#             */
/*   Updated: 2020/02/06 11:31:33 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

int     adjust_speed(t_vars *vars, int dir)
{
	float	a;

    a = vars->world.lookdir + (dir * DEG90);
	vars->move.speedy += cos(a) * MOVESPEED;
	vars->move.speedx += sin(a) * MOVESPEED;
    return (0);
}

int     adjust_look(t_vars *vars)
{
    if (vars->move.lookleft)
    {
        vars->world.lookdir += LOOKSPEED;
        if (vars->world.lookdir > (M_PI * 2))
            vars->world.lookdir = 0;
    }
    else if (vars->move.lookright)
    {
        vars->world.lookdir -= LOOKSPEED;
        if (vars->world.lookdir < 0)
            vars->world.lookdir = (M_PI * 2);
    }
    return (0);
}

int     do_movement(t_vars *vars)
{
    if (vars->move.forward)
		adjust_speed(vars, 1);
    else if (vars->move.backward)
		adjust_speed(vars, 3);
    if (vars->move.strafeleft)
		adjust_speed(vars, 2);
    else if (vars->move.straferight)
		adjust_speed(vars, 0);
    vars->move.speedx -= vars->move.speedx * FRICTION;
    vars->move.speedy -= vars->move.speedy * FRICTION;
    vars->world.playerx += vars->move.speedx;
    vars->world.playery += vars->move.speedy;
    adjust_look(vars);
    return (0);
}



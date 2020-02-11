/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averheij <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:55:59 by averheij          #+#    #+#             */
/*   Updated: 2020/02/11 11:53:01 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

void	adjust_speed(float lookdir, float movedir, t_movement *move)
{
	float	a;

	a = lookdir + movedir;
	move->speedy += cos(a) * MOVESPEED;
	move->speedx += sin(a) * MOVESPEED;
}

void	adjust_look(float *lookdir, t_movement *move)
{
	if (move->lookleft)
	{
		*lookdir += LOOKSPEED;
		if (*lookdir > (M_PI2))
			*lookdir = LOOKSPEED;
	}
	else if (move->lookright)
	{
		*lookdir -= LOOKSPEED;
		if (*lookdir < 0)
			*lookdir = (M_PI2) - LOOKSPEED;
	}
}

void	do_movement(t_world *world, t_movement *move)
{
	if (move->forward)
		adjust_speed(world->lookdir, DEG90, move);
	else if (move->backward)
		adjust_speed(world->lookdir, DEG270, move);
	if (move->strafeleft)
		adjust_speed(world->lookdir, DEG180, move);
	else if (move->straferight)
		adjust_speed(world->lookdir, 0, move);
	move->speedx -= move->speedx * FRICTION;
	move->speedy -= move->speedy * FRICTION;
	world->playerx += move->speedx;
	world->playery += move->speedy;
	adjust_look(&(world->lookdir), move);
}


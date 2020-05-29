/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   movement.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/05/29 13:44:43 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <libft.h>
#include "cub3d.h"

void	adjust_speed(double lookdir, double movedir, t_movement *move)
{
	double	a;

	a = lookdir + movedir;
	move->speedy += cos(a) * MOVESPEED;
	move->speedx += sin(a) * MOVESPEED;
}

void	adjust_look(double *lookdir, t_movement *move)
{
	if (move->lookleft && move->lookvel < LOOKSPEED)
		move->lookvel += LOOKACCEL;
	else if (move->lookright && move->lookvel > (LOOKSPEED * -1))
		move->lookvel -= LOOKACCEL;
	else
		move->lookvel += move->lookvel * -0.5;
	*lookdir += move->lookvel;
	if (*lookdir >= M_PI2)
		*lookdir = 0;
	else if (*lookdir <= 0)
		*lookdir = M_PI2;
}

int		check_collision(t_world *world, t_movement *move, int xy)
{
	int		gridx;
	int		gridy;

	if (xy == 0)
	{
		gridx = (world->playerx + move->speedx) / GRID;
		gridy = world->playery / GRID;
	}
	else if (xy == 1)
	{
		gridx = world->playerx / GRID;
		gridy = (world->playery + move->speedy) / GRID;
	}
	if (world->playery < 0 || world->playery >= world->max_y)
		return (1);
	world->map_width = ft_strlen(world->map[gridy]);
	world->max_x = world->map_width << GRIDPOW;
	if (world->playerx < 0 || world->playerx >= world->max_x)
		return (1);
	if (world->map[gridy][gridx] == '1')
		return (1);
	return (0);
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
	if (check_collision(world, move, 0))
		move->speedx *= COLELASTICITY;
	if (check_collision(world, move, 1))
		move->speedy *= COLELASTICITY;
	world->playerx += move->speedx;
	world->playery += move->speedy;
	adjust_look(&(world->lookdir), move);
}

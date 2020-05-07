/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <marvin@42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/06 10:51:20 by averheij       #+#    #+#                */
/*   Updated: 2020/03/09 12:48:48 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** *///REPLACE

#include "cub3d.h"

t_sprite	*select_furthest(t_sprite **lst)
{
	t_sprite	*furthest;
	t_sprite	*link;

	link = *lst;
	furthest = link;
	while (link)
	{
		if (furthest->dist < link->dist)
			furthest = link;
		link = link->lstnext;
	}
	if (furthest->lstprev)
		furthest->lstprev->lstnext = furthest->lstnext;
	else
		*lst = furthest->lstnext;
	if (furthest->lstnext)
		furthest->lstnext->lstprev = furthest->lstprev;
	furthest->queued = 0;
	return (furthest);
}

void	queue_sprite(t_world *world, int gridy, int gridx)
{
	if (!world->spritelst)
	{
		world->spritelst = world->spritemap[gridy][gridx];
		world->spritelstlast = world->spritelst;
		world->spritelst->lstprev = (void *)0;
		world->spritelst->lstnext = (void *)0;
		world->spritelst->queued = 1;
	}
	else
	{
		world->spritemap[gridy][gridx]->lstprev = world->spritelstlast;
		world->spritemap[gridy][gridx]->lstnext = (void *)0;
		world->spritemap[gridy][gridx]->queued = 1;
		world->spritelstlast->lstnext = world->spritemap[gridy][gridx];
		world->spritelstlast = world->spritemap[gridy][gridx];
	}
}

void	check_sprite_map(t_world *world, int gridy, int gridx)
{
	if (ismap(gridy, gridx, world)
			&& world->map[gridy][gridx] == 'I'
			&& !(world->spritemap[gridy][gridx]->queued))
		queue_sprite(world, gridy, gridx);
	if (ismap(gridy - 1, gridx, world)
			&& world->map[gridy - 1][gridx] == 'I'
			&& !(world->spritemap[gridy - 1][gridx]->queued))
		queue_sprite(world, gridy - 1, gridx);
	if (ismap(gridy, gridx - 1, world)
			&& world->map[gridy][gridx - 1] == 'I'
			&& !(world->spritemap[gridy][gridx - 1]->queued))
		queue_sprite(world, gridy, gridx - 1);
}

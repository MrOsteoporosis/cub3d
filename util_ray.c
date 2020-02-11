/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averheij <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 10:57:33 by averheij          #+#    #+#             */
/*   Updated: 2020/02/11 09:26:43 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <libft.h>
#include "cub3d.h"

float	ray_angle(float lookdir, float raydir)
{
	float a;

	a = lookdir + raydir;
	if (a > M_PI2)
		a-= M_PI2;
	return (a);
}

int		check_bounds(t_world *world, t_ray *ray)
{
	ray->foundwall = 0;
	ray->safe = 0;
	if (ray->x < 0 || ray->x >= world->max_x)
		return (0);
	if (ray->y < 0 || ray->y >= world->max_y)
		return (0);
    if (world->map[ray->gridy][ray->gridx] == 1)
    	ray->foundwall = 1;
	ray->safe = 1;
	return (1);
}

int     ft_abs(int x)
{
    int y;

    y = (x >> 31);
    return ((x ^ y) - y);
}

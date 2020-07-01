/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_sky_dist_draw.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/07/01 14:50:25 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cub3d.h"

void	draw_texture_column(t_data *frame, t_ray *ray, int frame_column,
		t_data *tex)
{
	char			*dst;
	unsigned int	color;
	int				i[2];
	int				end;

	if (ray->height > frame->resy)
		ray->height = frame->resy;
	i[1] = frame->halfresy - (ray->height >> 1);
	end = frame->halfresy + (ray->height >> 1);
	i[0] = -1 * (frame->halfresy - (ray->real_height >> 1)) + i[1];
	while (i[1] < end)
	{
		dst = frame->addr + (i[1] * frame->line_length +
				frame_column * (frame->bits_per_pixel >> 3));
		color = *(unsigned int*)(tex->addr +
				(((tex->resy * i[0]) / ray->real_height) * tex->line_length
				+ ray->tex_column * (tex->bits_per_pixel >> 3)));
		if ((ray->invisible_on && color != INVISIBLE) || !ray->invisible_on)
			*(unsigned int*)dst = color;
		i[0]++;
		i[1]++;
	}
}

void	calc_distance(t_world *world, t_caster *cast, double *distarr)
{
	if (!cast->taniszero)
	{
		cast->trig_a = sin(cast->a);
		cast->h.dist = ft_abs((world->playery - cast->h.y) / cast->trig_a);
		cast->v.dist = ft_abs((world->playery - cast->v.y) / cast->trig_a);
	}
	else
	{
		cast->trig_a = cos(cast->a);
		cast->h.dist = ft_abs((world->playerx - cast->h.x) / cast->trig_a);
		cast->v.dist = ft_abs((world->playerx - cast->v.x) / cast->trig_a);
	}
	cast->near = &(cast->h);
	if (cast->v.dist < cast->h.dist)
		cast->near = &(cast->v);
	else if (cast->v.dist == cast->h.dist)
		cast->near = cast->ftprev;
	distarr[cast->column] = cast->near->dist;
	cast->ftprev = cast->near;
	calc_distance_norm(world, cast);
}

void	calc_distance_norm(t_world *world, t_caster *caster)
{
	caster->near->dist = caster->near->dist * cos(caster->raydir);
	caster->near->tex_column = (caster->near->tex->resx *
										caster->near->tex_offset) >> GRIDPOW;
	caster->near->height = (GRID / caster->near->dist) * world->proj_plane_dist;
	caster->near->real_height = caster->near->height;
	caster->near->invisible_on = 0;
}

void	clear_frame_color_sky_floor(t_data *data, int sky, int ftfloor)
{
	int		x;
	int		y;
	char	*dst;

	x = 0;
	while (x < data->resx)
	{
		y = 0;
		while (y < data->halfresy)
		{
			dst = data->addr + (y * data->line_length +
					x * (data->bits_per_pixel >> 3));
			*(unsigned int*)dst = sky;
			y++;
		}
		while (y < data->resy)
		{
			dst = data->addr + (y * data->line_length +
					x * (data->bits_per_pixel >> 3));
			*(unsigned int*)dst = ftfloor;
			y++;
		}
		x++;
	}
}

int		create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

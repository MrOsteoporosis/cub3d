/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util_pixel.c                                       :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 12:58:16 by averheij       #+#    #+#                */
/*   Updated: 2020/02/24 10:37:48 by averheij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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


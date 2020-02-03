/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util_pixel.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/20 12:58:16 by averheij       #+#    #+#                */
/*   Updated: 2020/02/03 12:54:53 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	my_mlx_sliver_put(t_data *data, int x, int y, int height, int color)
{
	char	*dst;
	int		i;

	i = 0;
	while (i < height)
	{
		dst = data->addr + ((y + i) * data->line_length + x * (data->bits_per_pixel / 8));
		*(unsigned int*)dst = color;
		i++;
	}
}

void	my_mlx_clear_frame(t_data *data, int width, int height)
{
	char	*dst;
	int		i;

	i = height * data->line_length + width * (data->bits_per_pixel / 8);
	dst = data->addr + i;
	while (dst <= data->addr)
	{
		*(unsigned int*)dst = create_trgb(0, 255, 0, 0);
		i--;
		dst--;
	}
}

int		create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

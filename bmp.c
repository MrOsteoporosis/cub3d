/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bmp.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 13:42:34 by averheij      #+#    #+#                 */
/*   Updated: 2020/09/02 15:42:27 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "cub3d.h"

int		write_bitmaprgb(t_data *data, int fd)
{
	int			y;
	int			x;
	const void	*linestart;
	char		zero;

	zero = 0;
	y = data->resy;
	while (y > 0)
	{
		y--;
		linestart = data->addr + (data->line_length * y);
		x = 0;
		while (x < data->resx)
		{
			if (write(fd, linestart + (x * data->bits_per_pixel >> 3),
						data->bits_per_pixel >> 3) == -1)
				return (1);
			if (write(fd, &zero, 1) == -1)
				return (1);
			x++;
		}
	}
	return (0);
}

int		write_bitmapinfoheader(t_data *data, int fd)
{
	static int	header[11] = {40, -1, -1, 1, -1, 0, 0, 0, 0, 0, 0};
	static int	size[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
	int			i;

	header[1] = data->resx;
	header[2] = data->resy;
	header[4] = data->bits_per_pixel + 8;
	/*header[4] = data->bits_per_pixel;*/
	header[9] = 1 << data->bits_per_pixel;
	i = 0;
	while (i < 11)
	{
		if (write(fd, &(header[i]), size[i]) == -1)
			return (1);
		i++;
	}
	return (0);
}

int		write_bitmapfileheader(t_data *data, int fd)
{
	static int	header[5] = {0x4D42, -1, 0, 0, 54};
	static int	size[5] = {2, 4, 2, 2, 4};
	int			i;

	header[1] = (data->resx * data->resy * ((data->bits_per_pixel >> 3) + 1))
	/*header[1] = (data->resx * data->resy * ((data->bits_per_pixel >> 3)))*/
					+ 54;
	i = 0;
	while (i < 5)
	{
		if (write(fd, &(header[i]), size[i]) == -1)
			return (1);
		i++;
	}
	return (0);
}

int		write_bmp(t_data *data)
{
	int		fd;

	fd = open("./save.bmp", O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		return (1);
	if (write_bitmapfileheader(data, fd))
		return (1);
	if (write_bitmapinfoheader(data, fd))
		return (1);
	if (write_bitmaprgb(data, fd))
		return (1);
	close(fd);
	return (0);
}

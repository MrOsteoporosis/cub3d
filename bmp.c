/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bmp.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/11 17:59:32 by averheij      #+#   #+#                  */
/*   Updated: 2020/05/11 20:47:29 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "cub3d.h"

int		write_bitmaprgb(t_data *data, int fd)
{
	int			y;
	int			x;
	const void	*linestart;

	y = data->resy;
	while (y > 0)
	{
		y--;
		linestart = data->addr + (data->line_length * y);
		if (write(fd, linestart, data->resx *
											(data->bits_per_pixel >> 3)) == -1)
			return (1);
	}
	return (0);
}

int		write_bitmapinfoheader(t_data *data, int fd)
{
	static int	header[11] = {40, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0};
	static int	size[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
	int			i;

	header[1] = data->resx;
	header[2] = data->resy;
	header[4] = data->bits_per_pixel;
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
	static int	header[5] = {19778, -1, 0, 0, 54};
	static int	size[5] = {2, 4, 2, 2, 4};
	int			i;

	header[1] = (data->resx * data->resy * (data->bits_per_pixel >> 3)) + 55;
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
	int		ret;

	fd = open("./save.bmp", O_CREAT | O_WRONLY);
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

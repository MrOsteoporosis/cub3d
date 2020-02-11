/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 15:02:14 by averheij       #+#    #+#                */
/*   Updated: 2019/11/04 09:58:20 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_recur(int n, int fd)
{
	if (n / 10)
		ft_recur(n / 10, fd);
	if (n < 0)
		ft_putchar_fd('0' - n % 10, fd);
	else
		ft_putchar_fd('0' + n % 10, fd);
}

void		ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
		ft_putchar_fd('-', fd);
	ft_recur(n, fd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 12:54:03 by averheij       #+#    #+#                */
/*   Updated: 2019/12/04 12:22:37 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ub;
	size_t			i;

	ub = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		ub[i] = c;
		i++;
	}
	return (b);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 16:10:19 by averheij       #+#    #+#                */
/*   Updated: 2019/12/04 12:26:40 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*udst;
	const unsigned char	*usrc;
	size_t				i;

	if (!dst && !src)
		return (dst);
	udst = (unsigned char *)dst;
	usrc = (const unsigned char *)src;
	if (dst > src)
	{
		i = len;
		while (i > 0)
		{
			udst[i - 1] = usrc[i - 1];
			i--;
		}
	}
	else
		ft_memcpy(dst, src, len);
	return (dst);
}

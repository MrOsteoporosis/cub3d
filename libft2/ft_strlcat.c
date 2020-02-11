/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 12:47:59 by averheij       #+#    #+#                */
/*   Updated: 2019/12/04 12:49:31 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t		s;
	size_t		d;

	d = 0;
	s = 0;
	while (d < dstsize && dst[d])
		d++;
	while (d < dstsize && s + 1 < dstsize - d && src[s])
	{
		dst[d + s] = src[s];
		s++;
	}
	if (d < dstsize)
		dst[d + s] = '\0';
	while (src[s])
		s++;
	if (d == dstsize)
		return (s + dstsize);
	return (s + d);
}

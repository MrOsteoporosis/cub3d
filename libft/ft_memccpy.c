/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memccpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/28 15:38:29 by averheij       #+#    #+#                */
/*   Updated: 2019/12/05 11:12:40 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char			*udst;
	const unsigned char		*usrc;
	unsigned char			uc;
	size_t					i;

	udst = (unsigned char *)dst;
	usrc = (const unsigned char *)src;
	uc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		udst[i] = usrc[i];
		if (usrc[i] == uc)
			return (&udst[i + 1]);
		i++;
	}
	return (0);
}

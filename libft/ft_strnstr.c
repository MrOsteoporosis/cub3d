/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 13:25:15 by averheij       #+#    #+#                */
/*   Updated: 2019/12/05 11:20:04 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		h;
	size_t		n;

	if (!*needle)
		return ((char *)haystack);
	h = 0;
	while (h < len && haystack[h])
	{
		if (haystack[h] == needle[0])
		{
			n = 0;
			while (h + n < len && haystack[h + n]
				&& needle[n] && haystack[h + n] == needle[n])
				n++;
			if (!needle[n])
				return ((char *)&haystack[h]);
		}
		h++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strncmp.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 13:53:58 by averheij       #+#    #+#                */
/*   Updated: 2019/12/04 13:10:46 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	const unsigned char	*us1;
	const unsigned char	*us2;
	size_t				i;

	us1 = (const unsigned char *)s1;
	us2 = (const unsigned char *)s2;
	if (!n)
		return (0);
	i = 0;
	while (i < n - 1 && us1[i] && us2[i] && us1[i] == us2[i])
		i++;
	return (us1[i] - us2[i]);
}

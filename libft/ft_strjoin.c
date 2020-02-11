/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 10:29:29 by averheij       #+#    #+#                */
/*   Updated: 2019/12/04 14:29:21 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	l1;
	size_t	l2;
	size_t	i;

	if (!s1 || !s2)
		return (0);
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	res = (char *)ft_calloc(sizeof(char), l1 + l2 + 1);
	if (!res)
		return (0);
	res[l1 + l2] = '\0';
	i = 0;
	while (i < l1 || i < l2)
	{
		if (i < l1)
			res[i] = s1[i];
		if (i < l2)
			res[i + l1] = s2[i];
		i++;
	}
	return (res);
}

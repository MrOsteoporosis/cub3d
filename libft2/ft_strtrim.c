/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtrim.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 10:45:14 by averheij       #+#    #+#                */
/*   Updated: 2019/12/04 14:32:16 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_skipmatch(char const *s1, char const *set, int incr, int i)
{
	int		done;
	int		j;

	done = 1;
	if (*set)
		done = 0;
	while (!done)
	{
		j = 0;
		done = 1;
		while (set[j])
		{
			while (i >= 0 && s1[i] && s1[i] == set[j])
			{
				i += incr;
				done = 0;
			}
			j++;
		}
	}
	return (i);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		pos1;
	char	*res;

	if (!s1)
		return (0);
	if (!set)
		return (ft_strdup(""));
	pos1 = ft_skipmatch(s1, set, 1, 0);
	i = 0;
	while (s1[i])
		i++;
	i--;
	i = ft_skipmatch(s1, set, -1, i);
	i++;
	if (i <= pos1)
		return (ft_strdup(""));
	res = (char *)ft_calloc(sizeof(char), i - pos1 + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, &s1[pos1], i - pos1 + 1);
	return (res);
}

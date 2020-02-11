/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/09/19 12:48:50 by averheij       #+#    #+#                */
/*   Updated: 2019/12/04 14:34:17 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int		fallingwithstyle(char **res, int pos)
{
	while (pos >= 0)
	{
		if (res[pos])
			free(res[pos]);
		pos--;
	}
	free(res);
	return (-1);
}

static int		ft_wordlength(char c, char *str)
{
	int i;

	i = 0;
	while (str[i] && !(str[i] == c))
		i++;
	return (i);
}

static int		ft_wc(char *c, char *str)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i + 1] && ((str[i] == *c && str[i + 1] != *c)
			|| (i == 0 && str[i] != *c)))
			count++;
		i++;
	}
	return (count);
}

static int		ft_wordcopy(char *c, char **res, char *str, int pos)
{
	int		fe;
	char	*ptr;
	int		i;

	if (*str != *c)
	{
		fe = ft_wordlength(*c, str);
		res[pos] = (char *)malloc(sizeof(char) * (fe + 1));
		if (!res[pos])
			return (fallingwithstyle(res, pos));
		ptr = res[pos];
		i = 0;
		while (i < fe)
		{
			ptr[i] = str[i];
			i++;
		}
		ptr[i] = '\0';
		pos++;
	}
	return (pos);
}

char			**ft_split(const char *s, char c)
{
	int		i;
	int		pos;
	char	**res;

	if (!s)
		return (NULL);
	pos = 0;
	i = 0;
	res = (char **)malloc(sizeof(char *) * (ft_wc(&c, (char *)s) + 1));
	if (!res)
		return (NULL);
	while (s[i])
	{
		if (s[i + 1] && ((s[i] == c && s[i + 1] != c) || (i == 0 && s[i] != c)))
		{
			if (s[i] == c)
				i++;
			pos = ft_wordcopy(&c, res, &((char *)s)[i], pos);
			if (pos == -1)
				return (NULL);
		}
		i++;
	}
	res[pos] = 0;
	return (res);
}

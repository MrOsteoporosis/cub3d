/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 11:52:43 by averheij       #+#    #+#                */
/*   Updated: 2019/12/05 11:09:21 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_fitoa(char *str, int size, int temp, int nb)
{
	while (size >= ((nb < 0) ? 1 : 0))
	{
		str[size] = ((temp < 0) ? '0' - temp % 10 : '0' + temp % 10);
		temp = temp / 10;
		size--;
	}
	str[0] = (nb < 0) ? '-' : str[0];
	return (str);
}

char		*ft_itoa(int nb)
{
	char	*str;
	int		size;
	int		temp;

	temp = nb;
	size = (nb == 0) ? 1 : 0;
	while (temp)
	{
		temp = temp / 10;
		size++;
	}
	size += (nb < 0) ? 1 : 0;
	str = (char *)ft_calloc(sizeof(char), size + 1);
	if (!str)
		return (NULL);
	str[size] = '\0';
	size--;
	temp = nb;
	return (ft_fitoa(str, size, temp, nb));
}

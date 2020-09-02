/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 14:26:33 by averheij      #+#    #+#                 */
/*   Updated: 2020/09/02 14:27:58 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int					ft_atoi(const char *str)
{
	int					sign;
	unsigned long int	res;

	sign = 1;
	res = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
		if (res > 2147483647UL && sign == 1)
			return (-1);
		else if (res > 2147483648UL && sign == -1)
			return (0);
	}
	return (sign * (int)res);
}

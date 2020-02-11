/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_calloc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 15:32:45 by averheij       #+#    #+#                */
/*   Updated: 2019/12/04 14:23:36 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size)
{
	void		*block;

	block = (void *)malloc(count * size);
	if (!block)
		return (NULL);
	ft_bzero(block, count * size);
	return (block);
}

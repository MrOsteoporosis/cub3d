/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstnew.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 15:33:50 by averheij       #+#    #+#                */
/*   Updated: 2019/11/01 14:47:25 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdlib.h"

t_list	*ft_lstnew(void *content)
{
	t_list *ele;

	ele = malloc(sizeof(t_list));
	if (ele)
	{
		ele->content = content;
		ele->next = 0;
	}
	return (ele);
}

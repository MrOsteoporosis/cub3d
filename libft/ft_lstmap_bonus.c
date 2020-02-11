/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap_bonus.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: averheij <averheij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/04 12:10:28 by averheij       #+#    #+#                */
/*   Updated: 2019/11/06 14:34:41 by averheij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	*fallingwithstyle(t_list *lst, void (*del)(void *))
{
	ft_lstclear(&lst, del);
	return (NULL);
}

t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*nlst;
	t_list	*ele;

	if (!lst)
		return (NULL);
	ele = ft_lstnew(f(lst->content));
	if (!ele)
		return (fallingwithstyle(ele, del));
	nlst = ele;
	lst = lst->next;
	while (lst)
	{
		ele->next = ft_lstnew(f(lst->content));
		if (!ele->next)
			return (fallingwithstyle(nlst, del));
		lst = lst->next;
		ele = ele->next;
	}
	return (nlst);
}

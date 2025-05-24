/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 00:41:35 by kizuna            #+#    #+#             */
/*   Updated: 2025/01/25 16:51:22 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;
	size_t	s1_len;
	size_t	start;
	size_t	end;

	if (!s1)
		return (0);
	s1_len = ft_strlen(s1);
	start = 0;
	while (start < s1_len && ft_strchr(set, s1[start]))
		start++;
	end = s1_len;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	ptr = (char *)malloc(end - start + 1);
	if (!ptr)
		return (0);
	ft_memcpy(ptr, s1 + start, end - start);
	ptr[end - start] = 0;
	return (ptr);
}

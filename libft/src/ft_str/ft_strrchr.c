/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:07:58 by kishino           #+#    #+#             */
/*   Updated: 2025/01/25 16:51:26 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*last;

	last = NULL;
	while (*s)
	{
		if (*s == (char)c)
			last = s;
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return ((char *)last);
}

// #include <string.h>

// #define SIZE 40

// int main(void)
// {
//   const char buf[SIZE] = "computer program";
//   char * ptr;
//   int    ch = 'p';

//   /* This illustrates strchr */
//   ptr = strchr( buf, ch );
//   printf( "The first occurrence of %c in '%s' is '%s'\n", ch, buf, ptr );

//   /* This illustrates strrchr */
//   ptr = ft_strrchr( buf, ch );
//   printf( "The last occurrence of %c in '%s' is '%s'\n", ch, buf, ptr );
// }

/*****************  Output should be similar to:  *****************

The first occurrence of p in 'computer program' is 'puter program'
The last occurrence of p in 'computer program' is 'program'
*/

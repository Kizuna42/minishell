/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 10:50:55 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/04 22:34:35 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

char	*get_next_line(int fd);
char	*gnl_strchr(char *s, int c);
char	*gnl_strjoin(char *s1, char *s2);
int		gnl_strlen(char *s);
void	ft_copy_line(char *save, char *str);
void	ft_copy_remainder(char *save, char *str, int i);
char	*ft_read_to_save(int fd, char *save);
int		gnl(int fd, char **str);

#endif

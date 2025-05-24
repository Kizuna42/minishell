# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/25 16:01:19 by kizuna            #+#    #+#              #
#    Updated: 2025/05/24 17:50:05 by kizuna           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror -I.

SRCS_DIR = src/
FT_IS_DIR = $(SRCS_DIR)ft_is/
FT_LST_DIR = $(SRCS_DIR)ft_lst/
FT_MEM_DIR = $(SRCS_DIR)ft_mem/
FT_PUT_DIR = $(SRCS_DIR)ft_put/
FT_STR_DIR = $(SRCS_DIR)ft_str/
FT_TO_DIR = $(SRCS_DIR)ft_to/
GNL_DIR = $(SRCS_DIR)gnl/
SRCS = $(addprefix $(SRCS_DIR), $(addprefix ft_is/, ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c)) \
		$(addprefix $(SRCS_DIR), $(addprefix ft_to/, ft_atoi.c ft_itoa.c ft_tolower.c ft_toupper.c)) \
		$(addprefix $(SRCS_DIR), $(addprefix ft_mem/, ft_bzero.c ft_calloc.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c)) \
		$(addprefix $(SRCS_DIR), $(addprefix ft_put/, ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c)) \
		$(addprefix $(SRCS_DIR), $(addprefix ft_str/, ft_split.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_striteri.c)) \
		$(addprefix $(SRCS_DIR), $(addprefix gnl/, get_next_line.c get_next_line_utils.c get_next_line_utils2.c))


BONUS = $(FT_LST_DIR)ft_lstnew_bonus.c \
		$(FT_LST_DIR)ft_lstadd_front_bonus.c \
		$(FT_LST_DIR)ft_lstsize_bonus.c \
		$(FT_LST_DIR)ft_lstlast_bonus.c \
		$(FT_LST_DIR)ft_lstadd_back_bonus.c \
		$(FT_LST_DIR)ft_lstdelone_bonus.c \
		$(FT_LST_DIR)ft_lstclear_bonus.c \
		$(FT_LST_DIR)ft_lstiter_bonus.c \
		$(FT_LST_DIR)ft_lstmap_bonus.c

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)
	ranlib $(NAME)

bonus: $(OBJS) $(BONUS_OBJS)
	ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)
	ranlib $(NAME)

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus

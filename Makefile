# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#              #
#    Updated: 2025/05/24 18:59:28 by kizuna           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes -I./libft
LDFLAGS = -lreadline

# Directories
SRCDIR = src
INCDIR = includes
OBJDIR = obj
LIBFTDIR = libft

# Source files
SRCS = $(SRCDIR)/main.c \
		$(SRCDIR)/lexer/lexer.c \
		$(SRCDIR)/lexer/lexer_utils.c \
		$(SRCDIR)/lexer/tokenizer.c \
		$(SRCDIR)/parser/parser.c \
		$(SRCDIR)/parser/parser_utils.c \
		$(SRCDIR)/executor/executor.c \
		$(SRCDIR)/executor/builtin.c \
		$(SRCDIR)/executor/builtin_commands.c \
		$(SRCDIR)/executor/builtin_extra.c \
		$(SRCDIR)/executor/redirections.c \
		$(SRCDIR)/executor/pipes.c \
		$(SRCDIR)/utils/utils.c \
		$(SRCDIR)/utils/env_utils.c \
		$(SRCDIR)/utils/signal_utils.c \
		$(SRCDIR)/utils/cleanup.c \
		$(SRCDIR)/utils/expand.c

# Bonus source files
BONUS_SRCS = $(SRCDIR)/bonus/logical_ops.c \
			$(SRCDIR)/bonus/wildcards.c

# Object files
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BONUS_OBJS = $(BONUS_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Libraries
LIBFT = $(LIBFTDIR)/libft.a

# Colors
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

$(LIBFT):
	@echo "$(GREEN)Building libft...$(RESET)"
	@$(MAKE) -C $(LIBFTDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

bonus: $(LIBFT) $(OBJS) $(BONUS_OBJS)
	@echo "$(GREEN)Linking $(NAME) with bonus...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(BONUS_OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) with bonus created successfully!$(RESET)"

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)
	@$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	@echo "$(RED)Cleaning $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus

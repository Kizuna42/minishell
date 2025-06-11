# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#              #
#    Updated: 2025/06/11 17:55:11 by kizuna           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
BONUS_NAME = minishell_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes -I./libft
BONUS_CFLAGS = -Wall -Wextra -Werror -I./includes -I./libft -DBONUS
LDFLAGS = -lreadline

# Directories
SRCDIR = src
INCDIR = includes
OBJDIR = obj
BONUS_OBJDIR = obj_bonus
LIBFTDIR = libft

# Source files
SRCS = $(SRCDIR)/main.c \
		$(SRCDIR)/main_utils.c \
		$(SRCDIR)/lexer/lexer.c \
		$(SRCDIR)/lexer/lexer_utils.c \
		$(SRCDIR)/lexer/lexer_quote.c \
		$(SRCDIR)/lexer/tokenizer.c \
		$(SRCDIR)/parser/parser.c \
		$(SRCDIR)/parser/parser_utils.c \
		$(SRCDIR)/parser/parser_syntax.c \
		$(SRCDIR)/parser/parser_syntax_utils.c \
		$(SRCDIR)/parser/parser_token_utils.c \
		$(SRCDIR)/parser/parser_logical.c \
		$(SRCDIR)/parser/parser_redirect.c \
		$(SRCDIR)/parser/parser_redirect_mixed.c \
		$(SRCDIR)/parser/parser_mixed_utils.c \
		$(SRCDIR)/executor/executor.c \
		$(SRCDIR)/executor/executor_logical.c \
		$(SRCDIR)/executor/builtin.c \
		$(SRCDIR)/executor/builtin_commands.c \
		$(SRCDIR)/executor/builtin_extra.c \
		$(SRCDIR)/executor/redirections.c \
		$(SRCDIR)/executor/redirect_utils.c \
		$(SRCDIR)/executor/heredoc_utils.c \
		$(SRCDIR)/executor/pipes.c \
		$(SRCDIR)/utils/utils.c \
		$(SRCDIR)/utils/env_utils.c \
		$(SRCDIR)/utils/env_array.c \
		$(SRCDIR)/utils/signal_utils.c \
		$(SRCDIR)/utils/signal_child.c \
		$(SRCDIR)/utils/input_utils.c \
		$(SRCDIR)/utils/cleanup.c \
		$(SRCDIR)/utils/expand.c \
		$(SRCDIR)/utils/expand_vars.c \
		$(SRCDIR)/utils/expand_utils.c \
		$(SRCDIR)/utils/expand_helpers.c \
		$(SRCDIR)/utils/expand_split.c \
		$(SRCDIR)/utils/expand_backslash.c \
		$(SRCDIR)/utils/expand_tilde.c \
		$(SRCDIR)/utils/expand_dollar_quote.c \
		$(SRCDIR)/utils/echo_utils.c \
		$(SRCDIR)/utils/path_utils.c \
		$(SRCDIR)/utils/args_utils.c \
		$(SRCDIR)/utils/export_utils.c \
		$(SRCDIR)/utils/export_sort.c \
		$(SRCDIR)/utils/error_utils.c \
		$(SRCDIR)/utils/wildcard.c \
		$(SRCDIR)/utils/wildcard_expand.c \
		$(SRCDIR)/utils/wildcard_utils.c \
		$(SRCDIR)/utils/env_utils2.c \
		$(SRCDIR)/parser/parser_command.c

# Bonus source files
BONUS_SRCS = $(SRCS) \
			$(SRCDIR)/bonus/logical_ops.c \
			$(SRCDIR)/bonus/wildcards.c

# Object files
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BONUS_OBJS = $(BONUS_SRCS:$(SRCDIR)/%.c=$(BONUS_OBJDIR)/%.o)

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

$(BONUS_OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< (bonus)..."
	@$(CC) $(BONUS_CFLAGS) -c $< -o $@

bonus: $(LIBFT) $(BONUS_OBJS)
	@echo "$(GREEN)Linking $(BONUS_NAME)...$(RESET)"
	@$(CC) $(BONUS_CFLAGS) $(BONUS_OBJS) $(LIBFT) $(LDFLAGS) -o $(BONUS_NAME)
	@echo "$(GREEN)$(BONUS_NAME) created successfully!$(RESET)"

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)
	@rm -rf $(BONUS_OBJDIR)
	@$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	@echo "$(RED)Cleaning $(NAME) and $(BONUS_NAME)...$(RESET)"
	@rm -f $(NAME)
	@rm -f $(BONUS_NAME)
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus

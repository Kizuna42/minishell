/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 04:05:29 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (token)
		ft_putstr_fd(token, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

int	check_pipe_syntax(t_token *token)
{
	if (!token->next)
	{
		print_syntax_error("|");
		return (0);
	}
	if (token->next->type == TOKEN_PIPE || token->next->type == TOKEN_AND
		|| token->next->type == TOKEN_OR)
	{
		print_syntax_error(token->next->value);
		return (0);
	}
	return (1);
}

int	check_logical_syntax(t_token *token)
{
	if (!token->next)
	{
		print_syntax_error(token->value);
		return (0);
	}
	if (token->next->type == TOKEN_PIPE || token->next->type == TOKEN_AND
		|| token->next->type == TOKEN_OR
		|| token->next->type == TOKEN_BACKGROUND)
	{
		print_syntax_error(token->next->value);
		return (0);
	}
	return (1);
}

int	check_redirect_syntax(t_token *token)
{
	if (!token->next || token->next->type != TOKEN_WORD)
	{
		if (token->next)
			print_syntax_error(token->next->value);
		else
			print_syntax_error(NULL);
		return (0);
	}
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	if (!check_first_token(current))
		return (0);
	while (current)
	{
		if (!validate_token_syntax(current))
			return (0);
		current = current->next;
	}
	return (1);
}

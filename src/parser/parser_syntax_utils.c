/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 03:57:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 04:09:46 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_first_token(t_token *current)
{
	if (is_operator_token(current->type) && current->type != TOKEN_REDIRECT_IN
		&& current->type != TOKEN_REDIRECT_OUT
		&& current->type != TOKEN_REDIRECT_APPEND
		&& current->type != TOKEN_REDIRECT_HEREDOC
		&& current->type != TOKEN_LPAREN)
	{
		print_syntax_error(current->value);
		return (0);
	}
	return (1);
}

int	check_semicolon_syntax(t_token *token)
{
	if (token->next && (token->next->type == TOKEN_PIPE
			|| token->next->type == TOKEN_AND
			|| token->next->type == TOKEN_OR
			|| token->next->type == TOKEN_BACKGROUND
			|| token->next->type == TOKEN_SEMICOLON))
	{
		print_syntax_error(token->next->value);
		return (0);
	}
	return (1);
}

int	validate_token_syntax(t_token *current)
{
	if (current->type == TOKEN_PIPE && !check_pipe_syntax(current))
		return (0);
	if ((current->type == TOKEN_AND || current->type == TOKEN_OR
			|| current->type == TOKEN_BACKGROUND)
		&& !check_logical_syntax(current))
		return (0);
	if (current->type == TOKEN_SEMICOLON && !check_semicolon_syntax(current))
		return (0);
	if (is_redirect_token(current->type) && !check_redirect_syntax(current))
		return (0);
	return (1);
}

static t_token	*find_paren_error(t_token *token, int *depth)
{
	while (token)
	{
		if (token->type == TOKEN_LPAREN && token->next
			&& token->next->type == TOKEN_RPAREN)
			return (token->next);
		if (token->type == TOKEN_LPAREN)
			(*depth)++;
		else if (token->type == TOKEN_RPAREN)
			(*depth)--;
		if (*depth < 0)
			return (token);
		token = token->next;
	}
	return (NULL);
}

int	check_paren_balance(t_token *token)
{
	int		depth;
	t_token	*bad;

	depth = 0;
	bad = find_paren_error(token, &depth);
	if (bad)
	{
		print_syntax_error(bad->value);
		return (0);
	}
	if (depth > 0)
	{
		print_syntax_error(NULL);
		return (0);
	}
	return (1);
}

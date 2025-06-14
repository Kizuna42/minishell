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

int	validate_token_syntax(t_token *current)
{
	if (current->type == TOKEN_PIPE && !check_pipe_syntax(current))
		return (0);
	if ((current->type == TOKEN_AND || current->type == TOKEN_OR
			|| current->type == TOKEN_BACKGROUND)
		&& !check_logical_syntax(current))
		return (0);
	if (is_redirect_token(current->type) && !check_redirect_syntax(current))
		return (0);
	return (1);
}
